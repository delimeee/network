#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_set>

#include "solver_hta.h"
#include "../constants.h"
#include "../Graph/graph.h"

#define EPSILON ERROR

SolverMT::SolverMT(Graph& g, bool is_huinya_ebanaya) : 
    graph(g),
    env{}, 
    model(env),
    cplex{env},
    is_float(true) 
{	
	auto HUINYA = is_huinya_ebanaya ? ILOFLOAT : ILOINT;
	n = 0;
	m = 0;
	for(size_t i = 0; i != g.size(); ++i) {
		if(g[i].type == NodeType::PowerStation) {
			m++;
			source_max.push_back(g[i].demand);
		} else {
			n++;
			demands.push_back(g[i].demand);
		}

	}

	Mdist = IloArray<IloNumArray>(env, n + m);
	for (int i = 0; i < n + m; i++)
		Mdist[i] = IloNumArray(env, n + m);
	for (int i = 0; i < n + m; i++) {
		for (int j = i + 1; j < n + m; j++) {
			Mdist[i][j] = [](const Node& n1, const Node& n2){
                return sqrt((n1.x - n2.x) * (n1.x - n2.x) + (n1.y - n2.y) * (n1.y - n2.y));
            }(graph[i], graph[j]);
			Mdist[j][i] = [](const Node& n1, const Node& n2){
                return sqrt((n1.x - n2.x) * (n1.x - n2.x) + (n1.y - n2.y) * (n1.y - n2.y));
            }(graph[j], graph[i]);
		}
	}

	z_sol = IloArray<IloNumArray>(env, n + m);
    for (int i = 0; i < n + m; i++)
        z_sol[i] = IloNumArray(env, n + m);

    // 1. ИСПРАВЛЕНО: Убрали повторное объявление типа (было IloArray<...> x). 
    // Теперь пишем напрямую в поле класса.
    x = IloArray<IloArray<IloNumVarArray>>(env, m);
    for (int k = 0; k < m; k++) {
        // ИСПРАВЛЕНО: Изменено с IloBoolVarArray на IloNumVarArray
        x[k] = IloArray<IloNumVarArray>(env, n + m);
        for (int i = 0; i < n + m; i++)
            x[k][i] = IloNumVarArray(env, n + m,  0.0, 1.0, HUINYA);
    }

    // 2. ИСПРАВЛЕНО: Убрали повторное объявление типа для y
    y = IloArray<IloArray<IloNumVarArray>>(env, m);
    for (int k = 0; k < m; k++) {
        y[k] = IloArray<IloNumVarArray>(env, n + m);
        for (int i = 0; i < n + m; i++)
            y[k][i] = IloNumVarArray(env, n + m, 0, MAX_FLOW);
    }

    // 3. ИСПРАВЛЕНО: Убрали повторное объявление типа для z
    z = IloArray<IloNumVarArray>(env, n + m);
    for (int i = 0; i < n + m; i++)
        z[i] = IloNumVarArray(env, n + m, 0.0, 1.0, HUINYA);

    for (int i = 0; i < n + m; i++) {
        for (int j = i; j < n + m; j++) {
            string name = "z_" + to_string(i) + "_" + to_string(j);
            // Явно указываем границы и тип, чтобы CPLEX корректно работал в режиме LP
            z[i][j] = IloNumVar(env, 0.0, 1.0, ILOFLOAT, name.c_str());
            z[j][i] = z[i][j]; 
        }
    }
	// y[i] = IloNumVarArray(env, graph.size(), 0.0, 1.0, ILOFLOAT);

	//CONSTRAINTS

	//EQUILIBRAGE DES SOURCES
	// for (int k = 0; k < m; k++)
	// 	for (int i = 0; i < m; i++)
	// 		for (int ii = 0; ii < m; ii++)
	// 			if (i != k && ii != k) {
	// 				model.add(IloSum(y[k][i+n]) - IloSum(y[k][ii+n]) <= +data->K);
	// 				model.add(IloSum(y[k][i+n]) - IloSum(y[k][ii+n]) >= -data->K);
	// 			}

	//CUT
	for (int k = 0; k < m; k++)
		for (int j = 0; j < n; j++)
			model.add(x[k][n + k][j] == 0);

	// FLOW CABLE
	for (int k = 0; k < m; k++) {
		for (int i = 0; i < n; i++) {
			IloExpr in(env);
			for (int j = 0; j < n + m; j++)
				in += y[k][j][i];
			model.add(in - IloSum(y[k][i]) == demands[i]);
			in.end();
		}
	}

	// MAXIMUM SOURCES OUTPUT
	// for (int k = 0; k < m; k++)
	// 	for (int i = n; i < n + m; i++)
	// 		model.add(IloSum(y[k][i]) == 2);

	for (int k = 0; k < m; k++) {
		for (int i = n; i < n + m; i++) {
			int src_idx = i - n;
			double cap = source_max[src_idx];
			model.add(IloSum(y[k][i]) <= cap);
		}
	}


	// MAX CAPACITY CABLE
	for (int k = 0; k < m; k++)
		for (int i = 0; i < n + m; i++)
			for (int j = 0; j < n + m; j++)
				model.add(y[k][i][j] <= MAX_FLOW * x[k][i][j]);

	// IN TRANSFO = 1
	for (int k = 0; k < m; k++) {
		for (int i = 0; i < n; i++) {
			IloExpr in(env);
			for (int j = 0; j < n + m; j++)
				in += x[k][j][i];
			model.add(in == 1);
			in.end();
		}
	}

	// OUT TRANSFO <= 1
	for (int k = 0; k < m; k++)
		for (int i = 0; i < n; i++)
			model.add(IloSum(x[k][i]) <= 1);

	// EXACT DEGREE 2
	for (int i = 0; i < n; i++)
		model.add(IloSum(z[i]) == 2);

	// NO LOOP
	for (int i = 0; i < n + m; i++)
		model.add(z[i][i] == 0);

	// NO FLOW RETURN TO SOURCES
	for (int k = 0; k < m; k++)
		for (int i = 0; i < n; i++)
			for (int j = n; j < n + m; j++)
				model.add(x[k][i][j] == 0);

	// CAN USE LINK ONLY IF BUILT
	for (int i = 0; i < n + m; i++)
		for (int j = 0; j < n + m; j++)
			for (int k = 0; k < m; k++)
				model.add(x[k][i][j] <= z[i][j]);


	// OBJECTIVE
	IloExpr obj(env);
	for (int i = 0; i < n + m; i++)
		// obj += IloScalProd(z[i], Mdist[i]) * cost/2;
		for (int j = i + 1; j < n + m; j++)
			obj += z[i][j] * Mdist[i][j];
	model.add(IloMinimize(env, obj));
	obj.end();

}

SolverMT::~SolverMT() { 
	env.end();
}

bool SolverMT::solve() {
	cplex.extract(model);
	bool res;
	try {
		res = cplex.solve();
	} catch (IloException &e) {
		cout << e << endl;
		throw e;
	}

	cout << "Solution status = " << cplex.getStatus() << endl;
	if (res) {
		cout << "Solution value  = " << cplex.getObjValue() << endl;
		if (!is_float) {
            cout << "Solution relative gap = " << cplex.getMIPRelativeGap()*100 << "%" << endl;
        }

		for (int i = 0; i < n + m; i++) {
        	cplex.getValues(z_sol[i], z[i]);
		}

		for(int i = 0; i < n + m; ++i) {
			for(int j = 0; j < n + m; ++j) {
				graph(i, j).value = IloRound(z_sol[i][j]);
			}
		}
	}
	return res;
}

Graph SolverMT::get_solution() {
	return graph;
}

// Setproblemtype

void SolverMT::add_survivable_constraint(const std::unordered_set<size_t>& nodes, double rvalue) {
    IloExpr surv_expr(env);
    for(auto& v: nodes) {
        // std::cout << v + 1 << '\n';
        for(size_t i = 0; i != graph.size(); ++i) {
            if(!nodes.contains(i)) {
                surv_expr += z[i][v];
            }
        }
    }
    // std::cout << "ADDED CONSTRAINT\n";
    // std::cout << surv_expr << rvalue << '\n';
    model.add(surv_expr >= rvalue);
    surv_expr.end();
}

void SolverMT::add_survivable_constraint(
    const std::vector<std::pair<std::unordered_set<size_t>, double>>& constrains
) {
    for(auto& constraint: constrains) {
        auto& [nodes, rvalue] = constraint;
        SolverMT::add_survivable_constraint(nodes, rvalue);
    }
}

void SolverMT::add_mip_start(const Graph& g) {
	IloNumVarArray startVars(env);
    IloNumArray startValues(env);

	for(size_t i = 0; i != g.size(); ++i) {
		for(size_t j = 0; j != g.size(); ++j) {
			startVars.add(z[i][j]);
			startValues.add(IloRound(g(i, j).value));
		}
	}

	cplex.extract(model);
	cplex.addMIPStart(startVars, startValues);
}
#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_set>

#include "solver_hta.h"
#include "../constants.h"
#include "../Graph/graph.h"

#define EPSILON ERROR

SolverMT::SolverMT(Graph& g): graph(g) {
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
		z_sol[i] = IloNumArray(env);
}

SolverMT::~SolverMT() { }

bool SolverMT::solve() {
	//VARS
	IloArray<IloArray<IloBoolVarArray>> x(env, m);
	for (int k = 0; k < m; k++) {
		x[k] = IloArray<IloBoolVarArray>(env, n + m);
		for (int i = 0; i < n + m; i++)
			x[k][i] = IloBoolVarArray(env, n + m);
	}

	// Ykij
	IloArray<IloArray<IloNumVarArray>> y(env, m);
	for (int k = 0; k < m; k++) {
		y[k] = IloArray<IloNumVarArray>(env, n + m);
		for (int i = 0; i < n + m; i++)
			y[k][i] = IloNumVarArray(env, n + m, 0, MAX_FLOW);
	}

	// Zij
	IloArray<IloBoolVarArray> z(env, n + m);
	for (int i = 0; i < n + m; i++)
			z[i] = IloBoolVarArray(env, n + m);

	for (int i = 0; i < n + m; i++) {
		for (int j = i; j < n + m; j++) {
			string name = "z_" + to_string(i) + "_" + to_string(j);
			z[i][j] = IloBoolVar(env, name.c_str());
			z[j][i] = z[i][j]; // REFERENCE INSTEAD OF EXTRA VAR
		}
	}


	// CONSTRAINTS
	IloModel model(env);

	// EQUILIBRAGE DES SOURCES
	// for (int k = 0; k < m; k++)
	// 	for (int i = 0; i < m; i++)
	// 		for (int ii = 0; ii < m; ii++)
	// 			if (i != k && ii != k) {
	// 				model.add(IloSum(y[k][i+n]) - IloSum(y[k][ii+n]) <= +data->K);
	// 				model.add(IloSum(y[k][i+n]) - IloSum(y[k][ii+n]) >= -data->K);
	// 			}

	// CUT
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
	// 		model.add(IloSum(y[k][i]) <= data->output_max);
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


	// SOLVE
	IloCplex cplex(model);
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
		cout << "Solution relative gap = " << cplex.getMIPRelativeGap()*100 << "%" << endl;

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

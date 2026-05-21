#ifndef SOLVER_HTA_H
#define SOLVER_HTA_H

#include <ilcplex/ilocplex.h>

#include "../Graph/graph.h"

using namespace std;

class SolverMT {

	public:
		SolverMT(Graph&);
		~SolverMT();

		bool solve();

		Graph get_solution();
		void export_results();
		void add_survivable_constraint(const std::unordered_set<size_t>&, double);
        void add_survivable_constraint(
            const std::vector<std::pair<std::unordered_set<size_t>, double>>&
        );
		void switch_model();

	private:
		int n;
		int m;
		Graph graph;
		bool is_float=true;
		vector<double> demands;
		vector<double> source_max;

		IloEnv env;
		IloArray<IloNumArray> Mdist;
		IloExtractableArray conversions;

		IloModel model;
		IloCplex cplex;
		IloArray<IloArray<IloNumVarArray>> x;
        IloArray<IloArray<IloNumVarArray>> y;
        IloArray<IloNumVarArray> z;
		IloArray<IloNumArray> z_sol;
};

#endif // SOLVER_HTA_H

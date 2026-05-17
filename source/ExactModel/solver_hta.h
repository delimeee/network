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

	private:
		int n;
		int m;
		Graph graph;
		vector<double> demands;
		IloArray<IloNumArray> Mdist;
		vector<double> source_max;

		IloArray<IloNumArray> z_sol;
		IloEnv env;
};

#endif // SOLVER_HTA_H

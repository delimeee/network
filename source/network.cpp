#include <ilcplex/ilocplex.h>
#include<iostream>
#include<vector>


typedef struct Node {
    double x;
    double y;
    double weitht;
}Node;


double metric(Vertex edge1, Vertex edge2);

int main(size_t argc, char* argv[]) {
    try {
        size_t powerst_amount;
        size_t cities_amount;
        std::cin >> powerst_amount >> cities_amount;

        size_t n = powerst_amount + cities_amount;

        std::vector<Node> node(n);
        for(size_t i = 0; i != n; ++i) {
            std::cin >> node[i].x >> node[i].y >> node[i].weitht;
        }

        IloEnv env;

        IloArray<IloNumArray> distance(env, node.size());
        for(int i = 0; i != v_vtxs.size(); ++i) {
            distance[i] = IloNumArray(env, node.size());
        }

        for(int i = 0; i != node.size(); i++) {
            for(int j = 0; j != node.size(); j++) {
                distance[i][j] = metric(node[i], node[j]);
            }
        }

        IloArray<IloNumVarArray> y(env, node.size());
        for(int i = 0; i != node.size(); ++i) {
            y[i] = IloNumVarArray(env, node.size(), 0.0, 1.0, ILOFLOAT);
            for(int j = 0; j != node.size(); ++j) {
                std::string var_name = "y" + std::to_string(i+1) + '_' + std::to_string(j+1);
                y[i][j].setName(node.c_str());
            }
        }

        IloArray<IloNumVarArray> f(env, node.size());
        for(int i = 0; i != node.size(); ++i) {
            f[i] = IloNumVarArray(env, node.size(), 0.0, 1.0, ILOFLOAT);
            for(int j = 0; j != node.size(); ++j) {
                std::string var_name = "f" + std::to_string(i+1) + '_' + std::to_string(j+1);
                f[i][j].setName(node.c_str());
            }
        }

        IloModel model(env);
        



    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

double metric(Vertex edge1, Vertex edge2) {
    return sqrt((edge1.x - edge2.x) * (edge1.x - edge2.x) + (edge1.y - edge2.y)*(edge1.y - edge2.y));
}
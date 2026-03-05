#ifndef W_GRAPH_H
#define W_GRAPH_H
#include<vector>

template<typename EDGE_W, typename NODE_W>
class Graph {
    private:
        std::vector<std::vector<EDGE_W>> adjacency_matrix;
        std::vector<NODE_W> node_worth;
        size_t nodes_amount;

    public:
        Graph() = delete;
        Graph(size_t);
        size_t size();
        std::vector<EDGE_W> nodes(size_t) const;
        void change_node(size_t, NODE_W);
        void change_edge(size_t, size_t, EDGE_W);
        NODE_W get_node(size_t) const;
        EDGE_W get_edge(size_t, size_t) const;
        std::vector<EDGE_W> operator[](size_t i);
};

#endif
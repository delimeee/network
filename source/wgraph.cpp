#include"../header/wgraph.h"

Graph::Graph(size_t n): nodes_amount{n}, node_worth(n), adjacency_matrix(n, std::vector<EDGE_W>(n)) { }

const std::vector<EDGE_W>& Graph::nodes(size_t i) {
    return adjacency_matrix[i];
}

void Graph::change_node(size_t i, NODE_W w) {
    node_worth[i] = w;
}

void Graph::change_edge(size_t i, size_t j, EDGE_W w) {
    adjacency_matrix[i][j] = w;
}

NODE_W Graph::get_node(size_t i) const{
    return node_worth[i];
}

EDGE_W Graph::get_edge(size_t i, size_t j) const{
    return adjacency_matrix[i][j];
}

size_t Graph::size() const{
    return nodes_amount;
}

const std::vector<EDGE_W>& Graph::operator[](size_t i) {
    return adjacency_matrix[i];
}
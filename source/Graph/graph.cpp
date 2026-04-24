#include "graph.h"
#include "../constants.h"
#include<stdexcept>

// class Graph {
//     private:
//         std::vector<std::vector<Edge>> adj;
//         std::vector<Node> nodes;
//     public:
//         void add_edge(Edge&);
//         void add_node(Node&);
//         const std::vector<Node>& neighbors(size_t);

// };

Node::Node(double x_p, double y_p, double d, NodeType t)
    : x{x_p}, y{y_p}, demand{d}, type{t} 
{ }

Node::Node(double x_p, double y_p, NodeType t)
    : Node::Node(x_p, y_p, 0.0, t)
{ }

Node::Node()
    : Node::Node(0.0, 0.0, NodeType::City)
{ }

Edge::Edge(size_t fromp, size_t top, double costp, double maxp, double flowp, double valp)
    : from{fromp}, to{top}, cost{costp}, max_flow{maxp}, flow{flowp}, value{valp} 
{ }

Edge::Edge(size_t fromp, size_t top)
    : Edge::Edge(fromp, top, 0.0, 0.0, 0.0, 0.0)
{ }

Edge::Edge()
    : Edge(0, 0)
{ }

Graph::Graph() { }

Graph::Graph(size_t n): adj(n, std::vector<Edge>(n)), nodes(n) {
    for(size_t i = 0; i != n; ++i) {
        for(size_t j = 0; j != n; ++j) {
            adj[i][j].to = j;
            adj[i][j].from = i;
        }
    }
}

Graph::Graph(const std::vector<Node>& nodesp)
    : Graph(nodesp.size())
{
    nodes = nodesp;
}

void Graph::add_edge(const Edge& e) {
    if(adj.size() <= e.from || adj.size() <= e.to) {
        throw std::out_of_range("Edge links to not existing nodes!");
    }
    adj[e.from][e.to] = e;
}

void Graph::add_node(const Node& a) {
    nodes.push_back(a);
    
    size_t n = adj.size();
    for(size_t i = 0; i != n; ++i) {
        adj[i].emplace_back(i, n);
    }

    adj.emplace_back();
    for(size_t i = 0; i <= n; i++) {
        adj[n].emplace_back(n, i);
    }
}

void Graph::delete_edge(size_t i, size_t j) {
    if(adj.size() <= i || adj.size() <= j) {
        throw std::out_of_range("Edge links to not existing nodes!");
    }

    adj[i][j].value = 0.0;
}

void Graph::delete_edge(Edge& e) {
    delete_edge(e.from, e.to);
}

Edge& Graph::get_edge(size_t i, size_t j) {
    if(adj.size() <= i || adj.size() <= j) {
        throw std::out_of_range("Edge links to not existing nodes!");
    }

    return adj[i][j];
}

std::vector<Edge>& Graph::get_edges(size_t i) {
    if(adj.size() <= i) {
        throw std::out_of_range("Node doesn't exist!");
    }

    return adj[i];
}

Node& Graph::get_node(size_t i) {
    if(adj.size() <= i) {
        throw std::out_of_range("Node doesn't exist!");
    }

    return nodes[i];
}


const Edge& Graph::get_edge(size_t i, size_t j) const {
    if(adj.size() <= i || adj.size() <= j) {
        throw std::out_of_range("Edge links to not existing nodes!");
    }

    return adj[i][j];
}

const Node& Graph::get_node(size_t i) const {
        if(adj.size() <= i) {
        throw std::out_of_range("Node doesn't exist!");
    }

    return nodes[i];
}

const std::vector<Edge>& Graph::get_edges(size_t i) const {
    if(adj.size() <= i) {
        throw std::out_of_range("Node doesn't exist!");
    }

    return adj[i];
}

Edge& Graph::operator()(size_t i, size_t j) {
    if (i >= adj.size() || j >= adj.size()) {
        throw std::out_of_range("Edge doesn't exist");
    }

    return adj[i][j];
}

const Edge& Graph::operator()(size_t i, size_t j) const {
    if (i >= adj.size() || j >= adj.size()) {
        throw std::out_of_range("Edge doesn't exist");
    }

    return adj[i][j];
}

Node& Graph::operator[](size_t i) {
    if (i >= nodes.size()) {
        throw std::out_of_range("Node doesn't exist");
    }

    return nodes[i];
}

const Node& Graph::operator[](size_t i) const {
    if (i >= nodes.size()) {
        throw std::out_of_range("Node doesn't exist");
    }

    return nodes[i];
}

size_t Graph::size() const {
    return nodes.size();
}

bool Graph::has_edge(size_t i, size_t j) const {
    return adj[i][j].value > ERROR;
}
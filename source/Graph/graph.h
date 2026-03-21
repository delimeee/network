#ifndef GRAPH_H
#define GRAPH_H
#include<vector>
#define ERROR 1.e-8
enum class NodeType {
    City,
    PowerStation
};

struct Node {
    double x;
    double y;
    double demand;
    NodeType type;

    Node(double, double, double, NodeType);
    Node(double, double, NodeType);
    Node();
};

struct Edge {
    size_t from;
    size_t to;
    double cost;
    double max_flow;
    double flow;
    double value;

    Edge();
    Edge(size_t, size_t);
    Edge(size_t, size_t, double, double, double, double);
};

class Graph {
    private:
        std::vector<std::vector<Edge>> adj;
        std::vector<Node> nodes;
    public:
        // --- constructors ---
        Graph();
        Graph(size_t n);
        Graph(const std::vector<Node>& nodes);

        // --- modifiers ---
        void add_node(const Node&);
        void add_edge(const Edge&);
        void delete_edge(size_t, size_t);
        void delete_edge(Edge&);

        // --- element access ---
        Edge& get_edge(size_t, size_t);
        const Edge& get_edge(size_t, size_t) const;

        std::vector<Edge>& get_edges(size_t);
        const std::vector<Edge>& get_edges(size_t) const;

        Node& get_node(size_t);
        const Node& get_node(size_t) const;

        // --- operators ---
        Edge& operator()(size_t, size_t);
        const Edge& operator()(size_t, size_t) const;

        Node& operator[](size_t);
        const Node& operator[](size_t) const;

        // --- capacity / info ---
        size_t size() const;

        // --- queries ---
        bool has_edge(size_t, size_t) const;

};
#endif
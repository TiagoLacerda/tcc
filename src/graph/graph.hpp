#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef SET
#define SET
#include <set>
#endif

#ifndef STRING
#define STRING
#include <string>
#endif

/// @brief An implementation of undirected graphs using an adjacency list.
class Graph
{
public:
    /// @brief Default constructor.
    Graph();

    /// @brief Copy constructor.
    /// @param graph a Graph.
    Graph(Graph &graph);

    /// @brief Construct a complete graph, where every pair of distinct nodes is connected by an edge.
    /// @param n The number of nodes in the graph.
    /// @return A complete graph of n nodes.
    static Graph complete(unsigned int n);

    /// @brief Insert a new node in the graph.
    void insert_node();

    /// @brief Insert a new edge in the graph, between nodes u and v. If u and v are the same, out of bounds or already neighbors, nothing changes.
    /// @param u Index of a node in the graph.
    /// @param v Index of a node in the graph.
    void insert_edge(unsigned int u, unsigned int v);

    /// @brief Remove a node from the graph. If u is out of bounds, nothing changes.
    /// @param u Index of a node in the graph.
    void remove_node(unsigned int u);

    /// @brief Remove an edge from the graph, between nodes u and v. If u and v are the same, out of bounds or not neighbors, nothing changes.
    /// @param u Index of a node in the graph.
    /// @param v Index of a node in the graph.
    void remove_edge(unsigned int u, unsigned int v);

    /// @brief Get the number of nodes in the graph.
    /// @return the number of nodes in the graph.
    int get_n();

    /// @brief Get the number of edges in the graph.
    /// @return the number of edges in the graph.
    int get_m();

    /// @brief Get the degree of the lowest-degree node in the graph.
    /// @return the degree of the lowest-degree node in the graph.
    int get_minimum_degree();

    /// @brief Get the degree of the highest-degree node in the graph.
    /// @return the degree of the highest-degree node in the graph.
    int get_maximum_degree();

    /// @brief Check whether the graph contains cycles.
    /// @return true if a cycle is found, false otherwise.
    bool is_cyclic();

    /// @brief Check whether the graph is connected. The Null-Graph is said to be connected.
    /// @return true if the graph is connected, false otherwise.
    bool is_connected();

    /// @brief Generate a JSON representation of the graph.
    /// @return an std::string equivalent of the graph.
    std::string to_json();

private:
    std::vector<std::vector<unsigned int>> adjacencies;
    unsigned int n;
    unsigned int m;

    Graph(std::vector<std::vector<unsigned int>> adjacencies, unsigned int n, unsigned int m);

    bool is_cyclic_internal(unsigned int node, unsigned int parent, std::set<unsigned int> *visited);

    void is_connected_internal(unsigned int node, std::set<unsigned int> *visited);
};

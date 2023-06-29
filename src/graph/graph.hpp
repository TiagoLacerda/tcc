#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef SET
#define SET
#include <set>
#endif

#ifndef TUPLE
#define TUPLE
#include <tuple>
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
    /// @param graph A Graph.
    Graph(Graph &graph);

    /// @brief Construct a graph with n nodes, but no edges.
    /// @param n The number of nodes in the graph.
    /// @return A graph with n nodes, but no edges.
    Graph(unsigned int n);

    /// @brief Construct a graph that is the inverse or complement of another.
    /// @param graph A graph.
    /// @return A graph that is the inverse or complement of another.
    static Graph inverse(Graph &graph);

    /// @brief Construct a complete graph, where every pair of distinct nodes is connected by an edge.
    /// @param n The number of nodes in the graph.
    /// @return A complete graph with n nodes.
    static Graph complete(unsigned int n);

    /// @brief Construct a spanning tree with n nodes, where edges are chosen at random without violating the properties that make up a tree.
    /// @param n The number of nodes in the graph.
    /// @return A spanning tree.
    static Graph random_spanning_tree(unsigned int n);

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
    /// @return The number of nodes in the graph.
    unsigned int get_n();

    /// @brief Get the number of edges in the graph.
    /// @return The number of edges in the graph.
    unsigned int get_m();

    /// @brief Get the degree of the lowest-degree node in the graph.
    /// @return The degree of the lowest-degree node in the graph.
    unsigned int get_minimum_degree();

    /// @brief Get the degree of the highest-degree node in the graph.
    /// @return The degree of the highest-degree node in the graph.
    unsigned int get_maximum_degree();

    /// @brief Whether there is an edge connecting u and v,
    /// @param u Index of a node in the graph.
    /// @param v Index of a node in the graph.
    /// @return true if there is an edge connecting u and v, false otherwise.
    bool has_edge(unsigned int u, unsigned int v);

    /// @brief Get a list of all the edges in the graph.
    /// @return An std::vector of std::tuple representing each edge.
    std::set<std::tuple<unsigned int, unsigned int>> get_edges();

    /// @brief Check whether the graph contains cycles.
    /// @return true if a cycle is found, false otherwise.
    bool is_cyclic();

    /// @brief Check whether the graph is connected. The Null-Graph is said to be connected.
    /// @return true if the graph is connected, false otherwise.
    bool is_connected();

    /// @brief Generate a JSON representation of the graph.
    /// @return An std::string equivalent of the graph.
    std::string to_json();

    /// @brief Generate a file at the given path with a representation of the graph. The first n lines represent each node with an index. The next m lines represent each edge with a pair of indexes.
    /// @param path The path of the file to be created.
    void to_file(std::string path);

private:
    std::vector<std::vector<unsigned int>> adjacencies;
    unsigned int n;
    unsigned int m;

    Graph(std::vector<std::vector<unsigned int>> adjacencies, unsigned int n, unsigned int m);

    bool is_cyclic_internal(unsigned int node, unsigned int parent, std::set<unsigned int> *visited);

    void is_connected_internal(unsigned int node, std::set<unsigned int> *visited);
};

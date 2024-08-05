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
    Graph(const Graph &graph);

    /// @brief Construct a graph with n nodes, but no edges.
    /// @param n The number of nodes in the graph.
    /// @return A graph with n nodes, but no edges.
    Graph(int n);

    /// @brief Construct a graph that is the inverse or complement of another.
    /// @param graph A graph.
    /// @return A graph that is the inverse or complement of another.
    static Graph inverse(Graph &graph);

    /// @brief Construct a complete graph, where every pair of distinct nodes is connected by an edge.
    /// @param n The number of nodes in the graph.
    /// @return A complete graph with n nodes.
    static Graph complete(int n);

    /// @brief Construct a cycle graph, where each node is connected to exactly two other nodes, forming a single cycle.
    /// @param n The number of nodes in the graph.
    /// @return A cycle graph with n nodes.
    static Graph cycle(int n);

    /// @brief Construct a spanning tree with n nodes, where edges are chosen at random without violating the properties that make up a tree.
    /// @param n The number of nodes in the graph.
    /// @return A spanning tree.
    static Graph random_spanning_tree(int n);

    /// @brief Insert a new node in the graph.
    void insert_node();

    /// @brief Insert a new edge in the graph, between nodes u and v. If u and v are the same, out of bounds or already neighbors, nothing changes.
    /// @param u Index of a node in the graph.
    /// @param v Index of a node in the graph.
    void insert_edge(int u, int v);

    /// @brief Remove a node from the graph. If u is out of bounds, nothing changes.
    /// @param u Index of a node in the graph.
    void remove_node(int u);

    /// @brief Remove an edge from the graph, between nodes u and v. If u and v are the same, out of bounds or not neighbors, nothing changes.
    /// @param u Index of a node in the graph.
    /// @param v Index of a node in the graph.
    void remove_edge(int u, int v);

    /// @brief Get the number of nodes in the graph.
    /// @return The number of nodes in the graph.
    int get_n();

    /// @brief Get the number of edges in the graph.
    /// @return The number of edges in the graph.
    int get_m();

    /// @brief Get the degree of the lowest-degree node in the graph.
    /// @return The degree of the lowest-degree node in the graph.
    int get_minimum_degree();

    /// @brief Get the degree of the highest-degree node in the graph.
    /// @return The degree of the highest-degree node in the graph.
    int get_maximum_degree();

    /// @brief Whether there is an edge connecting u and v,
    /// @param u Index of a node in the graph.
    /// @param v Index of a node in the graph.
    /// @return true if there is an edge connecting u and v, false otherwise.
    bool has_edge(int u, int v);

    /// @brief Get a list of all the edges in the graph.
    /// @return An std::vector of std::tuple representing each edge.
    std::vector<std::tuple<int, int>> get_edges();

    /// @brief Check whether the graph contains cycles.
    /// @return true if a cycle is found, false otherwise.
    bool is_cyclic();

    /// @brief Check whether the graph contains cycles using a depth-first search tree.
    /// @return true if a cycle is found, false otherwise.
    bool is_cyclic_depth_first_search();

    /// @brief Check whether the graph contains cycles using disjoint sets.
    /// @return true if a cycle is found, false otherwise.
    bool is_cyclic_disjoint_sets();

    /// @brief Check whether the graph is connected. The Null-Graph is said to be connected.
    /// @return true if the graph is connected, false otherwise.
    bool is_connected();

    /// @brief Check whether the graph is connected using a depth-first search tree. The Null-Graph is said to be connected.
    /// @return true if the graph is connected, false otherwise.
    bool is_connected_depth_first_search();

    /// @brief Check whether the graph is connected using disjoint sets. The Null-Graph is said to be connected.
    /// @return true if the graph is connected, false otherwise.
    bool is_connected_disjoint_sets();

    /// @brief Generate a JSON representation of the graph.
    /// @return An std::string equivalent of the graph.
    std::string to_json();

    /// @brief Insert up to k randomly chosen edges in the graph. S
    /// @param k The number of edges to be inserted.
    void insert_random_edges(int k);

    /// @brief Generate a file at the given path with a representation of the graph. The first n lines represent each node with an index. The next m lines represent each edge with a pair of indexes.
    /// @param path The path of the file to be created.
    void to_file(std::string path);

private:
    std::vector<std::vector<int>> adjacencies;
    int n;
    int m;

    Graph(std::vector<std::vector<int>> adjacencies, int n, int m);

    bool is_cyclic_depth_first_search_internal(int node, int parent, std::set<int> *visited);

    void is_connected_depth_first_search_internal(int node, std::set<int> *visited);
};

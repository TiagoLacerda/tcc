#ifndef FUNCTIONAL
#define FUNCTIONAL
#include <functional>
#endif

class Graph;

namespace spanning_tree
{
    /// @brief Generate all spanning trees of a graph using a single-threaded approach. For details, see https://doi.org/10.1016/j.ipl.2022.106265.
    /// @param graph A graph.
    /// @param callback A function to be called whenever a new spanning tree is found.
    void generate_sequential(const Graph &graph, const std::function<int(const Graph &tree)> &callback, const bool *abort, const int lower_bound);

    /// @brief Generate all spanning trees of a graph using a multi-threaded approach.
    /// @param graph A graph.
    /// @param callback A function to be called whenever a new spanning tree is found.
    /// @param num_threads Number of threads between which to split the workload.
    void generate_parallel(const Graph &graph, const std::function<int(const Graph &tree)> &callback, const bool *abort, const int lower_bound, const int num_threads);

    /// @brief Determine the start and end positions for the first pointer in a search.
    /// @param n The number of nodes in the graph.
    /// @param m The number of edges in the graph.
    /// @param i The index of the current thread.
    /// @param num_threads The number of threads.
    /// @param start When attempting to assign pointers, the search will start at this position in the edge list.
    /// @param end When attempting to assign pointers, the search will halt when the leftmost pointer reaches this position. In a single thread context, this would be `m - (n - 1) + 1`, since there wouldn't be enough edges to the right to form a spanning tree.
    void workload(const int n, const int m, const int i, const int num_threads, int *start, int *end);
}
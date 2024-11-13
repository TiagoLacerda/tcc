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
    void generate(const Graph &graph, const std::function<int(const Graph &tree)> &callback, const bool *abort, const int lower_bound, const int num_threads);
}
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
    void generate_parallel(const Graph &graph, const std::function<int(const Graph &tree)> &callback, const bool *abort, const int lower_bound, const int num_threads, const std::vector<int> start, const std::vector<int> end);

    /// @brief Determine the start and end positions for the first pointer in a search for each thread, as well as the number of actual worker threads.
    /// @param graph A graph.
    /// @param num_threads Desired number of threads, which might be higher than the number of actual worker threads used, but not lower.
    /// @return A tuple containing the number of worker threads as well as the start and end positions of the initial pointer of the search for each thread.
    std::tuple<int, std::vector<int>, std::vector<int>> get_workload(const Graph &graph, const int num_threads);
}
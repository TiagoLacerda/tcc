#ifndef FUNCTIONAL
#define FUNCTIONAL
#include <functional>
#endif

class Graph;

namespace spanning_tree
{
    /// @brief Generate all spanning trees of a graph.
    /// @param graph A graph.
    /// @param callback A function to be called whenever a new spanning tree is found.
    void generate(Graph graph, std::function<void(Graph tree)> callback);
}
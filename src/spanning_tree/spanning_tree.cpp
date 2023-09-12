#ifndef FUNCTIONAL
#define FUNCTIONAL
#include <functional>
#endif

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.hpp"
#endif

#ifndef SPANNING_TREE
#define SPANNING_TREE
#include "spanning_tree.hpp"
#endif

namespace spanning_tree
{
    void generate(Graph graph, std::function<void(Graph tree)> callback)
    {
        auto n = graph.get_n();
        auto m = graph.get_m();

        if (m < n - 1)
        {
            return; // Not enough edges in the graph to construct a spanning tree.
        }

        // Determine initial pointer assignment.

        auto candidate = Graph(n);

        auto edges = graph.get_edges();

        unsigned int pointers[n - 1];

        unsigned int i = 0; // Index of pointer.
        unsigned int j = 0; // Index of edge.

        while (i < n - 1 && j < m)
        {
            auto u = std::get<0>(edges[j]);
            auto v = std::get<1>(edges[j]);

            candidate.insert_edge(u, v);

            if (candidate.is_cyclic_disjoint_sets())
            {
                candidate.remove_edge(u, v);
            }
            else
            {
                pointers[i] = j;
                i++;
            }

            j++;
        }

        if (i < n - 1)
        {
            return; // Unable to determine an initial pointer assignment.
        }

        // Generate all trees.

        unsigned int index = 0;

        unsigned int p = n - 2; // Index of furthermost assigned pointer.

        while (pointers[0] < m - (n - 1) + 1)
        {
            if (p == 0 || pointers[p] != pointers[p - 1])
            {
                auto u = std::get<0>(edges[pointers[p]]);
                auto v = std::get<1>(edges[pointers[p]]);
                candidate.remove_edge(u, v);
            }

            pointers[p]++;

            if (pointers[p] < m)
            {
                auto u = std::get<0>(edges[pointers[p]]);
                auto v = std::get<1>(edges[pointers[p]]);
                candidate.insert_edge(u, v);

                if (!candidate.is_cyclic_disjoint_sets()) // Is acyclic
                {
                    if (p == n - 2) // Has n - 1 edges.
                    {
                        if (candidate.is_connected_disjoint_sets()) // Is connected
                        {
                            callback(candidate);
                        }
                    }
                    else
                    {
                        p++;
                        pointers[p] = pointers[p - 1];
                    }
                }
            }
            else
            {
                p--;
            }
        }
    }
}
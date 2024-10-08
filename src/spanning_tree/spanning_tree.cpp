#ifndef FUNCTIONAL
#define FUNCTIONAL
#include <functional>
#endif

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.hpp"
#endif

#ifndef OMP
#define OMP
#include <omp.h>
#endif

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef SPANNING_TREE
#define SPANNING_TREE
#include "spanning_tree.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "../util/util.hpp"
#endif

namespace spanning_tree
{
    namespace
    {
        void generate_internal(const Graph &graph, std::function<void(int stretch_factor)> callback, int lower_bound, bool early_halt, int start, int end)
        {
            auto n = graph.get_n();
            auto m = graph.get_m();

            if (m < n - 1)
            {
                return; // Not enough edges in the graph to construct a spanning tree.
            }

            auto candidate = Graph(n);

            auto edges = graph.get_edges();

            int pointers[n - 1];

            // Determine initial pointer assignment.

            int i = 0;     // Index of pointer.
            int j = start; // Index of edge.

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

            auto stretch_factor = stretch(graph, candidate);

            callback(stretch_factor);

            // TODO: Avoid this silly code repetition
            if (stretch_factor <= lower_bound && early_halt)
            {
                return;
            }

            // Generate all remaining trees.

            int p = n - 2; // Index of furthermost assigned pointer.

            while (pointers[0] < end)
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
                                stretch_factor = stretch(graph, candidate);

                                callback(stretch_factor);

                                // TODO: Avoid this silly code repetition
                                if (stretch_factor <= lower_bound && early_halt)
                                {
                                    return;
                                }
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

    void generate(const Graph &graph, const std::function<void(int stretch_factor)> &callback, int lower_bound, bool early_halt, int num_threads)
    {
        auto n = graph.get_n();
        auto m = graph.get_m();

        if (num_threads < 2)
        {
            generate_internal(graph, callback, lower_bound, early_halt, 0, m - (n - 1) + 1);
        }
        else
        {
#pragma omp parallel num_threads(num_threads)
            {
                int i = omp_get_thread_num(), start, end;

                workload(n, m, i, num_threads, &start, &end);

                generate_internal(graph, callback, lower_bound, early_halt, start, end);
            }
        }
    }

    void workload(int n, int m, int i, int num_threads, int *start, int *end)
    {
        if (n == 0 || m == 0 || m > n * (n - 1) / 2 || num_threads == 0 || i >= num_threads) // Invalid parameters.
        {
            *start = m;
            *end = m;
        }
        else
        {
            int total = m - (n - 1) + 1; // If search started here, there woudldn't be enough edges to make a spanning tree.

            if (num_threads > total) // There are more threads than work available.
            {
                if (i < total) // First threads are busy.
                {
                    *start = i;
                    *end = *start + 1;
                }
                else // Last threads are idle.
                {
                    *start = m;
                    *end = m;
                }
            }
            else // There is more work than threads.
            {
                int chunk = total / num_threads;

                *start = chunk * i;
                *end = (i == num_threads - 1) ? total : *start + chunk;
            }
        }
    }
}
#include "macros.h"

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef IOMANIP
#define IOMANIP
#include <iomanip>
#endif

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

    void generate_sequential(const Graph &graph, const std::function<int(const Graph &tree)> &callback, const bool *abort, const int lower_bound)
    {
        int n = graph.get_n();
        int u;
        int v = 0;
        int next_neighbor[n];
        int last_neighbor[n];

        Graph tree(n);

        for (int i = 0; i < n; i++)
        {
            next_neighbor[i] = 0;
            last_neighbor[i] = -1;
        }

        while (v >= 0 && !(*abort))
        {
            if (next_neighbor[v] == graph.get_degree(v))
            {
                next_neighbor[v] = 0;

                --v;

                if (v < 0)
                {
                    break;
                }

                tree.remove_edge(v, last_neighbor[v]);

                last_neighbor[v] = -1;
            }
            else
            {
                u = graph.get_neighbors(v)[next_neighbor[v]];

                next_neighbor[v]++;

                if (!tree.has_edge(v, u))
                {
                    tree.insert_edge(v, u);

                    last_neighbor[v] = u;

                    if (!tree.is_cyclic_disjoint_sets())
                    {
                        if (tree.get_m() == tree.get_n() - 1)
                        {
                            callback(tree);
                        }
                        else
                        {
                            v++;
                            continue;
                        }
                    }

                    tree.remove_edge(v, u);
                }
            }
        }
    }

    std::tuple<int, std::vector<int>, std::vector<int>> get_workload(const Graph &graph, const int num_threads)
    {
        auto n = graph.get_n();
        auto m = graph.get_m();

        // Graph has no vertices
        // Graph has no edges
        // Graph doesn't have enough vertices to be connected
        // Graph has more edges than a complete graph with [n] nodes (multigraph, not supported)
        // Can't have 0 worker threads

        if (n == 0 || m == 0 || m < n - 1 || m > n * (n - 1) / 2 || num_threads == 0)
        {
            // TODO: Break up each criterion and provide a specific message as to why work was aborted.
            DEBUG_ONLY(std::cout << "There was no work to be done by any thread." << std::endl;)

            return std::tuple<int, std::vector<int>, std::vector<int>>(0, {}, {});
        }

        auto threshold = m - (n - 1) + 1; // If search started here, there woudldn't be enough edges to make a spanning tree.

        auto edges = graph.get_edges();

        auto candidate = Graph(n);

        for (int k = threshold; k < m; ++k)
        {
            auto [u, v] = edges[k];

            candidate.insert_edge(u, v);
        }

        while (!candidate.is_connected() && threshold > 0)
        {
            auto [u, v] = edges[threshold - 1];

            candidate.insert_edge(u, v);

            if (!candidate.is_connected())
            {
                --threshold;

                DEBUG_ONLY(std::cout << "Threshold was moved to the left" << std::endl;)
            }
        }

        if (threshold == 0)
        {
            DEBUG_ONLY(std::cout << "There was no work to be done by any thread." << std::endl;)

            return std::tuple<int, std::vector<int>, std::vector<int>>(0, {}, {});
        }

        int threads = std::min(num_threads, threshold); // If there are less edges to the left of [threshold], some threads would be idle.

        int chunk = threshold / threads;

        std::vector<int> start(threads);
        std::vector<int> end(threads);

        for (int i = 0; i < threads; ++i)
        {
            start[i] = (chunk * i);
            end[i] = (i == threads - 1 ? threshold : chunk * (i + 1));
        }

        return std::tuple<int, std::vector<int>, std::vector<int>>(threads, start, end);
    }

    void generate_parallel_internal(const Graph &graph, const std::function<int(const Graph &tree)> callback, const bool *abort, const int lower_bound, const int n, const int m, const std::vector<std::tuple<int, int>> edges, const int start, const int end)
    {
        auto candidate = Graph(n);

        int pointers[n - 1];

        int p = 0;

        int count = 0;

        pointers[p] = start - 1;

        while (pointers[0] < end && !(*abort))
        {
            pointers[p]++;

            if (pointers[p] < m)
            {
                auto u = std::get<0>(edges[pointers[p]]);
                auto v = std::get<1>(edges[pointers[p]]);
                candidate.insert_edge(u, v);

                if (p == n - 2) // Has n - 1 edges.
                {
                    if (!candidate.is_cyclic_disjoint_sets()) // Is acyclic
                    {
                        callback(candidate);
                    }

                    candidate.remove_edge(u, v);
                }
                else
                {
                    p++;
                    pointers[p] = pointers[p - 1];
                }
            }
            else
            {
                p--;
                if (p >= 0)
                {
                    auto u = std::get<0>(edges[pointers[p]]);
                    auto v = std::get<1>(edges[pointers[p]]);
                    candidate.remove_edge(u, v);
                }
            }
        }
    }

    void generate_parallel(const Graph &graph, const std::function<int(const Graph &tree)> &callback, const bool *abort, const int lower_bound, const int num_threads, const std::vector<int> start, const std::vector<int> end)
    {

        if (num_threads < 1)
        {
            return;
        }

        auto n = graph.get_n();
        auto m = graph.get_m();
        auto edges = graph.get_edges();

        DEBUG_ONLY_BLOCK({
            std::cout << "There will be " << num_threads << " worker threads." << std::endl;

            for (int k = 0; k < num_threads; k++)
            {
                std::cout << " " << k << ": " << start[k] << " to " << end[k] << std::endl;
            }
        });

#pragma omp parallel num_threads(num_threads)
        {
            int i = omp_get_thread_num();

            generate_parallel_internal(graph, callback, abort, lower_bound, n, m, edges, start[i], end[i]);
        }
    }
}
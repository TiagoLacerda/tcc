#ifndef FILESYSTEM
#define FILESYSTEM
#include <filesystem>
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef SSTREAM
#define SSTREAM
#include <sstream>
#endif

#ifndef CHRONO
#define CHRONO
#include <chrono>
#endif

#ifndef TUPLE
#define TUPLE
#include <tuple>
#endif

#ifndef RANDOM
#define RANDOM
#include <random>
#endif

#ifndef LOG
#define LOG
#include "src/log.hpp"
#endif

#ifndef GRAPH
#define GRAPH
#include "src/graph/graph.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "src/util/util.hpp"
#endif

void print_pointers(unsigned int *pointers, unsigned int length)
{
    std::cout << "[";

    for (unsigned int i = 0; i < length; i++)
    {
        if (pointers[i] < 10)
        {
            std::cout << " ";
        }

        std::cout << pointers[i];

        if (i < length - 1)
        {
            std::cout << ", ";
        }
    }

    std::cout << "]";
    std::cout << std::endl;
}

std::string edges_to_string(std::vector<std::tuple<unsigned int, unsigned int>> edges)
{
    auto stream = std::stringstream();

    stream << "[";

    for (auto i = edges.begin(); i != edges.end(); i++)
    {
        auto u = std::get<0>(*i);
        auto v = std::get<1>(*i);

        stream << "(";
        stream << u;
        stream << ", ";
        stream << v;
        stream << ")";

        if (*i != edges.back())
        {
            stream << ", ";
        }
    }

    stream << "]";

    return stream.str();
}

/// @brief Find and edge that would keep the graph acyclic.
unsigned int find_edge(Graph graph, std::vector<std::tuple<unsigned int, unsigned int>> edges, unsigned int start)
{
    // std::cout << "find_edge(..., " << edges_to_string(edges) << ", " << start << ")" << std::endl;

    unsigned int m = edges.size();

    for (unsigned int j = start; j < m; j++)
    {
        auto u = std::get<0>(edges[j]);
        auto v = std::get<1>(edges[j]);

        auto copy = Graph(graph);
        copy.insert_edge(u, v);

        if (!copy.is_cyclic_disjoint_sets())
        {
            return j;
        }
    }

    return m;
}

Graph graph_from_pointers(unsigned int n, std::vector<std::tuple<unsigned int, unsigned int>> edges, unsigned int *pointers, unsigned int length)
{
    auto graph = Graph(n);

    for (unsigned int i = 0; i < length; i++)
    {
        auto u = std::get<0>(edges[pointers[i]]);
        auto v = std::get<1>(edges[pointers[i]]);

        graph.insert_edge(u, v);
    }

    return graph;
}

void generate_spanning_trees(Graph graph)
{
    // std::cout << "generate_spanning_trees(...)" << std::endl;

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

    for (unsigned int i = 0; i < n - 1; i++)
    {
        unsigned int j;

        if (i == 0)
        {
            j = find_edge(candidate, edges, 0);
        }
        else
        {
            j = find_edge(candidate, edges, pointers[i - 1] + 1);
        }

        if (j < m)
        {
            pointers[i] = j;

            auto u = std::get<0>(edges[j]);
            auto v = std::get<1>(edges[j]);

            candidate.insert_edge(u, v);
        }
        else
        {
            std::cout << "Unable to determine an initial pointer assignment!" << std::endl;
        }
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
                        std::stringstream stream;
                        stream << "trees/" << index;
                        candidate.to_file(stream.str());
                        index++;
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

int main()
{
    auto t0 = std::chrono::high_resolution_clock::now();

    std::filesystem::create_directory("trees");

    auto graph = Graph(9);

    graph.insert_edge(0, 1);
    graph.insert_edge(0, 4);
    graph.insert_edge(1, 5);
    graph.insert_edge(1, 6);
    graph.insert_edge(2, 3);
    graph.insert_edge(2, 4);
    graph.insert_edge(3, 4);
    graph.insert_edge(3, 7);
    graph.insert_edge(4, 7);
    graph.insert_edge(4, 8);
    graph.insert_edge(5, 7);
    graph.insert_edge(6, 8);
    graph.insert_edge(7, 8);

    graph.to_file("graph.txt");

    generate_spanning_trees(graph);

    auto t1 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

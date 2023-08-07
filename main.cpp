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

void print_array(unsigned int *pointers, unsigned int length)
{
    std::cout << "[";

    for (unsigned int i = 0; i < length; i++)
    {
        std::cout << pointers[i];

        if (i < length - 1)
        {
            std::cout << ", ";
        }
    }

    std::cout << "]";
    std::cout << std::endl;
}

void foo(Graph graph)
{
    auto n = graph.get_n();
    auto m = graph.get_m();

    if (m < n - 1)
    {
        return; // Not enough edges in the graph to construct a spanning tree.
    }

    auto edges = graph.get_edges();

    unsigned int pointers[n - 1];

    for (unsigned int i = 0; i < n - 1; i++)
    {
        pointers[i] = -1; // unsigned int, underflow?
    }

    auto tree = Graph(n);

    unsigned int i = 0;
    unsigned int k = 0;

    while (i < m && k < n - 1)
    {
        auto copy = Graph(tree);

        auto u = std::get<0>(edges[i]);
        auto v = std::get<1>(edges[i]);

        std::cout << "(" << u << ", " << v << ")" << std::endl;

        copy.insert_edge(u, v);

        std::cout << copy.to_json() << std::endl;

        if (!copy.is_cyclic())
        {
            std::cout << "Copy is not cyclic" << std::endl;

            tree = copy;
            pointers[k] = i;
            k++;
        }

        i++;
    }

    print_array(pointers, n - 1);
}

int main()
{
    auto t0 = std::chrono::high_resolution_clock::now();

    auto graph = Graph::complete(3);

    foo(graph);

    auto t1 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef CHRONO
#define CHRONO
#include <chrono>
#endif

#ifndef MUTEX
#define MUTEX
#include <mutex>
#endif

#ifndef IOMANIP
#define IOMANIP
#include <iomanip>
#endif

#ifndef GRAPH
#define GRAPH
#include "src/graph/graph.hpp"
#endif

#ifndef SPANNING_TREE
#define SPANNING_TREE
#include "src/spanning_tree/spanning_tree.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "src/util/util.hpp"
#endif

int main()
{
    auto graph = Graph::complete(8);

    for (auto neighbor : graph.get_neighbors(1))
    {
        if (neighbor != 0 && neighbor != 2)
        {
            graph.remove_edge(1, neighbor);
        }
    }

    for (auto neighbor : graph.get_neighbors(2))
    {
        if (neighbor != 1 && neighbor != 3)
        {
            graph.remove_edge(2, neighbor);
        }
    }

    graph.to_file("graph.txt");

    auto t0 = std::chrono::high_resolution_clock::now();

    auto smallest_e_cycle = graph.get_smallest_e_cycle();

    auto t1 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
    std::cout << "Smallest-e-cycle: " << smallest_e_cycle << std::endl;
}

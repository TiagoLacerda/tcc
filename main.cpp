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
    auto graph = Graph(5);

    // graph.insert_edge(0, 1);
    graph.insert_edge(1, 2);
    graph.insert_edge(2, 3);
    graph.insert_edge(1, 4);
    graph.insert_edge(3, 4);

    graph.to_file("graph.txt");

    auto t0 = std::chrono::high_resolution_clock::now();

    auto n = graph.get_n();

    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            auto length = graph.get_shortest_path_length(u, v);

            if (length == INT_MAX)
            {
                std::cout << std::setw(5) << " ";
            }
            else
            {
                std::cout << std::setw(5) << length;
            }

            if (v < n - 1)
            {
                std::cout << ", ";
            }
        }

        if (u < n - 1)
        {
            std::cout << ", ";
        }

        std::cout << std::endl;
    }

    auto t1 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

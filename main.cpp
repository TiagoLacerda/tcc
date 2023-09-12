#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef CHRONO
#define CHRONO
#include <chrono>
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
    auto t0 = std::chrono::high_resolution_clock::now();

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

    unsigned int factor = graph.get_n() - 1;

    auto callback = [graph, &factor](Graph tree)
    {
        unsigned int candidate = stretch(graph, tree);

        if (candidate < factor)
        {
            factor = candidate;
        }
    };

    spanning_tree::generate(graph, callback);

    std::cout << "Stretch factor: " << factor << std::endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

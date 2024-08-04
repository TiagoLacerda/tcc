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
    auto graph = Graph::complete(9);

    graph.to_file("graph.txt");

    int factor = graph.get_n() - 1;

    std::mutex mutex;

    auto callback = [graph, &factor, &mutex](Graph tree)
    {
        int candidate = stretch(graph, tree);

        mutex.lock();

        if (candidate < factor)
        {
            factor = candidate;
        }

        mutex.unlock();
    };

    // Sequential.

    auto t0 = std::chrono::high_resolution_clock::now();

    spanning_tree::generate(graph, callback);

    auto t1 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Sequential: " << duration.count() << " ms" << std::endl;

    // Parallel.

    t0 = std::chrono::high_resolution_clock::now();

    spanning_tree::generate(graph, callback, 8);

    t1 = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Parallel: " << duration.count() << " ms" << std::endl;
}

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

int main()
{
    auto t0 = std::chrono::high_resolution_clock::now();

    auto graph = Graph::complete(4);
    auto tree = Graph(4);

    tree.insert_edge(0, 1);
    tree.insert_edge(1, 2);
    tree.insert_edge(2, 3);

    auto stretch_index = stretch(graph, tree);

    std::cout << "Stretch index: " << stretch_index << std::endl;

    auto t1 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

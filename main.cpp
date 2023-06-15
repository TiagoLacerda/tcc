#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef CHRONO
#define CHRONO
#include <chrono>
#endif

#ifndef LOG
#define LOG
#include "src/log.hpp"
#endif

#ifndef GRAPH
#define GRAPH
#include "src/graph/graph.hpp"
#endif

int main()
{
    auto t1 = std::chrono::high_resolution_clock::now();

    Graph graph = Graph::complete(10000);

    std::cout << "G is " << (graph.is_cyclic() ? "a cyclic" : "an acyclic") << ", " << (graph.is_connected() ? "connected" : "disconnected") << " graph." << std::endl;

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

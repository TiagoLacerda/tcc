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

    std::ofstream file("data/data.txt");

    for (int n = 1000; n < 10000; n += 1000)
    {
        for (int sample = 0; sample < 10; sample++)
        {
            auto graph = Graph::random_spanning_tree(n);
            auto other = Graph::inverse(graph);

            auto t1 = std::chrono::high_resolution_clock::now();

            auto result1 = other.is_connected_depth_first_search();

            auto t2 = std::chrono::high_resolution_clock::now();

            auto result2 = other.is_connected_disjoint_sets();

            auto t3 = std::chrono::high_resolution_clock::now();

            auto duration_dfs = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            auto duration_djs = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);

            file << n << " " << duration_dfs.count() << " " << duration_djs.count() << std::endl;

            if (result1 != result2)
            {
                std::cout << "Algorithm returns differ!" << std::endl;
            }
        }
    }

    auto t4 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

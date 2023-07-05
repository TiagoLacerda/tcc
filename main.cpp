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

int main()
{
    auto t0 = std::chrono::high_resolution_clock::now();

    std::ofstream file("data/data.txt");

    for (unsigned int n = 100; n <= 1000; n += 10)
    {
        // auto m = (n * (n - 1)) / 2;
        // auto d = m / 50;

        // for (unsigned int k = 0; k <= m - (n - 1); k += d)
        for (unsigned int k = 1; k <= 1; k++)
        {
            for (unsigned int sample = 0; sample < 10; sample++)
            {
                auto graph = Graph::random_spanning_tree(n);

                graph.insert_random_edges(k);

                auto t1 = std::chrono::high_resolution_clock::now();

                bool result1 = graph.is_cyclic_depth_first_search();

                auto t2 = std::chrono::high_resolution_clock::now();

                bool result2 = graph.is_cyclic_disjoint_sets();

                auto t3 = std::chrono::high_resolution_clock::now();

                auto duration_dfs = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
                auto duration_djs = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);

                file << n << " " << k << " " << duration_dfs.count() << " " << duration_djs.count() << std::endl;

                if (result1 != 1 || result2 != 1)
                {
                    std::cout << "One of the algotithms failed to detect a cycle!" << std::endl;
                }
            }
        }
    }

    file.close();

    auto t4 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

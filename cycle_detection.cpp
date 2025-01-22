#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef CHRONO
#define CHRONO
#include <chrono>
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef GRAPH
#define GRAPH
#include "src/graph/graph.hpp"
#endif

int main(int argc, char **argv)
{
    std::vector<int> lengths = {};
    int samples = 10;

    std::chrono::_V2::system_clock::time_point t0, t1;
    int64_t duration_djs, duration_dfs;

    std::ofstream file("cycle_detection.txt");

    for (int i = 10000; i <= 100000; i += 10000)
    {
        lengths.push_back(i);
    }

    for (auto length : lengths)
    {
        for (int sample = 0; sample < samples; sample++)
        {
            auto graph = Graph::random_spanning_tree(length);

            t0 = std::chrono::high_resolution_clock::now();

            graph.is_connected_depth_first_search();

            t1 = std::chrono::high_resolution_clock::now();

            duration_dfs = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

            t0 = std::chrono::high_resolution_clock::now();

            graph.is_cyclic_disjoint_sets();

            t1 = std::chrono::high_resolution_clock::now();

            duration_djs = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

            file << length << " " << sample << " " << duration_dfs << " " << duration_djs << std::endl;
        }
    }

    file.close();
}

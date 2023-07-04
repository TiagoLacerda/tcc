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

#ifndef DISJOINT_SETS
#define DISJOINT_SETS
#include "src/disjoint_sets/disjoint_sets.hpp"
#endif

/// TODO: CREATE A FUNCION TO ADD K EDGES TO THIS TREE TO CREATE CYCLES. COMPARE CYCLE DETECTION ALGORITHMS WITH MANY VARIATIONS OF SPANNING TREES WITH K CYCLES.

bool is_cyclic_disjoint_sets(Graph graph)
{
    auto edges = graph.get_edges();
    auto sets = DisjointSets(graph.get_n());

    for (auto i = edges.begin(); i != edges.end(); i++)
    {
        auto u = std::get<0>(*i);
        auto v = std::get<1>(*i);

        if (!sets.disjoint(u, v))
        {
            return true;
        }

        sets.join(u, v);
    }

    return false;
}

void add_cycle_inducing_edges(Graph *graph, unsigned int k)
{
    auto edges = Graph::inverse(*graph).get_edges();

    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<> dis;

    for (unsigned int i = 0; i < k && !edges.empty(); i++)
    {
        dis = std::uniform_int_distribution<>(0, edges.size() - 1);

        auto edge = *std::next(edges.begin(), dis(rng));

        auto u = std::get<0>(edge);
        auto v = std::get<1>(edge);

        edges.erase(edge);

        (*graph).insert_edge(u, v);
    }
}

int main()
{
    auto t0 = std::chrono::high_resolution_clock::now();

    std::ofstream file("data/data.txt");

    for (unsigned int n = 10; n <= 500; n += 10)
    {
        for (unsigned int k = 1; k <= 1; k++)
        {
            for (unsigned int sample = 0; sample < 20; sample++)
            {
                auto graph = Graph::random_spanning_tree(n);

                add_cycle_inducing_edges(&graph, k);

                auto t1 = std::chrono::high_resolution_clock::now();

                bool result1 = graph.is_cyclic();

                auto t2 = std::chrono::high_resolution_clock::now();

                bool result2 = is_cyclic_disjoint_sets(graph);

                auto t3 = std::chrono::high_resolution_clock::now();

                auto duration_dfs = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
                auto duration_djs = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2);

                file << n << " " << k << " " << duration_dfs.count() << " " << duration_djs.count() << std::endl;
            }
        }
    }

    file.close();

    auto t4 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t0);

    std::cout << "Elapsed: " << duration.count() << " ms" << std::endl;
}

#ifndef LIMITS
#define LIMITS
#include <limits.h>
#endif

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "util.hpp"
#endif

int **floyd_warshall(Graph graph)
{
    int n = graph.get_n();

    int **distances = new int *[n];

    for (int i = 0; i < n; i++)
    {
        distances[i] = new int[n];

        for (int j = 0; j < n; j++)
        {
            distances[i][j] = INT_MAX / 2;
        }

        distances[i][i] = 0;
    }

    auto edges = graph.get_edges();

    for (auto edge = edges.begin(); edge != edges.end(); edge++)
    {
        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);

        distances[u][v] = 1;
        distances[v][u] = 1;
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                auto candidate = distances[i][k] + distances[k][j];

                if (distances[i][j] > candidate)
                {
                    distances[i][j] = candidate;
                    distances[j][i] = candidate;
                }
            }
        }
    }

    return distances;
}

int stretch(Graph graph, Graph tree)
{
    if (tree.get_m() == 0)
    {
        return 0;
    }

    int **distances = floyd_warshall(tree);

    // Iterate through [graph]'s edges to ... TODO: Document

    int stretch = 0;

    auto edges = graph.get_edges();

    for (auto edge = edges.begin(); edge != edges.end(); edge++)
    {
        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);

        if (distances[u][v] > stretch)
        {
            stretch = distances[u][v];
        }
    }

    // Free dynamically allocated memory

    for (auto i = 0; i < tree.get_n(); i++)
    {
        delete[] distances[i];
    }

    delete[] distances;

    //

    return stretch;
}

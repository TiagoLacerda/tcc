#ifndef LIMITS
#define LIMITS
#include <limits.h>
#endif

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.hpp"
#endif

int stretch(Graph graph, Graph tree)
{
    // Apply the Floyd-Warshall algorithm to determine pairwise distances in the tree.
    // https://www.youtube.com/watch?v=4OQeCuLYj-4&ab_channel=MichaelSambol

    if (tree.get_m() == 0)
    {
        return 0;
    }

    int n = tree.get_n();

    int distance[n][n];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            distance[i][j] = INT_MAX / 2;
        }
        distance[i][i] = 0;
    }

    auto edges = tree.get_edges();

    for (auto edge = edges.begin(); edge != edges.end(); edge++)
    {
        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);

        distance[u][v] = 1;
        distance[v][u] = 1;
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                auto candidate = distance[i][k] + distance[k][j];

                if (distance[i][j] > candidate)
                {
                    distance[i][j] = candidate;
                    distance[j][i] = candidate;
                }
            }
        }
    }

    // Iterate through [graph]'s edges to ... TODO: Document

    auto stretch = 0;

    edges = graph.get_edges();

    for (auto edge = edges.begin(); edge != edges.end(); edge++)
    {
        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);

        if (distance[u][v] > stretch)
        {
            stretch = distance[u][v];
        }
    }

    return stretch;
}
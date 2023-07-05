#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef SSTREAM
#define SSTREAM
#include <sstream>
#endif

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef SET
#define SET
#include <set>
#endif

#ifndef TUPLE
#define TUPLE
#include <tuple>
#endif

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef RANDOM
#define RANDOM
#include <random>
#endif

#ifndef GRAPH
#define GRAPH
#include "graph.hpp"
#endif

#ifndef DISJOINT_SETS
#define DISJOINT_SETS
#include "../disjoint_sets/disjoint_sets.hpp"
#endif

#ifndef LOG
#define LOG
#include "../log.hpp"
#endif

Graph::Graph()
{
    this->adjacencies = std::vector<std::vector<unsigned int>>();
    this->n = 0;
    this->m = 0;
}

Graph::Graph(Graph &graph)
{
    this->adjacencies = graph.adjacencies;
    this->n = graph.n;
    this->m = graph.m;
}

Graph::Graph(unsigned int n)
{
    this->adjacencies = std::vector<std::vector<unsigned int>>();
    this->n = n;
    this->m = 0;

    for (unsigned int i = 0; i < n; i++)
    {
        this->adjacencies.push_back(std::vector<unsigned int>());
    }
}

Graph Graph::inverse(Graph &graph)
{
    auto other = Graph::complete(graph.n);

    auto edges = graph.get_edges();

    for (auto i = edges.begin(); i != edges.end(); i++)
    {
        auto u = std::get<0>(*i);
        auto v = std::get<1>(*i);

        other.remove_edge(u, v);
    }

    return other;
}

Graph::Graph(std::vector<std::vector<unsigned int>> adjacencies, unsigned int n, unsigned int m)
{
    this->adjacencies = adjacencies;
    this->n = n;
    this->m = m;
}

Graph Graph::complete(unsigned int n)
{
    std::vector<std::vector<unsigned int>> adjacencies;

    for (unsigned int i = 0; i < n; i++)
    {
        adjacencies.push_back(std::vector<unsigned int>());
        for (unsigned int j = 0; j < n; j++)
        {
            if (i != j)
            {
                adjacencies[i].push_back(j);
            }
        }
    }

    return Graph(adjacencies, n, (n * (n - 1)) / 2);
}

Graph Graph::random_spanning_tree(unsigned int n)
{
    auto graph = Graph(n);

    if (n < 2)
    {
        return graph;
    }

    std::vector<unsigned int> visited;
    std::vector<unsigned int> unvisited(n);
    std::iota(unvisited.begin(), unvisited.end(), 0);

    auto u = unvisited[0];
    auto v = unvisited[1];

    graph.insert_edge(u, v);

    unvisited.erase(unvisited.begin());
    unvisited.erase(unvisited.begin());

    visited.push_back(u);
    visited.push_back(v);

    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<> dis;

    while (!unvisited.empty())
    {
        dis = std::uniform_int_distribution<>(0, visited.size() - 1);
        auto i = dis(rng);

        dis = std::uniform_int_distribution<>(0, unvisited.size() - 1);
        auto j = dis(rng);

        auto u = visited[i];
        auto v = unvisited[j];

        graph.insert_edge(u, v);

        visited.push_back(v);
        unvisited.erase(unvisited.begin() + j);
    }

    return graph;
};

void Graph::insert_node()
{
    this->adjacencies.push_back(std::vector<unsigned int>());
    this->n++;
}

void Graph::insert_edge(unsigned int u, unsigned int v)
{
    if (u == v)
    {
        return; // u and v are the same.
    }

    if (u >= this->n || v >= this->n)
    {
        return; // u or v are out of bounds.
    }

    for (std::size_t i = 0; i < this->adjacencies[u].size(); i++)
    {
        if (this->adjacencies[u][i] == v)
        {
            return; // u and v are already neighbors.
        }
    }

    this->adjacencies[u].push_back(v);
    this->adjacencies[v].push_back(u);
    this->m++;
}

void Graph::remove_node(unsigned int u)
{
    if (u >= this->n)
    {
        return; // u is out of bounds.
    }

    // Remove edges from u to neighborhood.
    while (!this->adjacencies[u].empty())
    {
        this->remove_edge(u, this->adjacencies[u][0]);
    }

    // Remove u.
    this->adjacencies.erase(this->adjacencies.begin() + u);
    this->n--;

    // Decrease indexes higher than u.
    for (size_t i = 0; i < this->adjacencies.size(); i++)
    {
        for (size_t j = 0; j < this->adjacencies.size(); j++)
        {
            if (this->adjacencies[i][j] > u)
            {
                this->adjacencies[i][j]--;
            }
        }
    }
}

void Graph::remove_edge(unsigned int u, unsigned int v)
{
    if (u == v)
    {
        return; // u and v are the same.
    }

    if (u >= this->n || v >= this->n)
    {
        return; // u or v are out of bounds.
    }

    bool removed = false;

    // Remove adjacency from u to v.
    for (std::size_t i = 0; i < this->adjacencies[u].size(); i++)
    {
        if (this->adjacencies[u][i] == v)
        {
            this->adjacencies[u].erase(this->adjacencies[u].begin() + i);
            removed = true;
        }
    }

    // Remove adjacency from v to u.
    for (std::size_t i = 0; i < this->adjacencies[v].size(); i++)
    {
        if (this->adjacencies[v][i] == u)
        {
            this->adjacencies[v].erase(this->adjacencies[v].begin() + i);
            removed = true;
        }
    }

    // If the edge did exist, decrease m.
    if (removed)
    {
        this->m--;
    }
}

unsigned int Graph::get_n()
{
    return this->n;
}

unsigned int Graph::get_m()
{
    return this->m;
}

unsigned int Graph::get_minimum_degree()
{
    if (this->adjacencies.empty())
    {
        return 0; // Null graph.
    }

    auto minimum = this->adjacencies[0].size();

    for (size_t i = 0; i < this->adjacencies.size(); i++)
    {
        auto degree = this->adjacencies[i].size();
        if (degree < minimum)
        {
            minimum = degree;
        }
    }

    return minimum;
}

unsigned int Graph::get_maximum_degree()
{
    if (this->adjacencies.empty())
    {
        return 0; // Null graph.
    }

    auto maximum = this->adjacencies[0].size();

    for (size_t i = 0; i < this->adjacencies.size(); i++)
    {
        auto degree = this->adjacencies[i].size();
        if (degree > maximum)
        {
            maximum = degree;
        }
    }

    return maximum;
}

bool Graph::has_edge(unsigned int u, unsigned int v)
{
    if (u == v)
    {
        return false; // u and v are the same.
    }

    if (u >= this->n || v >= this->n)
    {
        return false; // u or v are out of bounds.
    }

    for (auto i = this->adjacencies[u].begin(); i < this->adjacencies[u].end(); i++)
    {
        // v found in u's adjacency list.
        if (v == *i)
        {
            return true;
        }
    }

    return false;
}

std::vector<std::tuple<unsigned int, unsigned int>> Graph::get_edges()
{
    auto edges = std::vector<std::tuple<unsigned int, unsigned int>>();

    for (unsigned int u = 0; u < this->n; u++)
    {
        for (size_t i = 0; i < this->adjacencies[u].size(); i++)
        {
            auto v = this->adjacencies[u][i];
            // This ensures each edge is inserted only once
            if (u < v)
            {
                edges.push_back(std::tuple(u, v));
            }
        }
    }

    return edges;
}

bool Graph::is_cyclic()
{
    return this->is_cyclic_depth_first_search();
}

bool Graph::is_cyclic_depth_first_search()
{
    auto visited = std::set<unsigned int>();

    for (unsigned int i = 0; i < this->n; i++)
    {
        if (visited.find(i) != visited.end())
        {
            continue; // Do not search a visited node again.
        }

        if (is_cyclic_depth_first_search_internal(i, -1, &visited))
        {
            return true;
        }
    }

    return false;
}

bool Graph::is_cyclic_depth_first_search_internal(unsigned int node, unsigned int parent, std::set<unsigned int> *visited)
{
    visited->insert(node);

    for (auto neighbor : this->adjacencies[node])
    {
        if (neighbor == parent)
        {
            continue; // Do not traverse back in the tree.
        }

        if (visited->find(neighbor) != visited->end())
        {
            return true; // Found a visited node.
        }

        if (is_cyclic_depth_first_search_internal(neighbor, node, visited))
        {
            return true;
        }
    }

    return false;
}

bool Graph::is_cyclic_disjoint_sets()
{
    auto edges = this->get_edges();
    auto sets = DisjointSets(this->get_n());

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

bool Graph::is_connected()
{
    if (this->n == 0)
    {
        return true;
    }

    auto visited = std::set<unsigned int>();

    is_connected_internal(0, &visited);

    return visited.size() == this->adjacencies.size();
}

void Graph::is_connected_internal(unsigned int node, std::set<unsigned int> *visited)
{
    visited->insert(node);

    for (auto neighbor : this->adjacencies[node])
    {
        if (visited->find(neighbor) == visited->end())
        {
            is_connected_internal(neighbor, visited);
        }
    }
}

void Graph::insert_random_edges(unsigned int k)
{
    auto edges = Graph::inverse(*this).get_edges();

    static std::random_device rd;
    static std::mt19937 rng(rd());
    std::uniform_int_distribution<> dis;

    for (unsigned int i = 0; i < k && !edges.empty(); i++)
    {
        dis = std::uniform_int_distribution<>(0, edges.size() - 1);

        auto edge = std::next(edges.begin(), dis(rng));

        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);

        edges.erase(edge);

        this->insert_edge(u, v);
    }
}

std::string Graph::to_json()
{
    std::stringstream stream;

    stream << "{";

    // ADJACENCIES
    stream << "\"adjacencies\":[";
    for (std::size_t i = 0; i < this->adjacencies.size(); i++)
    {
        stream << "[";
        for (std::size_t j = 0; j < this->adjacencies[i].size(); j++)
        {
            stream << this->adjacencies[i][j];

            if (j < this->adjacencies[i].size() - 1)
            {
                stream << ",";
            }
        }
        stream << "]";

        if (i < this->adjacencies.size() - 1)
        {
            stream << ",";
        }
    }
    stream << "]";
    stream << ",";
    // M
    stream << "\"n\":";
    stream << this->n;
    stream << ",";

    // M
    stream << "\"m\":";
    stream << this->m;

    stream << "}";

    return stream.str();
};

void Graph::to_file(std::string path)
{
    std::ofstream file(path);

    for (unsigned int i = 0; i < this->n; i++)
    {
        file << i << std::endl;
    }

    auto edges = this->get_edges();

    for (auto i = edges.begin(); i != edges.end(); i++)
    {
        auto u = std::get<0>(*i);
        auto v = std::get<1>(*i);

        file << u << " " << v << std::endl;
    }

    file.close();
};
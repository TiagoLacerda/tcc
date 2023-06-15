
#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

#ifndef GRAPH
#define GRAPH
#include "graph.hpp"
#endif

#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef SET
#define SET
#include <set>
#endif

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef STRING
#define STRING
#include <string>
#endif

#ifndef SSTREAM
#define SSTREAM
#include <sstream>
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
};

Graph::Graph(Graph &graph)
{
    this->adjacencies = graph.adjacencies;
    this->n = graph.n;
    this->m = graph.m;
};

Graph::Graph(std::vector<std::vector<unsigned int>> adjacencies, unsigned int n, unsigned int m)
{
    this->adjacencies = adjacencies;
    this->n = n;
    this->m = m;
};

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
};

void Graph::insert_node()
{
    this->adjacencies.push_back(std::vector<unsigned int>());
    this->n++;
};

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
};

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
};

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
};

int Graph::get_n()
{
    return this->n;
};

int Graph::get_m()
{
    return this->m;
};

int Graph::get_minimum_degree()
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
};

int Graph::get_maximum_degree()
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
};

bool Graph::is_cyclic()
{
    auto visited = std::set<unsigned int>();

    for (unsigned int i = 0; i < this->n; i++)
    {
        if (visited.find(i) != visited.end())
        {
            continue; // Do not search a visited node again.
        }

        if (is_cyclic_internal(i, -1, &visited))
        {
            return true;
        }
    }

    return false;
}

bool Graph::is_cyclic_internal(unsigned int node, unsigned int parent, std::set<unsigned int> *visited)
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

        if (is_cyclic_internal(neighbor, node, visited))
        {
            return true;
        }
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
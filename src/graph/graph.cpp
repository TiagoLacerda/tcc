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

#ifndef STACK
#define STACK
#include <stack>
#endif

#ifndef QUEUE
#define QUEUE
#include <queue>
#endif

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef NUMERIC
#define NUMERIC
#include <numeric>
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

#ifndef CLIMITS
#define CLIMITS
#include <climits>
#endif

static std::random_device rd;
static std::mt19937 rng(rd());

Graph::Graph()
{
    this->adjacencies = std::vector<std::vector<int>>();
    this->n = 0;
    this->m = 0;
}

Graph::Graph(const Graph &graph)
{
    this->adjacencies = graph.adjacencies;
    this->n = graph.n;
    this->m = graph.m;
}

Graph::Graph(int n)
{
    this->adjacencies = std::vector<std::vector<int>>();
    this->n = n;
    this->m = 0;

    for (int i = 0; i < n; i++)
    {
        this->adjacencies.push_back(std::vector<int>());
    }
}

Graph Graph::inverse(const Graph &graph)
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

Graph::Graph(std::vector<std::vector<int>> adjacencies, int n, int m)
{
    this->adjacencies = adjacencies;
    this->n = n;
    this->m = m;
}

Graph Graph::complete(int n)
{
    std::vector<std::vector<int>> adjacencies;

    for (int i = 0; i < n; i++)
    {
        adjacencies.push_back(std::vector<int>());
        for (int j = 0; j < n; j++)
        {
            if (i != j)
            {
                adjacencies[i].push_back(j);
            }
        }
    }

    return Graph(adjacencies, n, (n * (n - 1)) / 2);
}

Graph Graph::cycle(int n)
{
    if (n < 3)
    {
        throw std::invalid_argument("At least 3 nodes are required for a simple cycle graph.");
    }

    std::vector<std::vector<int>> adjacencies;

    for (int i = 0; i < n; i++)
    {
        adjacencies.push_back(std::vector<int>());
        adjacencies[i].push_back(((i + n) - 1) % n);
        adjacencies[i].push_back(((i + n) + 1) % n);
    }

    return Graph(adjacencies, n, n);
}

Graph Graph::random_spanning_tree(int n)
{
    auto graph = Graph(n);

    if (n < 2)
    {
        return graph;
    }

    std::vector<int> visited;
    std::vector<int> unvisited(n);
    std::iota(unvisited.begin(), unvisited.end(), 0);

    auto u = unvisited[0];
    auto v = unvisited[1];

    graph.insert_edge(u, v);

    unvisited.erase(unvisited.begin());
    unvisited.erase(unvisited.begin());

    visited.push_back(u);
    visited.push_back(v);

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
    this->adjacencies.push_back(std::vector<int>());
    this->n++;
}

void Graph::insert_edge(int u, int v)
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

void Graph::remove_node(int u)
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

void Graph::remove_edge(int u, int v)
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

int Graph::get_n() const
{
    return this->n;
}

int Graph::get_m() const
{
    return this->m;
}

int Graph::get_degree(int u) const
{

    return static_cast<int>(adjacencies[u].size());
}

std::vector<int> Graph::get_neighbors(int u) const
{
    return adjacencies[u];
}

int Graph::get_minimum_degree() const
{
    if (this->adjacencies.empty())
    {
        return 0; // Null graph.
    }

    auto minimum = this->adjacencies[0].size();

    for (auto neighborhood : adjacencies)
    {
        auto degree = neighborhood.size();

        if (degree < minimum)
        {
            minimum = degree;
        }
    }

    return minimum;
}

int Graph::get_maximum_degree() const
{
    if (this->adjacencies.empty())
    {
        return 0; // Null graph.
    }

    auto maximum = this->adjacencies[0].size();

    for (auto neighborhood : adjacencies)
    {
        auto degree = neighborhood.size();

        if (degree > maximum)
        {
            maximum = degree;
        }
    }

    return maximum;
}

int Graph::get_girth() const
{
    int girth = INT_MAX;
    int parent[n];
    int visited[n];
    int distance[n];
    std::queue<int> queue;

    for (int i = 0; i < n; i++)
    {
        parent[i] = -1;
        visited[i] = -1;
        distance[i] = INT_MAX;
    }

    for (int root = 0; root < n; root++)
    {
        queue.push(root);

        parent[root] = -1;
        distance[root] = 0;

        while (!queue.empty())
        {
            int current = queue.front();

            queue.pop();
            visited[current] = root;

            for (int neighbor : adjacencies[current])
            {
                if (neighbor != parent[current])
                {
                    if (visited[neighbor] == root)
                    {
                        int length = distance[current] + distance[neighbor] + 1;

                        if (length < girth)
                        {
                            girth = length;
                        }
                    }
                    else
                    {
                        parent[neighbor] = current;
                        distance[neighbor] = distance[current] + 1;
                        queue.push(neighbor);
                    }
                }
            }
        }
    }

    return girth;
}

int Graph::get_smallest_e_cycle() const
{
    auto edges = get_edges();

    auto clone = Graph(*this);

    auto highest = -1;

    for (auto edge : edges)
    {
        auto u = std::get<0>(edge);
        auto v = std::get<1>(edge);

        clone.remove_edge(u, v);

        auto length = clone.get_shortest_path_length(u, v);

        clone.insert_edge(u, v);

        if (length + 1 > highest)
        {
            highest = length + 1;
        }
    }

    return highest;
}

int Graph::get_shortest_path_length(int u, int v) const
{
    if (u == v)
    {
        return 0;
    }

    bool visited[n];
    int distance[n];
    std::queue<int> queue;

    for (int i = 0; i < n; i++)
    {
        visited[i] = false;
        distance[i] = -1;
    }

    queue.push(u);

    visited[u] = true;
    distance[u] = 0;

    while (!queue.empty())
    {
        int current = queue.front();

        queue.pop();

        for (int neighbor : adjacencies[current])
        {
            if (!visited[neighbor])
            {

                distance[neighbor] = distance[current] + 1;
                visited[neighbor] = true;
                queue.push(neighbor);

                if (neighbor == v)
                {
                    return distance[neighbor];
                }
            }
        }
    }

    return distance[v];
}

bool Graph::has_edge(int u, int v) const
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

std::vector<std::tuple<int, int>> Graph::get_edges() const
{
    auto edges = std::vector<std::tuple<int, int>>();

    edges.reserve(m);

    for (int u = 0; u < this->n; u++)
    {
        for (size_t i = 0; i < this->adjacencies[u].size(); i++)
        {
            auto v = this->adjacencies[u][i];
            // This ensures each edge is inserted only once
            if (u < v)
            {
                edges.emplace_back(std::tuple(u, v));
            }
        }
    }

    return edges;
}

bool Graph::is_cyclic() const
{
    return this->is_cyclic_depth_first_search();
}

bool Graph::is_cyclic_depth_first_search() const
{
    auto visited = std::set<int>();

    for (int i = 0; i < this->n; i++)
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

bool Graph::is_cyclic_depth_first_search_internal(int node, int parent, std::set<int> *visited) const
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

bool Graph::is_cyclic_disjoint_sets() const
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

bool Graph::is_connected() const
{
    return this->is_connected_depth_first_search();
}

bool Graph::is_connected_depth_first_search() const
{
    if (this->n == 0)
    {
        return true;
    }

    auto visited = std::set<int>();

    is_connected_depth_first_search_internal(0, &visited);

    return visited.size() == this->adjacencies.size();
}

void Graph::is_connected_depth_first_search_internal(int node, std::set<int> *visited) const
{
    visited->insert(node);

    for (auto neighbor : this->adjacencies[node])
    {
        if (visited->find(neighbor) == visited->end())
        {
            is_connected_depth_first_search_internal(neighbor, visited);
        }
    }
}

bool Graph::is_connected_disjoint_sets() const
{
    auto edges = this->get_edges();
    auto sets = DisjointSets(this->get_n());

    for (auto i = edges.begin(); i != edges.end(); i++)
    {
        auto u = std::get<0>(*i);
        auto v = std::get<1>(*i);
        sets.join(u, v);
    }

    std::set<int> representatives;

    for (int i = 0; i < this->n; i++)
    {
        auto representative = sets.find(i);
        representatives.insert(representative);
    }

    return representatives.size() < 2;
}

void Graph::insert_random_edges(int k)
{
    if (k < 1)
    {
        return;
    }

    auto edges = Graph::inverse(*this).get_edges();

    std::shuffle(edges.begin(), edges.end(), rng);

    for (int i = 0; i < std::min(k, static_cast<int>(edges.size())); i++)
    {
        auto u = std::get<0>(edges[i]);
        auto v = std::get<1>(edges[i]);

        this->insert_edge(u, v);
    }
}

std::string Graph::to_json() const
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

void Graph::to_file(const std::string &path) const
{
    std::ofstream file(path);

    if (!file)
    {
        throw std::invalid_argument("Failed to open file");
    }

    for (int i = 0; i < this->n; i++)
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

Graph Graph::load(const std::string &path)
{
    std::ifstream file(path);

    if (!file)
    {
        throw std::invalid_argument("Failed to open file");
    }

    int n, u, v, k;

    file >> n;

    auto graph = Graph(n);

    for (u = 0; u < n; u++)
    {
        for (v = 0; v < n; v++)
        {

            file >> k;

            if (k == 1)
            {
                graph.insert_edge(u, v);
            }
        }
    }

    file.close();

    return graph;
}
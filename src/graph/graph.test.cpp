#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#ifndef DOCTEST
#define DOCTEST
#include "../../include/doctest.h"
#endif

#ifndef SET
#define SET
#include <set>
#endif

#ifndef TUPLE
#define TUPLE
#include <tuple>
#endif

#ifndef GRAPH
#define GRAPH
#include "graph.hpp"
#endif

TEST_CASE("Graph::Graph()")
{
    Graph graph = Graph();

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);
}

TEST_CASE("Graph::Graph(Graph &graph)")
{
    Graph graph = Graph();
    Graph clone = Graph(graph);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);

    REQUIRE(clone.get_n() == 0);
    REQUIRE(clone.get_m() == 0);
    REQUIRE(clone.get_minimum_degree() == 0);
    REQUIRE(clone.get_maximum_degree() == 0);

    clone.insert_node();
    clone.insert_node();
    clone.insert_edge(0, 1);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);

    REQUIRE(clone.get_n() == 2);
    REQUIRE(clone.get_m() == 1);
    REQUIRE(clone.get_minimum_degree() == 1);
    REQUIRE(clone.get_maximum_degree() == 1);

    clone.remove_node(0);
    clone.remove_node(0);

    graph.insert_node();
    graph.insert_node();
    graph.insert_edge(0, 1);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);
    REQUIRE(graph.get_minimum_degree() == 1);
    REQUIRE(graph.get_maximum_degree() == 1);

    REQUIRE(clone.get_n() == 0);
    REQUIRE(clone.get_m() == 0);
    REQUIRE(clone.get_minimum_degree() == 0);
    REQUIRE(clone.get_maximum_degree() == 0);
}

TEST_CASE("Graph::Graph(int n)")
{
    Graph graph;

    graph = Graph(0);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph = Graph(1);

    REQUIRE(graph.get_n() == 1);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph = Graph(2);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == false);

    graph.insert_edge(0, 1);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);
    REQUIRE(graph.get_minimum_degree() == 1);
    REQUIRE(graph.get_maximum_degree() == 1);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);
}

TEST_CASE("Graph Graph::inverse(Graph &graph)")
{
    Graph graph = Graph::complete(5);

    Graph other = Graph::inverse(graph);

    REQUIRE(graph.get_n() == 5);
    REQUIRE(graph.get_m() == 10);
    REQUIRE(graph.get_minimum_degree() == 4);
    REQUIRE(graph.get_maximum_degree() == 4);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);

    auto edges = graph.get_edges();

    for (auto edge = edges.begin(); edge != edges.end(); edge++)
    {
        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);
        REQUIRE(other.has_edge(u, v) == false);
    }

    REQUIRE(other.get_n() == 5);
    REQUIRE(other.get_m() == 0);
    REQUIRE(other.get_minimum_degree() == 0);
    REQUIRE(other.get_maximum_degree() == 0);
    REQUIRE(other.is_cyclic() == false);
    REQUIRE(other.is_connected() == false);

    edges = other.get_edges();

    for (auto edge = edges.begin(); edge != edges.end(); edge++)
    {
        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);
        REQUIRE(graph.has_edge(u, v) == false);
    }
}

TEST_CASE("Graph Graph::complete(int n)")
{
    Graph graph;

    graph = Graph::complete(0);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph = Graph::complete(1);

    REQUIRE(graph.get_n() == 1);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph = Graph::complete(2);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);
    REQUIRE(graph.get_minimum_degree() == 1);
    REQUIRE(graph.get_maximum_degree() == 1);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph = Graph::complete(3);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 3);
    REQUIRE(graph.get_minimum_degree() == 2);
    REQUIRE(graph.get_maximum_degree() == 2);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);

    graph = Graph::complete(4);

    REQUIRE(graph.get_n() == 4);
    REQUIRE(graph.get_m() == 6);
    REQUIRE(graph.get_minimum_degree() == 3);
    REQUIRE(graph.get_maximum_degree() == 3);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);

    graph = Graph::complete(5);

    REQUIRE(graph.get_n() == 5);
    REQUIRE(graph.get_m() == 10);
    REQUIRE(graph.get_minimum_degree() == 4);
    REQUIRE(graph.get_maximum_degree() == 4);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);
}

TEST_CASE("void Graph::insert_node()")
{
    Graph graph = Graph();

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph.insert_node();

    REQUIRE(graph.get_n() == 1);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph.insert_node();

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == false);
}

TEST_CASE("void Graph::insert_edge(int u, int v)")
{

    Graph graph = Graph();

    graph.insert_node();
    graph.insert_node();

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == false);

    graph.insert_edge(0, 0);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == false);

    graph.insert_edge(0, 2);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == false);

    graph.insert_edge(0, 1);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);
    REQUIRE(graph.get_minimum_degree() == 1);
    REQUIRE(graph.get_maximum_degree() == 1);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);
}

TEST_CASE("void Graph::remove_node(int u)")
{
    Graph graph = Graph::complete(5);

    REQUIRE(graph.get_n() == 5);
    REQUIRE(graph.get_m() == 10);
    REQUIRE(graph.get_minimum_degree() == 4);
    REQUIRE(graph.get_maximum_degree() == 4);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 4);
    REQUIRE(graph.get_m() == 6);
    REQUIRE(graph.get_minimum_degree() == 3);
    REQUIRE(graph.get_maximum_degree() == 3);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 3);
    REQUIRE(graph.get_minimum_degree() == 2);
    REQUIRE(graph.get_maximum_degree() == 2);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);
    REQUIRE(graph.get_minimum_degree() == 1);
    REQUIRE(graph.get_maximum_degree() == 1);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 1);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);
}

TEST_CASE("void Graph::remove_edge(int u, int v)")
{

    Graph graph = Graph::complete(3);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 3);
    REQUIRE(graph.get_minimum_degree() == 2);
    REQUIRE(graph.get_maximum_degree() == 2);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);

    graph.remove_edge(0, 0);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 3);
    REQUIRE(graph.get_minimum_degree() == 2);
    REQUIRE(graph.get_maximum_degree() == 2);
    REQUIRE(graph.is_cyclic() == true);
    REQUIRE(graph.is_connected() == true);

    graph.remove_edge(0, 1);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 2);
    REQUIRE(graph.get_minimum_degree() == 1);
    REQUIRE(graph.get_maximum_degree() == 2);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == true);

    graph.remove_edge(0, 2);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 1);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 1);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == false);

    graph.remove_edge(1, 2);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == false);

    graph.remove_edge(1, 2);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(graph.get_minimum_degree() == 0);
    REQUIRE(graph.get_maximum_degree() == 0);
    REQUIRE(graph.is_cyclic() == false);
    REQUIRE(graph.is_connected() == false);
}

TEST_CASE("int Graph::get_minimum_degree()")
{
    Graph graph = Graph();

    REQUIRE(graph.get_minimum_degree() == 0);

    graph.insert_node();
    graph.insert_node();
    graph.insert_edge(0, 1);

    REQUIRE(graph.get_minimum_degree() == 1);

    graph.insert_node();
    graph.insert_edge(0, 2);

    REQUIRE(graph.get_minimum_degree() == 1);
}

TEST_CASE("int Graph::get_maximum_degree()")
{

    Graph graph = Graph();

    REQUIRE(graph.get_maximum_degree() == 0);

    graph.insert_node();
    graph.insert_node();
    graph.insert_edge(0, 1);

    REQUIRE(graph.get_maximum_degree() == 1);

    graph.insert_node();
    graph.insert_edge(0, 2);

    REQUIRE(graph.get_maximum_degree() == 2);
}

TEST_CASE("int Graph::get_girth()")
{

    Graph graph = Graph();

    REQUIRE(graph.get_girth() == INT_MAX);

    graph = Graph::complete(3);

    REQUIRE(graph.get_girth() == 3);

    graph = Graph::cycle(4);

    REQUIRE(graph.get_girth() == 4);

    graph = Graph::cycle(5);

    REQUIRE(graph.get_girth() == 5);

    graph.insert_edge(0, 2);

    REQUIRE(graph.get_girth() == 3);

    graph.remove_edge(0, 2);
    graph.remove_edge(0, 1);

    REQUIRE(graph.get_girth() == INT_MAX);
}

TEST_CASE("int Graph::get_smallest_e_cycle()")
{
    Graph graph = Graph(1);

    REQUIRE(graph.get_smallest_e_cycle() == -1);

    graph = Graph::cycle(3);

    REQUIRE(graph.get_smallest_e_cycle() == 3);

    graph = Graph::cycle(5);

    REQUIRE(graph.get_smallest_e_cycle() == 5);

    graph = Graph::cycle(5);

    graph.insert_edge(0, 2);

    REQUIRE(graph.get_smallest_e_cycle() == 4);
}

TEST_CASE("int Graph::get_shortest_path_length(int u, int v)")
{

    Graph graph = Graph(5);

    graph.insert_edge(1, 2);
    graph.insert_edge(2, 3);
    graph.insert_edge(3, 4);
    graph.insert_edge(4, 1);

    REQUIRE(graph.get_shortest_path_length(0, 0) == 0);
    REQUIRE(graph.get_shortest_path_length(0, 1) == -1);
    REQUIRE(graph.get_shortest_path_length(1, 1) == 0);
    REQUIRE(graph.get_shortest_path_length(1, 2) == 1);
    REQUIRE(graph.get_shortest_path_length(1, 3) == 2);
    REQUIRE(graph.get_shortest_path_length(1, 4) == 1);
}

TEST_CASE("int Graph::has_edge(int u, int v)")
{

    Graph graph = Graph(2);

    REQUIRE(graph.has_edge(0, 0) == false);

    graph.insert_edge(0, 1);

    REQUIRE(graph.has_edge(0, 1) == true);
}

TEST_CASE("bool Graph::is_cyclic_depth_first_search()")
{

    Graph graph;

    graph = Graph::complete(0);

    REQUIRE(graph.is_cyclic_depth_first_search() == false);

    graph = Graph::complete(1);

    REQUIRE(graph.is_cyclic_depth_first_search() == false);

    graph = Graph::complete(2);

    REQUIRE(graph.is_cyclic_depth_first_search() == false);

    graph = Graph::complete(3);

    REQUIRE(graph.is_cyclic_depth_first_search() == true);

    graph = Graph::complete(4);

    REQUIRE(graph.is_cyclic_depth_first_search() == true);

    graph = Graph::complete(5);

    REQUIRE(graph.is_cyclic_depth_first_search() == true);

    graph = Graph();

    graph.insert_node();
    graph.insert_node();
    graph.insert_node();
    graph.insert_node();

    graph.insert_edge(0, 1);
    graph.insert_edge(1, 2);
    graph.insert_edge(2, 3);

    REQUIRE(graph.is_cyclic_depth_first_search() == false);
}

TEST_CASE("bool Graph::is_cyclic_disjoint_sets()")
{

    Graph graph;

    graph = Graph::complete(0);

    REQUIRE(graph.is_cyclic_disjoint_sets() == false);

    graph = Graph::complete(1);

    REQUIRE(graph.is_cyclic_disjoint_sets() == false);

    graph = Graph::complete(2);

    REQUIRE(graph.is_cyclic_disjoint_sets() == false);

    graph = Graph::complete(3);

    REQUIRE(graph.is_cyclic_disjoint_sets() == true);

    graph = Graph::complete(4);

    REQUIRE(graph.is_cyclic_disjoint_sets() == true);

    graph = Graph::complete(5);

    REQUIRE(graph.is_cyclic_disjoint_sets() == true);

    graph = Graph();

    graph.insert_node();
    graph.insert_node();
    graph.insert_node();
    graph.insert_node();

    graph.insert_edge(0, 1);
    graph.insert_edge(1, 2);
    graph.insert_edge(2, 3);

    REQUIRE(graph.is_cyclic_disjoint_sets() == false);
}

TEST_CASE("bool Graph::is_connected_depth_first_search()")
{
    Graph graph;

    graph = Graph();

    REQUIRE(graph.is_connected_depth_first_search() == true);

    graph.insert_node();

    REQUIRE(graph.is_connected_depth_first_search() == true);

    graph.insert_node();

    REQUIRE(graph.is_connected_depth_first_search() == false);

    graph.insert_edge(0, 1);

    REQUIRE(graph.is_connected_depth_first_search() == true);

    graph = Graph::complete(3);

    REQUIRE(graph.is_connected_depth_first_search() == true);

    graph = Graph();

    graph.insert_node();
    graph.insert_node();
    graph.insert_node();
    graph.insert_node();

    graph.insert_edge(0, 1);
    graph.insert_edge(1, 2);
    graph.insert_edge(2, 3);

    REQUIRE(graph.is_connected_depth_first_search() == true);
}

TEST_CASE("bool Graph::is_connected_disjoint_sets()")
{
    Graph graph;

    graph = Graph();

    REQUIRE(graph.is_connected_disjoint_sets() == true);

    graph.insert_node();

    REQUIRE(graph.is_connected_disjoint_sets() == true);

    graph.insert_node();

    REQUIRE(graph.is_connected_disjoint_sets() == false);

    graph.insert_edge(0, 1);

    REQUIRE(graph.is_connected_disjoint_sets() == true);

    graph = Graph::complete(3);

    REQUIRE(graph.is_connected_disjoint_sets() == true);

    graph = Graph();

    graph.insert_node();
    graph.insert_node();
    graph.insert_node();
    graph.insert_node();

    graph.insert_edge(0, 1);
    graph.insert_edge(1, 2);
    graph.insert_edge(2, 3);

    REQUIRE(graph.is_connected_disjoint_sets() == true);
}

TEST_CASE("bool Graph::insert_random_edges(int k)")
{
    Graph graph;

    graph = Graph();

    graph.insert_random_edges(1);

    REQUIRE(graph.get_m() == 0);

    graph = Graph(2);

    graph.insert_random_edges(1);

    REQUIRE(graph.get_m() == 1);

    graph = Graph(3);

    graph.insert_random_edges(4);

    REQUIRE(graph.get_m() == 3);
}

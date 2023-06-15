#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#ifndef DOCTEST
#define DOCTEST
#include "../../include/doctest.h"
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
    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
}

TEST_CASE("Graph::Graph(Graph &graph)")
{
    Graph graph = Graph();
    Graph clone = Graph(graph);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(clone.get_n() == 0);
    REQUIRE(clone.get_m() == 0);

    clone.insert_node();
    clone.insert_node();
    clone.insert_edge(0, 1);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
    REQUIRE(clone.get_n() == 2);
    REQUIRE(clone.get_m() == 1);

    clone.remove_node(0);
    clone.remove_node(0);

    graph.insert_node();
    graph.insert_node();
    graph.insert_edge(0, 1);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);
    REQUIRE(clone.get_n() == 0);
    REQUIRE(clone.get_m() == 0);
}

TEST_CASE("Graph Graph::complete(unsigned int n)")
{
    Graph graph;

    graph = Graph::complete(0);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);

    graph = Graph::complete(1);

    REQUIRE(graph.get_n() == 1);
    REQUIRE(graph.get_m() == 0);

    graph = Graph::complete(2);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);

    graph = Graph::complete(3);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 3);

    graph = Graph::complete(4);

    REQUIRE(graph.get_n() == 4);
    REQUIRE(graph.get_m() == 6);

    graph = Graph::complete(5);

    REQUIRE(graph.get_n() == 5);
    REQUIRE(graph.get_m() == 10);
}

TEST_CASE("void Graph::insert_node()")
{
    Graph graph = Graph();

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);

    graph.insert_node();

    REQUIRE(graph.get_n() == 1);
    REQUIRE(graph.get_m() == 0);

    graph.insert_node();

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);
}

TEST_CASE("void Graph::insert_edge(unsigned int u, unsigned int v)")
{

    Graph graph = Graph();

    graph.insert_node();
    graph.insert_node();

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);

    graph.insert_edge(0, 0);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);

    graph.insert_edge(0, 2);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 0);

    graph.insert_edge(0, 1);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);
}

TEST_CASE("void Graph::remove_node(unsigned int u)")
{
    Graph graph = Graph::complete(5);

    REQUIRE(graph.get_n() == 5);
    REQUIRE(graph.get_m() == 10);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 4);
    REQUIRE(graph.get_m() == 6);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 3);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 2);
    REQUIRE(graph.get_m() == 1);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 1);
    REQUIRE(graph.get_m() == 0);

    graph.remove_node(0);

    REQUIRE(graph.get_n() == 0);
    REQUIRE(graph.get_m() == 0);
}

TEST_CASE("void Graph::remove_edge(unsigned int u, unsigned int v)")
{

    Graph graph = Graph::complete(3);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 3);

    graph.remove_edge(0, 0);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 3);

    graph.remove_edge(0, 1);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 2);

    graph.remove_edge(0, 2);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 1);

    graph.remove_edge(1, 2);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 0);

    graph.remove_edge(1, 2);

    REQUIRE(graph.get_n() == 3);
    REQUIRE(graph.get_m() == 0);
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

TEST_CASE("bool Graph::is_cyclic()")
{

    Graph graph;

    graph = Graph::complete(0);

    REQUIRE(graph.is_cyclic() == false);

    graph = Graph::complete(1);

    REQUIRE(graph.is_cyclic() == false);

    graph = Graph::complete(2);

    REQUIRE(graph.is_cyclic() == false);

    graph = Graph::complete(3);

    REQUIRE(graph.is_cyclic() == true);

    graph = Graph::complete(4);

    REQUIRE(graph.is_cyclic() == true);

    graph = Graph::complete(5);

    REQUIRE(graph.is_cyclic() == true);

    graph = Graph();

    graph.insert_node();
    graph.insert_node();
    graph.insert_node();
    graph.insert_node();

    graph.insert_edge(0, 1);
    graph.insert_edge(1, 2);
    graph.insert_edge(2, 3);

    REQUIRE(graph.is_cyclic() == false);
}

TEST_CASE("bool Graph::is_connected()")
{
    Graph graph;

    graph = Graph();

    REQUIRE(graph.is_connected() == true);

    graph.insert_node();

    REQUIRE(graph.is_connected() == true);

    graph.insert_node();

    REQUIRE(graph.is_connected() == false);

    graph.insert_edge(0, 1);

    REQUIRE(graph.is_connected() == true);

    graph = Graph::complete(3);

    REQUIRE(graph.is_connected() == true);

    graph = Graph::complete(4);

    REQUIRE(graph.is_connected() == true);

    graph = Graph::complete(5);

    REQUIRE(graph.is_connected() == true);

    graph = Graph();

    graph.insert_node();
    graph.insert_node();
    graph.insert_node();
    graph.insert_node();

    graph.insert_edge(0, 1);
    graph.insert_edge(1, 2);
    graph.insert_edge(2, 3);

    REQUIRE(graph.is_connected() == true);
}
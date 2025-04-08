#ifndef DOCTEST
#define DOCTEST
#include "../../include/doctest.h"
#endif

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.hpp"
#endif

#ifndef SPANNING_TREE
#define SPANNING_TREE
#include "spanning_tree.hpp"
#endif

TEST_SUITE("spanning_tree::get_workload(const Graph &graph, const int num_threads)")
{
    TEST_CASE("Invalid parameters (n == 0)")
    {
        auto graph = Graph();

        auto [threads, start, end] = spanning_tree::get_workload(graph, 1);

        REQUIRE(threads == 0);
    }

    TEST_CASE("Invalid parameters (m == 0)")
    {
        auto graph = Graph(3);

        auto [threads, start, end] = spanning_tree::get_workload(graph, 1);

        REQUIRE(threads == 0);
    }

    TEST_CASE("Invalid parameters (m < n - 1)")
    {
        auto graph = Graph(3);

        graph.insert_edge(0, 1);

        auto [threads, start, end] = spanning_tree::get_workload(graph, 1);

        REQUIRE(threads == 0);
    }

    TEST_CASE("Invalid parameters (num_threads == 0)")
    {
        auto graph = Graph::complete(3);

        auto [threads, start, end] = spanning_tree::get_workload(graph, 0);

        REQUIRE(threads == 0);
    }

    TEST_CASE("There is only one thread")
    {
        auto graph = Graph::complete(3);

        auto [threads, start, end] = spanning_tree::get_workload(graph, 1);

        REQUIRE(threads == 1);
        REQUIRE(start[0] == 0);
        REQUIRE(end[0] == 2);
    }

    TEST_CASE("There is work enough for all threads")
    {
        auto graph = Graph::complete(5);

        auto [threads, start, end] = spanning_tree::get_workload(graph, 2);

        REQUIRE(threads == 2);
        REQUIRE(start[0] == 0);
        REQUIRE(end[0] == 3);
        REQUIRE(start[1] == 3);
        REQUIRE(end[1] == 7);
    }

    TEST_CASE("There are more threads than work")
    {
        auto graph = Graph::complete(3);

        auto [threads, start, end] = spanning_tree::get_workload(graph, 5);

        REQUIRE(threads == 2);
        REQUIRE(start[0] == 0);
        REQUIRE(end[0] == 1);
        REQUIRE(start[1] == 1);
        REQUIRE(end[1] == 2);
    }
}

#ifndef DOCTEST
#define DOCTEST
#include "../../include/doctest.h"
#endif

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "util.hpp"
#endif

TEST_CASE("int stretch(Graph graph, Graph tree)")
{
    auto graph = Graph::complete(4);
    auto tree = Graph(4);

    tree.insert_edge(0, 1);
    tree.insert_edge(1, 2);
    tree.insert_edge(2, 3);

    auto s = stretch(graph, tree);

    REQUIRE(s == 3);
}

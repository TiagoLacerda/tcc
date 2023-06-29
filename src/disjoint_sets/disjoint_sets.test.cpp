#ifndef DOCTEST
#define DOCTEST
#include "../../include/doctest.h"
#endif

#ifndef DISJOINT_SETS
#define DISJOINT_SETS
#include "disjoint_sets.hpp"
#endif

TEST_CASE("unsigned int DisjointSets::find(unsigned int u)")
{
    auto sets = DisjointSets(3);

    REQUIRE(sets.find(0) == 0);
    REQUIRE(sets.find(1) == 1);
    REQUIRE(sets.find(2) == 2);

    sets.join(0, 1);

    REQUIRE(sets.find(0) == sets.find(1));
    REQUIRE(sets.find(1) != sets.find(2));
}

TEST_CASE("void DisjointSets::join(unsigned int u, unsigned int v)")
{
    auto sets = DisjointSets(2);

    REQUIRE(sets.find(0) != sets.find(1));

    sets.join(0, 1);

    REQUIRE(sets.find(0) == sets.find(1));
}

TEST_CASE("bool DisjointSets::disjoint(unsigned int u, unsigned int v)")
{
    auto sets = DisjointSets(2);

    REQUIRE(sets.disjoint(0, 1));

    sets.join(0, 1);

    REQUIRE(!sets.disjoint(0, 1));
}
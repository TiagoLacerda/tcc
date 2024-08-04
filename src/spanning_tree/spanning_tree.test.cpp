#ifndef DOCTEST
#define DOCTEST
#include "../../include/doctest.h"
#endif

#ifndef SPANNING_TREE
#define SPANNING_TREE
#include "spanning_tree.hpp"
#endif

TEST_SUITE("spanning_tree::workload(int n, int m, int i, int *start, int *end)")
{
    TEST_CASE("Invalid parameters")
    {
        int start, end;

        spanning_tree::workload(0, 0, 0, 0, &start, &end);

        REQUIRE(start == 0);
        REQUIRE(end == 0);
    }

    TEST_CASE("There is only one thread")
    {
        int start, end;

        spanning_tree::workload(5, 10, 0, 1, &start, &end);

        REQUIRE(start == 0);
        REQUIRE(end == 7);
    }

    TEST_CASE("There is more work than threads")
    {
        int start, end;

        spanning_tree::workload(5, 10, 0, 2, &start, &end);

        REQUIRE(start == 0);
        REQUIRE(end == 3);

        spanning_tree::workload(5, 10, 1, 2, &start, &end);

        REQUIRE(start == 3);
        REQUIRE(end == 7);
    }

    TEST_CASE("There are more threads than work")
    {
        int start, end;

        spanning_tree::workload(5, 10, 0, 8, &start, &end);

        REQUIRE(start == 0);
        REQUIRE(end == 1);

        spanning_tree::workload(5, 10, 1, 8, &start, &end);

        REQUIRE(start == 1);
        REQUIRE(end == 2);

        spanning_tree::workload(5, 10, 2, 8, &start, &end);

        REQUIRE(start == 2);
        REQUIRE(end == 3);

        spanning_tree::workload(5, 10, 3, 8, &start, &end);

        REQUIRE(start == 3);
        REQUIRE(end == 4);

        spanning_tree::workload(5, 10, 4, 8, &start, &end);

        REQUIRE(start == 4);
        REQUIRE(end == 5);

        spanning_tree::workload(5, 10, 5, 8, &start, &end);

        REQUIRE(start == 5);
        REQUIRE(end == 6);

        spanning_tree::workload(5, 10, 6, 8, &start, &end);

        REQUIRE(start == 6);
        REQUIRE(end == 7);

        spanning_tree::workload(5, 10, 7, 8, &start, &end);

        REQUIRE(start == 10);
        REQUIRE(end == 10);
    }
}

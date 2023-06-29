#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

#ifndef DISJOINT_SETS
#define DISJOINT_SETS
#include "disjoint_sets.hpp"
#endif

// https://www.geeksforgeeks.org/disjoint-set-data-structures/

DisjointSets::DisjointSets(unsigned int n)
{
    this->parents = std::vector<unsigned int>();
    for (unsigned int i = 0; i < n; i++)
    {
        this->parents.push_back(i);
        this->ranks.push_back(0);
    }
};

unsigned int DisjointSets::find(unsigned int u)
{
    if (this->parents[u] == u)
    {
        // u is the representative of its set.
        return u;
    }
    else
    {
        // u is not the representative of its set, traverse the tree upwards.
        auto rep = find(this->parents[u]);

        // Make u its representative's immediate child, reducing overall tree height.
        this->parents[u] = rep;

        return rep;
    }
};

void DisjointSets::join(unsigned int u, unsigned int v)
{
    // Find the representative of u's set.
    auto u_rep = this->find(u);

    // Find the representative of v's set.
    auto v_rep = this->find(v);

    if (u_rep == v_rep)
    {
        // u and v belong to the same set.
        return;
    }

    // Get the rank of u's tree.
    auto u_rank = this->ranks[u_rep];

    // Get the rank of v's tree.
    auto v_rank = this->ranks[v_rep];

    if (u_rank < v_rank)
    {
        // Move u under v.
        this->parents[u_rep] = v_rep;
    }
    else if (v_rank < u_rank)
    {
        // Move v under u.
        this->parents[v_rep] = u_rep;
    }
    else
    {
        // Move u under v (does no matter which one gets moved, arbitratily chosen to move u).
        this->parents[u_rep] = v_rep;

        // Increment v's tree's rank.
        this->ranks[v_rep]++;
    }
};

bool DisjointSets::disjoint(unsigned int u, unsigned int v)
{
    return this->find(u) != this->find(v);
};
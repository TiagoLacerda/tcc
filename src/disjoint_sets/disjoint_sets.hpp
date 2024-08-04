#ifndef VECTOR
#define VECTOR
#include <vector>
#endif

// https://www.geeksforgeeks.org/disjoint-set-data-structures/

/// @brief An implementation of disjoint sets using two fixed-size indexed arrays, representing a virtual forest in which each tree represents a set.
class DisjointSets
{
public:
    /// @brief Default constructor.
    /// @param n The number of elements in the universe.
    DisjointSets(int n);

    /// @brief Find the representative of u.
    /// @param u An element in the universe.
    /// @return The element that represents u's set.
    int find(int u);

    /// @brief Join u's and v's sets.
    /// @param u An element in the universe.
    /// @param v An element in the universe.
    void join(int u, int v);

    /// @brief Whether u and v do not belong to the same set.
    /// @param u An element in the universe.
    /// @param v An element in the universe.
    /// @return true if u and v are not in the same set, false otherwise.
    bool disjoint(int u, int v);

private:
    /// @brief An array representing each element's parent in the trees that make up the disjoint sets, or themselves, should they be a root.
    std::vector<int> parents;

    /// @brief The height of the tree to which each element belongs, if they are a representative of a set.
    std::vector<int> ranks;
};

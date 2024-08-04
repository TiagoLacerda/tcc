class Graph;

// https://www.youtube.com/watch?v=4OQeCuLYj-4&ab_channel=MichaelSambol

/// @brief Determine the distance of the shortest path between all pairs of vertices of a graph.
/// @return A 2D array of integers where the element at [u][v] represents the distance of the shortest path between vertices u and v.
int **floyd_warshall(Graph graph);

/// @brief Determine the stretch index of a spanning tree of a graph.
/// @param graph A graph.
/// @param tree A spanning tree of [graph].
/// @return An integer representing the stretch index.
int stretch(Graph graph, Graph tree);

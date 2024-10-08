class Graph;

// https://www.youtube.com/watch?v=4OQeCuLYj-4&ab_channel=MichaelSambol

/// @brief Determine the distance of the shortest path between all pairs of vertices of a graph.
/// @return A 2D array of integers where the element at [u][v] represents the distance of the shortest path between vertices u and v.
int **floyd_warshall(const Graph &graph);

/// @brief Determine the stretch index of a spanning tree of a graph.
/// @param graph A graph.
/// @param tree A spanning tree of [graph].
/// @return An integer representing the stretch index.
int stretch(const Graph &graph, const Graph &tree);

/// @brief Determine current date and time using the format `"%FT%T%z"`.
///
/// For more info, see https://en.cppreference.com/w/cpp/io/manip/put_time.
/// @return a string representing the current date and time.
std::string now();
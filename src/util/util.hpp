#ifndef ATOMIC
#define ATOMIC
#include <atomic>
#endif

#ifndef THREAD
#define THREAD
#include <thread>
#endif

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

/// @brief Determine the number of spanning trees of a graph by leveraging Kirchoff's matrix tree theorem.
///
/// For more info, see https://en.wikipedia.org/wiki/Kirchhoff%27s_theorem.
/// @param graph A graph.
/// @return The number of spanning trees of [graph].
long long kirchoff(const Graph &graph);

/// @brief Load a collection of paths from a file. If the first line in the file is a path to an existing file, returns all lines each as a path. Otherwise, returns the path to the file itself.
/// @param path Path to the file.
/// @return a collection of paths to files.
std::vector<std::string> get_paths(const std::string &path);

std::thread track_progress(const int &count, const int &total, const std::atomic<bool> &abort);

std::thread track_progress(const std::vector<int> &counts, const int &total, const std::atomic<bool> &abort);
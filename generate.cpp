#include "macros.h"

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef SSTREAM
#define SSTREAM
#include <sstream>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef FILESYSTEM
#define FILESYSTEM
#include <filesystem>
#endif

#ifndef IOMANIP
#define IOMANIP
#include <iomanip>
#endif

#ifndef MUTEX
#define MUTEX
#include <mutex>
#endif

#ifndef MAP
#define MAP
#include <map>
#endif

#ifndef CMATH
#define CMATH
#include <cmath>
#endif

#ifndef SPANNING_TREE
#define SPANNING_TREE
#include "src/spanning_tree/spanning_tree.hpp"
#endif

#ifndef GRAPH
#define GRAPH
#include "src/graph/graph.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "src/util/util.hpp"
#endif

#ifndef SPANNING_TREE
#define SPANNING_TREE
#include "src/spanning_tree/spanning_tree.hpp"
#endif

/// @brief Validates CLI arguments.
std::tuple<int, int, int, int, int> validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    int nodes = 0, edges = 0, samples = 1, target_worker_threads = 1, retries = 1000;

    bool exit = false;

    // TODO: Fix this text
    const char *help =
        "                                                                                                         \n"
        "Usage: main                                                                                              \n"
        "                                                                                                         \n"
        "Options                                                                                                  \n"
        "-h, --help                 Show this help message.                                                       \n"
        "-n, --nodes                Number of nodes in the generated graphs.                                      \n"
        "                           Also accepts a sequence of numbers (e.g. \"-n 1 2 4 8\").                     \n"
        "-e, --edges                Edge density of the generated graphs.                                         \n"
        "                           Also accepts a sequence of numbers (e.g. \"-n 0.25 0.5 0.75 1.0.\").          \n"
        "-t, --threads              Workload should support this many threads (default: 1).                       \n"
        "-s, --samples              Number of samples per node and edge density combination (default: 1).         \n"
        "-r, --retries              How many tries to generate a graph with a given configuration (default: 1000).\n";

    try
    {
        for (auto i = 0; i < static_cast<int>(args.size()); i++)
        {
            if (args[i] == "-h" || args[i] == "--help")
            {
                exit = true;
            }

            if (args[i] == "-n" || args[i] == "--nodes")
            {
                nodes = std::stoi(args[i + 1]);

                if (nodes < 0)
                {
                    throw std::out_of_range("Number of nodes cannot be lower than 0.");
                }
            }

            if (args[i] == "-e" || args[i] == "--edges")
            {
                edges = std::stoi(args[i + 1]);

                if (edges < 1)
                {
                    throw std::out_of_range("Number of edges cannot be lower than 0.");
                }
            }

            if (args[i] == "-s" || args[i] == "--samples")
            {
                samples = std::stoi(args[i + 1]);

                if (samples < 1)
                {
                    throw std::out_of_range("Number of samples cannot be lower than 1.");
                }
            }

            if (args[i] == "-t" || args[i] == "--threads")
            {
                target_worker_threads = std::stoi(args[i + 1]);

                if (target_worker_threads < 1)
                {
                    throw std::out_of_range("Number of threads cannot be lower than 1.");
                }
            }

            if (args[i] == "-r" || args[i] == "--retries")
            {
                retries = std::stoi(args[i + 1]);

                if (retries < 0)
                {
                    throw std::out_of_range("Number of retries cannot be lower than 0.");
                }
            }
        }
    }
    catch (...)
    {
        exit = true;
    }

    if (exit)
    {
        std::cout << help << std::endl;

        std::exit(0); // TODO: See the implications of this function.
    }

    return std::tuple<int, int, int, int, int>(nodes, edges, samples, target_worker_threads, retries);
}

void print(const int &nodes, const int &edges, const int &samples, const int &target_worker_threads, const int &retries)
{
    std::cout << "Generation parameters:" << std::endl;
    std::cout << "  nodes................: " << nodes << std::endl;
    std::cout << "  edges................: " << edges << std::endl;
    std::cout << "  samples..............: " << samples << std::endl;
    std::cout << "  target_worker_threads: " << target_worker_threads << std::endl;
    std::cout << "  retries..............: " << retries << std::endl;
}

/// @brief Creates a directory for the generated graphs and returns the path to it.
/// @return The path to the directory.
std::string prepare_directory()
{
    std::time_t t = std::time(nullptr);

    std::tm tm = *std::localtime(&t);

    std::stringstream stream;

    stream << "data/graphs/generated/" << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S");

    std::string path = stream.str();

    std::filesystem::create_directories(path);

    return path;
}

Graph generate(const int &n, const int &m, const int &target_worker_threads, const int &target_spanning_trees, const int &retries)
{
    int lower = target_spanning_trees * 99 / 100;
    int upper = target_spanning_trees * 101 / 100;

    Graph champion = Graph::random_spanning_tree(n);

    champion.insert_random_edges(m - (n - 1));

    int aggressiveness = 0;

    auto [best_worker_threads, _, __] = spanning_tree::get_workload(champion, target_worker_threads);

    auto best_spanning_trees = kirchoff(champion);

    while (true)
    {
        aggressiveness = std::min(m, aggressiveness + 1);

        int tries = 1 + retries;

        while (tries > 0)
        {
            auto candidate = Graph(champion);

            candidate.remove_random_edges(aggressiveness);

            candidate.insert_random_edges(aggressiveness);

            auto [worker_threads, _, __] = spanning_tree::get_workload(candidate, target_worker_threads);

            auto spanning_trees = kirchoff(candidate);

            if (worker_threads >= target_worker_threads && lower <= spanning_trees && spanning_trees <= upper)
            {
                return candidate;
            }
            else if (best_worker_threads < target_worker_threads && worker_threads > best_worker_threads)
            {
                champion = candidate;
                best_worker_threads = worker_threads;
                best_spanning_trees = spanning_trees;
                tries = 1 + retries;
                aggressiveness = 1;
            }
            else if (worker_threads >= best_worker_threads && std::abs(spanning_trees - target_spanning_trees) < std::abs(best_spanning_trees - target_spanning_trees))
            {
                champion = candidate;
                best_worker_threads = worker_threads;
                best_spanning_trees = spanning_trees;
                tries = 1 + retries;
                aggressiveness = 1;
            }
            else
            {
                --tries;
            }
        }

        std::cout << "Failed generation, best was " << best_worker_threads << " worker threads and " << best_spanning_trees << " spanning trees. Trying again..." << std::endl;
    }
}

int main(int argc, char **argv)
{
    auto [nodes, edges, samples, target_worker_threads, retries] = validate_arguments(argc, argv);

    print(nodes, edges, samples, target_worker_threads, retries);

    // Validate and adjust generation parameters.

    int n = nodes, m = edges;

    if (target_worker_threads > n * (n - 1) * 0.5)
    {
        throw std::invalid_argument("Cannot have more worker threads than the maximum amount of edges in the graph.");
    }

    if (m < n - 1)
    {
        std::cout << "There must be at least n - 1 (" << n - 1 << ") edges for the graph to be connected." << std::endl;
        std::cout << "Would you like to proceed with " << n - 1 << " edges?" << std::endl;

        if (confirm())
        {
            m = n - 1;
        }
        else
        {
            return 0;
        }
    }

    if (target_worker_threads > m - (n - 1))
    {
        std::cout << "Cannot have more worker threads than m - (n - 1), as there wouldn't be enough edges in the edge vector for the last thread(s)." << std::endl;

        auto suggestion = n - 1 + target_worker_threads;

        std::cout << "Would you like to proceed with " << suggestion << " edges?" << std::endl;

        if (confirm())
        {
            m = suggestion;
        }
        else
        {
            return 0;
        }
    }

    // Find average amount of spanning trees for a graph with [n] nodes and [m] edges.

    long long target_spanning_trees = 0, lower = 0, upper = 0;

    for (int k = 0; k < 10000; ++k)
    {
        auto graph = Graph::random_spanning_tree(n);

        graph.insert_random_edges(m - (n - 1));

        target_spanning_trees += kirchoff(graph) / 10000;
    }

    lower = target_spanning_trees * 99 / 100;
    upper = target_spanning_trees * 101 / 100;

    std::cout << "Targeting " << target_spanning_trees << " spanning trees with accepted variance of 1% [" << lower << ", " << upper << "]." << std::endl;

    // Generate graphs

    std::vector<Graph> graphs;

    while (static_cast<int>(graphs.size()) < samples)
    {
        auto graph = generate(n, m, target_worker_threads, target_spanning_trees, retries);

        auto [worker_threads, _, __] = spanning_tree::get_workload(graph, target_worker_threads);

        auto spanning_trees = kirchoff(graph);

        if (worker_threads >= target_worker_threads && lower <= spanning_trees && spanning_trees <= upper)
        {
            std::cout << "Generated a graph with " << worker_threads << " worker threads and " << kirchoff(graph) << " spanning trees." << std::endl;

            graphs.emplace_back(graph);
        }
    }

    // Save generated graphs.

    std::string directory = prepare_directory(), path;

    for (int s = 0; s < samples; ++s)
    {
        std::stringstream stream;

        stream << directory << "/n_" << n << "_m_" << m << "_s_" << s << ".txt";

        path = stream.str();

        graphs[s].save(path);
    }
}

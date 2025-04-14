#include "macros.h"

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef SSTREAM
#define SSTREAM
#include <sstream>
#endif

#ifndef OPTIONAL
#define OPTIONAL
#include <optional>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef THREAD
#define THREAD
#include <thread>
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

#ifndef OMP
#define OMP
#include <omp.h>
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
std::tuple<std::vector<int>, std::vector<float>, int, int, int> validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    std::vector<int> nodes;
    std::vector<float> edges;
    int samples = 1, target_worker_threads = 1, retries = 1000;

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
                nodes.clear();

                for (int j = i + 1; j < static_cast<int>(args.size()); ++j)
                {
                    try
                    {
                        nodes.push_back(std::stoi(args[j]));
                    }
                    catch (const std::invalid_argument &)
                    {
                        break;
                    }
                }

                // Advance index to skip processed values
                i += static_cast<int>(nodes.size());
            }

            if (args[i] == "-e" || args[i] == "--edges")
            {
                edges.clear();

                for (int j = i + 1; j < static_cast<int>(args.size()); ++j)
                {
                    try
                    {
                        edges.push_back(std::stof(args[j]));
                    }
                    catch (const std::invalid_argument &)
                    {
                        break;
                    }
                }

                // Advance index to skip processed values
                i += static_cast<int>(edges.size());
            }

            if (args[i] == "-s" || args[i] == "--samples")
            {
                samples = std::stoi(args[i + 1]);

                if (samples < 1)
                {
                    throw std::out_of_range("Number of samples cannot be lower than 1");
                }
            }

            if (args[i] == "-t" || args[i] == "--threads")
            {
                target_worker_threads = std::stoi(args[i + 1]);

                if (target_worker_threads < 1)
                {
                    throw std::out_of_range("Number of threads cannot be lower than 1");
                }
            }

            if (args[i] == "-r" || args[i] == "--retries")
            {
                retries = std::stoi(args[i + 1]);

                if (retries < 0)
                {
                    throw std::out_of_range("Number of retries cannot be lower than 0");
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

    return std::tuple<std::vector<int>, std::vector<float>, int, int, int>(nodes, edges, samples, target_worker_threads, retries);
}

void print(const std::vector<int> &nodes, const std::vector<float> &edges, const int &samples, const int &threads, const int &retries)
{

    std::cout << "Generation parameters:" << std::endl;

    // nodes

    std::cout << "  nodes.....: [";

    for (int i = 0; i < static_cast<int>(nodes.size()); i++)
    {
        std::cout << nodes[i];

        if (i < static_cast<int>(nodes.size()) - 1)
        {
            std::cout << ", ";
        }
    }

    std::cout << "]" << std::endl;

    // edges

    std::cout << "  edges.....: [";

    for (int i = 0; i < static_cast<int>(edges.size()); i++)
    {
        std::cout << edges[i];

        if (i < static_cast<int>(edges.size()) - 1)
        {
            std::cout << ", ";
        }
    }

    std::cout << "]" << std::endl;

    //

    std::cout << "  samples...: " << samples << std::endl;
    std::cout << "  threads...: " << threads << std::endl;
    std::cout << "  retries...: " << retries << std::endl;
}

std::optional<Graph> generate(const int &n, const int &m, const int &target_worker_threads, const int &retries)
{
    int tries = 1 + retries;

    while (tries > 0)
    {
        Graph graph = Graph::random_spanning_tree(n);

        graph.insert_random_edges(m - (n - 1));

        auto [actual_worker_threads, _, __] = spanning_tree::get_workload(graph, target_worker_threads);

        if (actual_worker_threads == target_worker_threads)
        {
            return graph;
        }
        else
        {
            --tries;
        }
    };

    return std::nullopt;
}

int main(int argc, char **argv)
{
    auto [nodes, edges, samples, target_worker_threads, retries] = validate_arguments(argc, argv);

    print(nodes, edges, samples, target_worker_threads, retries);

    std::string directory, path;

    std::time_t t = std::time(nullptr);

    std::tm tm = *std::localtime(&t);

    std::stringstream stream;

    stream << "data/graphs/generated/" << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S");

    directory = stream.str();

    std::filesystem::create_directories(directory);

    std::vector<std::string> paths;

    int count = 0, total = nodes.size() * edges.size() * samples;

    int thread_num = omp_get_thread_num();

    std::atomic<bool> abort{false};

    DEBUG_ONLY(std::thread progress_thread = track_progress(count, total, abort);)

    for (auto n : nodes)
    {
        for (auto e : edges)
        {
            int m = std::floor(e * n * (n - 1) * 0.5);

            if (m < n - 1)
            {
                m = n - 1;
            }

            while (target_worker_threads > m - (n - 1) && m < n * (n - 1) * 0.5)
            {
                std::cerr << "Invalid parameters: target worker threads cannot be more than m - (n - 1), as there wouldn't be enough edges in the edge vector for the last thread." << std::endl;
                ++m;
            }

            std::vector<Graph> graphs;

            std::map<int, std::vector<Graph>> candidates;
            std::mutex mutex;

            int baseline = m;

#pragma omp parallel
            {

                while (static_cast<int>(graphs.size()) < samples && !abort.load())
                {
                    std::optional<Graph> generated = generate(n, m, target_worker_threads, retries);

                    if (generated)
                    {
                        std::cout << "Successfully generated a graph with n: " << n << ", m: " << m << std::endl;

                        auto graph = generated.value();

                        mutex.lock();

                        candidates[m].push_back(graph);

                        if (static_cast<int>(candidates[m].size()) >= samples) // If this thread found a group...
                        {
                            if (static_cast<int>(graphs.size()) >= samples) // But another thread found a group...
                            {
                                if (m < graphs[0].get_m()) // If this thread found smaller graphs...
                                {
                                    graphs = candidates[m];
                                }
                            }
                            else // And no other thread found a group...
                            {
                                graphs = candidates[m];
                            }
                        }

                        mutex.unlock();

                        m = baseline;
                    }
                    else
                    {
                        if (m < n * (n - 1) * 0.5)
                        {
                            ++m;
                        }
                    }
                }
            }

            for (int i = 0; i < samples; i++)
            {
                auto graph = graphs[i];

                stream.str("");
                stream.clear();
                stream << directory << "/" << "graph_n_" << graph.get_n() << "_m_" << graph.get_m() << "_s_" << i << ".txt";

                path = stream.str();

                paths.emplace_back(path);

                graph.save(path);

                count++;

                std::cout << "Saved: " << path << " (with up to " << kirchoff(graph) << " spanning trees)." << std::endl;
            }
        }
    }

    abort.store(true);

    DEBUG_ONLY(progress_thread.join();)

    stream.str("");
    stream.clear();

    stream << directory << "/batch.txt";

    path = stream.str();

    std::ofstream file(path);

    for (auto line : paths)
    {
        file << line << std::endl;
    }

    file.close();
}

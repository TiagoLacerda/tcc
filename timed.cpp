#include "macros.h"

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef FILESYSTEM
#define FILESYSTEM
#include <filesystem>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef FUTURE
#define FUTURE
#include <future>
#endif

#ifndef ATOMIC
#define ATOMIC
#include <atomic>
#endif

#ifndef CHRONO
#define CHRONO
#include <chrono>
#endif

#ifndef MUTEX
#define MUTEX
#include <mutex>
#endif

#ifndef IOMANIP
#define IOMANIP
#include <iomanip>
#endif

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef THREAD
#define THREAD
#include <thread>
#endif

#ifndef GRAPH
#define GRAPH
#include "src/graph/graph.hpp"
#endif

#ifndef SPANNING_TREE
#define SPANNING_TREE
#include "src/spanning_tree/spanning_tree.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "src/util/util.hpp"
#endif

#ifndef JSON
#define JSON
#include "include/json.hpp"
#endif

/// @brief Validates CLI arguments.
std::tuple<std::string, std::string, int, int> validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    std::string i_path; // Input file path.
    std::string o_path; // (Optional) Output file path.
    int seconds = 1;    // Number of evaluations per graph for averaging (default: 1).
    int threads = 1;    // Target number of threads (default: 1).

    bool exit = false;

    const char *help =
        "                                                                                                 \n"
        "Usage: main                                                                                      \n"
        "                                                                                                 \n"
        "Options                                                                                          \n"
        "-h, --help                 Show this help message.                                               \n"
        "-i, --input                Input file path.                                                      \n"
        "-o, --output               Output file path.                                                     \n"
        "-s, --seconds              Number of evaluations per graph for averaging (default: 1).           \n"
        "-t, --threads              Target number of threads.                                             \n";

    try
    {
        for (auto i = 0; i < static_cast<int>(args.size()); i++)
        {
            if (args[i] == "-h" || args[i] == "--help")
            {
                exit = true;
            }

            if (args[i] == "-i" || args[i] == "--input")
            {
                i_path = args[i + 1];
            }

            if (args[i] == "-o" || args[i] == "--output")
            {
                o_path = args[i + 1];
            }

            if (args[i] == "-s" || args[i] == "--seconds")
            {
                seconds = std::stoi(args[++i]);
            }

            if (args[i] == "-t" || args[i] == "--threads")
            {
                threads = std::stoi(args[++i]);
            }
        }

        if (i_path.empty())
        {
            throw std::invalid_argument("No input file path specified.");
        }

        if (o_path.empty())
        {
            throw std::invalid_argument("No output file path specified.");
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

    return std::tuple<std::string, std::string, int, int>(i_path, o_path, seconds, threads);
}

int main(int argc, char **argv)
{
    auto [i_path, o_path, seconds, threads] = validate_arguments(argc, argv);

    auto paths = get_paths(i_path);

    std::chrono::_V2::system_clock::time_point t0, t1;

    std::chrono::microseconds duration;

    // std::cout << "This machine supports at most " << omp_get_max_threads() << " threads." << std::endl;

    std::cout << std::fixed << std::setprecision(2);

    for (int i = 0; i < static_cast<int>(paths.size()); i++)
    {
        auto path = paths[i];

        // Graph construction

        Graph graph = Graph::load(path);

        // Tree generation

        auto total = kirchoff(graph);

        std::cout << "Kirchoff says there should be " << total << " trees in total." << std::endl;

        auto [num_threads, start, end] = spanning_tree::get_workload(graph, threads);

        std::vector<int> count(num_threads, 0);

        auto callback = [&](const Graph &tree, const int &thread_num)
        {
            ++count[thread_num];
        };

        std::atomic<bool> abort = false;

        auto work = [&]()
        {
            spanning_tree::generate_parallel(graph, callback, abort, 0, num_threads, start, end);

            auto trees = 0;

            for (int i = 0; i < num_threads; ++i)
            {
                trees += count[i];
            }

            return trees;
        };

        t0 = std::chrono::high_resolution_clock::now();

        auto future = std::async(work);

        if (future.wait_for(std::chrono::seconds(seconds)) == std::future_status::ready)
        {
            //
        }
        else
        {
            abort.store(true);
        }

        t1 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        auto trees = future.get();

        //

        std::cout << path << ":" << std::endl;
        std::cout << "  Elapsed..........: " << duration.count() << " ms (" << (abort.load() ? "aborted" : "completed") << ")." << std::endl;
        std::cout << "  Total (Kirchhoff): " << total << std::endl;
        std::cout << "  Total generated..: " << trees << std::endl;

        for (int i = 0; i < num_threads; ++i)
        {
            std::cout << "  Thread " << i << ".........: " << count[i] << std::endl;
        }

        std::cout << std::endl;
    }
}

// ./timed.exe -i data/graphs/generated/2025_04_21_21_06_21/n_20_m_31_s_0.txt -o timed.txt -s 5 -t 10
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
std::tuple<std::string, int, int> validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    std::string input_path; // Path to the file containing a graph (or a batch of graphs).
    int seconds = 1;        // Number of evaluations per graph for averaging (default: 1).
    int threads = 1;        // Target number of threads (default: 1).

    bool exit = false;

    const char *help =
        "                                                                                                 \n"
        "Usage: main                                                                                      \n"
        "                                                                                                 \n"
        "Options                                                                                          \n"
        "-h, --help                 Show this help message.                                               \n"
        "-i, --input                Path to the file containing a graph (or a batch of graphs).           \n"
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
                input_path = args[i + 1];
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

        if (input_path.empty())
        {
            throw std::invalid_argument("No input file path specified.");
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

    return std::tuple<std::string, int, int>(input_path, seconds, threads);
}

/// @brief Create a new file to which results will be written.
/// @return The path to the file.
std::string create_results_file()
{
    std::time_t t = std::time(nullptr);

    std::tm tm = *std::localtime(&t);

    std::string directory = "results/timed";

    std::filesystem::create_directories(directory);

    std::stringstream stream;

    stream << directory << "/timed_" << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S") << ".json";

    std::string path = stream.str();

    std::ofstream file(path, std::ios::app);

    file.close();

    return path;
}

int main(int argc, char **argv)
{
    auto [input_path, seconds, threads] = validate_arguments(argc, argv);

    auto paths = get_paths(input_path);

    auto output_path = create_results_file();

    std::chrono::_V2::system_clock::time_point t0, t1;

    std::chrono::seconds duration;

    // std::cout << "This machine supports at most " << omp_get_max_threads() << " threads." << std::endl;

    std::cout << std::fixed << std::setprecision(2);

    for (int i = 0; i < static_cast<int>(paths.size()); i++)
    {
        nlohmann::ordered_json data;

        auto path = paths[i];

        data["path"] = path;

        // Graph construction

        Graph graph = Graph::load(path);

        data["n"] = graph.get_n();
        data["m"] = graph.get_m();

        // Tree generation

        auto total = kirchhoff_boost(graph);

        data["kirchhoff"] = total.str();

        std::cout << "kirchhoff says there should be " << total << " trees in total." << std::endl;

        auto [num_threads, start, end] = spanning_tree::get_workload(graph, threads);

        std::vector<cpp_int> count(num_threads, 0);

        auto callback = [&](const Graph &tree, const int &thread_num)
        {
            ++count[thread_num];
        };

        std::atomic<bool> abort = false;

        auto work = [&]()
        {
            spanning_tree::generate_parallel(graph, callback, abort, 0, num_threads, start, end);

            cpp_int trees = 0;

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
        duration = std::chrono::duration_cast<std::chrono::seconds>(t1 - t0);

        auto trees = future.get();

        data["generated"] = trees.str();
        data["elapsed"] = duration.count();
        data["status"] = abort.load() ? "aborted" : "completed";

        nlohmann::json map;

        for (int t = 0; t < num_threads; ++t)
        {
            map[t] = count[t].str();
        }

        data["threads"] = map;

        // Save to file.

        std::ofstream file(output_path, std::ios::app);

        file << data.dump();

        if (i < static_cast<int>(paths.size()) - 1)
        {
            file << "," << std::endl;
        }

        file.close();
    }
}

// ./timed.exe -i batch.txt -o timed.txt -s 30 -t 8
// ./timed.exe -i data/graphs/generated/2025_04_21_21_06_21/n_20_m_31_s_0.txt -o timed.txt -s 5 -t 8
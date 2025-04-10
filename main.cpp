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

#ifndef OMP
#define OMP
#include <omp.h>
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
std::tuple<std::string, std::string, bool, bool, std::vector<int>, int> validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    std::string i_path;             // Input file path.
    std::string o_path;             // (Optional) Output file path.
    bool early_halt = false;        // Stop tree generation at lower bound (default: false).
    bool stretch_factor = true;     // Evaluate stretch factor (default: true).
    std::vector<int> threads = {1}; // Number of threads for evaluation (default: [1]).
    int samples = 1;                // Number of evaluations per graph for averaging (default: 1).

    bool exit = false;

    const char *help =
        "                                                                                                 \n"
        "Usage: main                                                                                      \n"
        "                                                                                                 \n"
        "Options                                                                                          \n"
        "-h, --help                 Show this help message.                                               \n"
        "-i, --input                Input file path.                                                      \n"
        "-o, --output               Output file path.                                                     \n"
        "--early-halt               Tree generation will stop once the lower-bound is reached.            \n"
        "--no-early-halt            Tree generation will go on until all trees are generated. (default)   \n"
        "--stretch-factor           Will evaluate the strech factor of every generated tree. (default)    \n"
        "--no-stretch-factor        Will not evaluate the stretch factor of any generated tree.           \n"
        "-t, --threads              Number of threads for evaluation (default: 1).                        \n"
        "                           Also accepts a sequence of numbers (e.g. \"-t 1 2 4 8\").             \n"
        "-s, --samples              Number of evaluations per graph for averaging (default: 1).           \n";

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

            if (args[i] == "--early-halt")
            {
                early_halt = true;
            }

            if (args[i] == "--no-early-halt")
            {
                early_halt = false;
            }

            if (args[i] == "--stretch-factor")
            {
                stretch_factor = true;
            }

            if (args[i] == "--no-stretch-factor")
            {
                stretch_factor = false;
            }

            if (args[i] == "-t" || args[i] == "--threads")
            {
                threads.clear();

                for (int j = i + 1; j < static_cast<int>(args.size()); ++j)
                {
                    try
                    {
                        threads.push_back(std::stoi(args[j]));
                    }
                    catch (const std::invalid_argument &)
                    {
                        break;
                    }
                }

                // Advance index to skip processed values
                i += static_cast<int>(threads.size());
            }

            if (args[i] == "-s" || args[i] == "--samples")
            {
                samples = std::stoi(args[i + 1]);

                if (samples < 1)
                {
                    throw std::out_of_range("Number of samples cannot be lower than 1");
                }
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

    return std::tuple<std::string, std::string, bool, bool, std::vector<int>, int>(i_path, o_path, early_halt, stretch_factor, threads, samples);
}

/// @brief Load a collection of paths from a file.
/// @param path Path to the file.
/// @return a collection of paths to files each containing a graph.
std::vector<std::string> get_paths(const std::string &path)
{
    std::vector<std::string> paths;
    std::ifstream file(path);
    std::string line;

    if (file && std::getline(file, line) && std::filesystem::exists(line))
    {
        do
        {
            paths.emplace_back(line);
        } while (std::getline(file, line));
    }
    else
    {
        paths.emplace_back(path);
    }

    return paths;
}

int main(int argc, char **argv)
{
    auto [i_path, o_path, early_halt, stretch_factor, threads, samples] = validate_arguments(argc, argv);

    auto paths = get_paths(i_path);

    std::chrono::_V2::system_clock::time_point t0, t1;

    std::chrono::microseconds duration;

    std::cout << "This machine supports at most " << omp_get_max_threads() << " threads." << std::endl;

    std::cout << std::fixed << std::setprecision(2);

    //

    {
        for (int attempt = 1; attempt <= 3; ++attempt)
        {
            try
            {
                std::ofstream file(o_path);

                file << "[";

                break;
            }
            catch (const std::exception &e)
            {
                std::cerr << "File write attempt " << attempt << " failed: " << e.what() << std::endl;

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

    for (int i = 0; i < static_cast<int>(paths.size()); i++)
    {
        nlohmann::ordered_json data;

        auto path = paths[i];

        data["path"] = path;

        // Graph construction

        Graph graph = Graph::load(path);

        data["n"] = graph.get_n();
        data["m"] = graph.get_m();

        // Girth

        t0 = std::chrono::high_resolution_clock::now();

        auto girth = graph.get_girth();

        t1 = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

        data["girth"] = girth;
        data["girth_elapsed"] = duration.count();

        // Smallest e-cycle

        t0 = std::chrono::high_resolution_clock::now();

        auto smallest_e_cycle = graph.get_smallest_e_cycle();

        t1 = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

        data["smallest_e_cycle"] = smallest_e_cycle;
        data["smallest_e_cycle_elapsed"] = duration.count();

        DEBUG_ONLY_BLOCK({
            for (auto &[key, value] : data.items())
            {
                std::cout << key << ": " << value << std::endl;
            }
        });

        // Tree generation

        data["executions"] = nlohmann::json::array();

        auto total = kirchoff(graph);

        DEBUG_ONLY(std::cout << "Kirchoff says there should be " << total << " trees in total." << std::endl;)

        for (auto t : threads)
        {
            for (int s = 0; s < samples; ++s)
            {
                int stretch_index = graph.get_n() - 1;

                int count = 0;

                int lower_bound = std::max(girth, smallest_e_cycle) - 1;

                bool abort = false;

                DEBUG_ONLY(std::thread progress_thread;)

                if (t == 1)
                {
                    auto callback = [&](const Graph &tree)
                    {
                        auto stretch_factor = stretch(graph, tree);

                        count++;

                        if (stretch_factor < stretch_index)
                        {
                            stretch_index = stretch_factor;
                        }
                    };

                    DEBUG_ONLY_BLOCK({
                        auto progress_callback = [](const int &count, const int &total, const bool &abort)
                        {
                            int progress;

                            while (!abort)
                            {
                                progress = count * 100 / total;

                                std::cout << "\r\033[2KProgress: " << progress << "%" << std::flush;

                                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                            }

                            progress = count * 100 / total;

                            std::cout << "\r\033[2KProgress: " << progress << "%" << std::flush;

                            std::cout << std::endl;
                        };

                        progress_thread = std::thread(progress_callback, std::ref(count), std::ref(total), std::ref(abort));
                    });

                    t0 = std::chrono::high_resolution_clock::now();

                    spanning_tree::generate_sequential(graph, callback, &abort, lower_bound);

                    t1 = std::chrono::high_resolution_clock::now();
                }
                else
                {
                    auto [num_threads, start, end] = spanning_tree::get_workload(graph, t);

                    std::vector<int> counts(num_threads, 0);

                    std::mutex mutex;

                    auto callback = [&](const Graph &tree, const int thread_num)
                    {
                        auto stretch_factor = stretch(graph, tree);

                        counts[thread_num]++;

                        mutex.lock();

                        if (stretch_factor < stretch_index)
                        {
                            stretch_index = stretch_factor;
                        }

                        mutex.unlock();
                    };

                    DEBUG_ONLY_BLOCK({
                        auto progress_callback = [](const std::vector<int> &counts, const int &total, const bool &abort)
                        {
                            int count;
                            int progress;

                            while (!abort)
                            {
                                count = std::accumulate(counts.begin(), counts.end(), 0);
                                progress = count * 100 / total;

                                std::cout << "\r\033[2KProgress: " << progress << "%" << std::flush;

                                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                            }

                            count = std::accumulate(counts.begin(), counts.end(), 0);
                            progress = count * 100 / total;

                            std::cout << "\r\033[2KProgress: " << progress << "%" << std::flush;

                            std::cout << std::endl;
                        };

                        progress_thread = std::thread(progress_callback, std::ref(counts), std::ref(total), std::ref(abort));
                    });

                    t0 = std::chrono::high_resolution_clock::now();

                    spanning_tree::generate_parallel(graph, callback, &abort, lower_bound, num_threads, start, end);

                    t1 = std::chrono::high_resolution_clock::now();

                    count = std::accumulate(counts.begin(), counts.end(), 0);
                }

                duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

                abort = true;

                DEBUG_ONLY(progress_thread.join();)

                nlohmann::ordered_json execution = {
                    {"elapsed", duration.count()},
                    {"spanning_trees", count},
                    {"stretch_index", stretch_index},
                    {"threads", t},
                };

                DEBUG_ONLY_BLOCK({
                    std::cout << std::endl;

                    for (auto &[key, value] : execution.items())
                    {
                        std::cout << key << ": " << value << std::endl;
                    }

                    std::cout << std::endl;
                });

                data["executions"].emplace_back(std::move(execution));
            }
        }

        {
            for (int attempt = 1; attempt <= 3; ++attempt)
            {
                try
                {
                    std::ofstream file(o_path, std::ios::app);

                    file << data.dump();

                    if (i < static_cast<int>(paths.size()) - 1)
                    {
                        file << ",";
                    }

                    break;
                }
                catch (const std::exception &e)
                {
                    std::cerr << "File write attempt " << attempt << " failed: " << e.what() << std::endl;

                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
    }

    {
        for (int attempt = 1; attempt <= 3; ++attempt)
        {
            try
            {
                std::ofstream file(o_path, std::ios::app);

                file << "]";

                break;
            }
            catch (const std::exception &e)
            {
                std::cerr << "File write attempt " << attempt << " failed: " << e.what() << std::endl;

                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
}

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

/// @brief TODO: Document
/// @param i_path
/// @param output
/// @param debug
/// @param early_halt
/// @param evaluate_stretch_factor
/// @param threads
/// @param samples
/// @return
nlohmann::ordered_json evaluate(const std::string path, const bool debug, const int samples, std::vector<int> threads)
{
    nlohmann::ordered_json data;

    std::chrono::_V2::system_clock::time_point t0, t1;

    std::chrono::microseconds duration;

    // Graph construction

    data["path"] = path;

    auto graph = Graph::load(path);

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

    //

    if (debug)
    {
        std::cout << data.dump(4) << std::endl;
    }

    // Tree generation

    data["executions"] = nlohmann::json::array();

    std::mutex mutex;

    int stretch_index;

    int count;

    int lower_bound = std::max(girth, smallest_e_cycle) - 1;

    bool abort = false;

    auto callback = [&graph, &mutex, &stretch_index, &count, &lower_bound, &abort](const Graph &tree)
    {
        auto stretch_factor = stretch(graph, tree);

        mutex.lock();

        count++;

        if (stretch_factor < stretch_index)
        {
            stretch_index = stretch_factor;
        }

        mutex.unlock();

        return stretch_factor;
    };

    for (auto t : threads)
    {
        for (int s = 0; s < samples; ++s)
        {
            stretch_index = graph.get_n() - 1;

            count = 0;

            abort = false;

            t0 = std::chrono::high_resolution_clock::now();

            spanning_tree::generate_parallel(graph, callback, &abort, lower_bound, t);

            t1 = std::chrono::high_resolution_clock::now();

            duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

            nlohmann::json execution = {{"threads", t}, {"spanning_trees", count}, {"elapsed", duration.count()}, {"stretch_index", stretch_index}};

            data["executions"].emplace_back(std::move(execution));

            if (debug)
            {
                std::cout << execution.dump(4) << std::endl;
            }
        }
    }

    return data;
}

/// @brief Validates CLI arguments.
std::tuple<std::string, std::string, bool, bool, bool, std::vector<int>, int> validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    std::string i_path;             // Input file path.
    std::string o_path;             // (Optional) Output file path.
    bool debug = false;             // Enable debug mode (outputs to console).
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
        "-d, --debug                Enable debug mode (outputs to console) (default: false).              \n"
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

            if (args[i] == "-d" || args[i] == "--debug")
            {
                debug = true;
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

    return std::tuple<std::string, std::string, bool, bool, bool, std::vector<int>, int>(i_path, o_path, debug, early_halt, stretch_factor, threads, samples);
}

int main(int argc, char **argv)
{
    auto [i_path, o_path, debug, early_halt, stretch_factor, threads, samples] = validate_arguments(argc, argv);

    // Handle input file

    std::vector<std::string> paths;

    {
        std::ifstream file(i_path);
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
            paths.emplace_back(i_path);
        }
    }

    //

    std::ofstream file(o_path);

    file << "[";
    file.flush();

    for (int i = 0; i < static_cast<int>(paths.size()); i++)
    {
        auto data = evaluate(paths[i], debug, samples, threads);

        file << data.dump();

        if (i < static_cast<int>(paths.size()) - 1)
        {

            file << ",";
        }

        file.flush();
    }

    file << "]";
    file.flush();
    file.close();
}

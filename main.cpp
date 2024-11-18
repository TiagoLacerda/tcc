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

void evaluate(const std::string i_path, std::ofstream &output, const bool debug, const bool early_halt, const bool evaluate_stretch_factor, const int threads, const int samples)
{
    auto write = output.is_open();

    std::chrono::_V2::system_clock::time_point t0, t1;

    std::chrono::microseconds duration;

    // Graph construction

    t0 = std::chrono::high_resolution_clock::now();

    auto graph = Graph::load(i_path);

    t1 = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

    if (write)
    {
        output << "{";
        output << "\"path\":\"" << i_path << "\",";
        output << "\"n\":" << graph.get_n() << ",";
        output << "\"m\":" << graph.get_m() << ",";
        output << "\"construction_microseconds\":" << duration.count() << ",";
        output.flush();
    }

    if (debug)
    {
        std::cout << "N: " << graph.get_n() << std::endl;
        std::cout << "M: " << graph.get_m() << std::endl;
        std::cout << "Construction (" << duration.count() << " us)" << std::endl;
        output.flush();
    }

    // Girth

    t0 = std::chrono::high_resolution_clock::now();

    auto girth = graph.get_girth();

    t1 = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

    if (write)
    {
        output << "\"girth\":" << girth << ",";
        output << "\"girth_microseconds\":" << duration.count() << ",";
        output.flush();
    }

    if (debug)
    {
        std::cout << "Girth: " << girth << " (" << duration.count() << " us)" << std::endl;
    }

    // Smallest e-cycle

    t0 = std::chrono::high_resolution_clock::now();

    auto smallest_e_cycle = graph.get_smallest_e_cycle();

    t1 = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

    if (write)
    {
        output << "\"smallest_e_cycle\":" << smallest_e_cycle << ",";
        output << "\"smallest_e_cycle_microseconds\":" << duration.count() << ",";
        output.flush();
    }

    if (debug)
    {
        std::cout << "Smallest e-cycle: " << smallest_e_cycle << " (" << duration.count() << " us)" << std::endl;
    }

    // Tree generation

    std::mutex mutex;

    int stretch_index;

    int count;

    int lower_bound = std::max(girth, smallest_e_cycle) - 1;

    bool abort = false;

    auto callback = [&graph, &early_halt, &mutex, &stretch_index, &count, &lower_bound, &abort](const Graph &tree)
    {
        auto stretch_factor = stretch(graph, tree);

        mutex.lock();

        count++;

        if (stretch_factor < stretch_index)
        {
            stretch_index = stretch_factor;
        }

        mutex.unlock();

        if (early_halt && stretch_factor <= lower_bound)
        {
            abort = true;
        }

        return stretch_factor;
    };

    double average = 0;

    if (write)
    {
        output << "\"runs\":[";
        output.flush();
    }

    for (int sample = 0; sample < samples; ++sample)
    {
        stretch_index = graph.get_n() - 1;

        count = 0;

        abort = false;

        t0 = std::chrono::high_resolution_clock::now();

        spanning_tree::generate(graph, callback, &abort, lower_bound, threads);

        t1 = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

        average += duration.count();

        if (write)
        {
            output << "{";
            output << "\"generation_microseconds\":" << duration.count() << ",";
            output << "\"stretch_index\":" << stretch_index << ",";
            output << "\"trees_generated\":" << count;
            output << "}";

            if (sample < samples - 1)
            {
                output << ",";
            }

            output.flush();
        }

        if (debug)
        {
            std::cout << "Stretch index: " << stretch_index << "(" << duration.count() << " us)" << std::endl;
            std::cout << "Count: " << count << std::endl;
        }
    }

    average /= samples;

    if (write)
    {
        output << "],";
        output << "\"average_generation_microseconds\":" << average << "";
        output << "}";
        output.flush();
    }

    if (debug)
    {
        std::cout << "Average genertion elapsed microseconds: " << average << std::endl;
    }
}

/// @brief Validates CLI arguments.
/// @param argc argument count.
/// @param argv argument values.
/// @return A tuple containing the following values:
///
/// `i_path`:         Input file path.
///
/// `o_path`:         (Optional) Output file path.
///
/// `debug`:          Enable debug mode (outputs to console).
///
/// `early_halt`:     Stop tree generation at lower bound (default: false).
///
/// `stretch_factor`: Evaluate stretch factor (default: true).
///
/// `threads`:        Number of threads for evaluation (default: 1).
///
/// `samples`:        Number of evaluations per graph for averaging (default: 1).
std::tuple<std::string, std::string, bool, bool, bool, int, int> validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    std::string i_path;         // Input file path.
    std::string o_path;         // (Optional) Output file path.
    bool debug = false;         // Enable debug mode (outputs to console).
    bool early_halt = false;    // Stop tree generation at lower bound (default: false).
    bool stretch_factor = true; // Evaluate stretch factor (default: true).
    int threads = 1;            // Number of threads for evaluation (default: 1).
    int samples = 1;            // Number of evaluations per graph for averaging (default: 1).

    bool exit = false;

    const char *help =
        "                                                                                                 \n"
        "Usage: main                                                                                      \n"
        "                                                                                                 \n"
        "Options                                                                                          \n"
        "-h, --help                 Show this help message.                                               \n"
        "-i, --input                Input file path.                                                      \n"
        "-o, --output               (Optional) Output file path.                                          \n"
        "-d, --debug                Enable debug mode (outputs to console) (default: false).              \n"
        "--early-halt               Tree generation will stop once the lower-bound is reached.            \n"
        "--no-early-halt            Tree generation will go on until all trees are generated. (default)   \n"
        "--stretch-factor           Will evaluate the strech factor of every generated tree. (default)    \n"
        "--no-stretch-factor        Will not evaluate the stretch factor of any generated tree.           \n"
        "-t, --threads              Number of threads for evaluation (default: 1).                        \n"
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

            if (args[i] == "-n" || args[i] == "--threads")
            {
                threads = std::stoi(args[i + 1]);

                if (threads < 1)
                {
                    throw std::out_of_range("Number of threads cannot be lower than 1");
                }
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

    return std::tuple<std::string, std::string, bool, bool, bool, int, int>(i_path, o_path, debug, early_halt, stretch_factor, threads, samples);
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

    auto write = file.is_open();

    if (write)
    {
        file << "[";
        file.flush();
    }

    for (int i = 0; i < static_cast<int>(paths.size()); i++)
    {
        evaluate(paths[i], file, debug, early_halt, stretch_factor, threads, samples);

        if (i < static_cast<int>(paths.size()) - 1)
        {

            if (write)
            {
                file << ",";
                file.flush();
            }
        }
    }

    if (write)
    {
        file << "]";
        file.flush();
    }
}

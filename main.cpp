#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
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

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
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

void evaluate(const Graph &graph, int num_threads = 1, bool early_halt = false)
{
    std::chrono::_V2::system_clock::time_point t0, t1;

    std::chrono::microseconds duration;

    //

    t0 = std::chrono::high_resolution_clock::now();

    auto girth = graph.get_girth();

    t1 = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

    std::cout << "Girth: " << girth << "(" << duration.count() << " us)" << std::endl;

    //

    t0 = std::chrono::high_resolution_clock::now();

    auto smallest_e_cycle = graph.get_smallest_e_cycle();

    t1 = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

    std::cout << "Smallest e-cycle: " << smallest_e_cycle << "(" << duration.count() << " us)" << std::endl;

    //

    std::mutex mutex;

    int stretch_index = graph.get_n() - 1;

    int count = 0;

    int lower_bound = std::max(girth, smallest_e_cycle) - 1;

    // TODO: early-halting a thread doesnt halt the others, change this to a flag acessible to all that gets passed back to the calling threads
    auto callback = [&mutex, &count, &stretch_index, &graph](const Graph &tree)
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

    t0 = std::chrono::high_resolution_clock::now();

    spanning_tree::generate(graph, callback, lower_bound, early_halt, num_threads);
    // spanning_tree::generate_sequential(graph, callback, lower_bound, early_halt);

    t1 = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

    std::cout << "Stretch index: " << stretch_index << "(" << duration.count() << " us)" << std::endl;
    std::cout << "Count: " << count << std::endl;
}

/// @brief Validates CLI arguments.
/// @param argc argument count.
/// @param argv argument values.
/// @return path to file containing a graph, the number of threads to use, whether to early-halt tree generation.
///
/// TODO: Improve this validation
std::tuple<std::string, int, bool> validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    std::string path;
    bool early_halt = false;
    int num_threads = 1;

    bool exit = false;

    const char *help =
        "                                                                                             \n"
        "Usage: main                                                                                  \n"
        "                                                                                             \n"
        "Global options                                                                               \n"
        "-h, --help                  Print this usage information.                                    \n"
        "                                                                                             \n"
        "-p, --path                  Specify the path to the file to be evaluated.                    \n"
        "-n, --num-threads           The number of threads to use in the evaluation.                  \n"
        "-e, --early-halt            Whether to stop generating trees when the lower bound is reached.\n";

    try
    {
        for (auto i = 0; i < static_cast<int>(args.size()); i++)
        {
            if (args[i] == "-p" || args[i] == "--path")
            {
                path = args[i + 1];
            }

            if (args[i] == "-n" || args[i] == "--num-threads")
            {
                num_threads = std::stoi(args[i + 1]);

                if (num_threads < 1)
                {
                    throw std::out_of_range("Number of threads cannot be lower than 1");
                }
            }

            if (args[i] == "-e" || args[i] == "--early-halt")
            {
                early_halt = true;
            }

            if (args[i] == "-h" || args[i] == "--help")
            {
                exit = true;
            }
        }

        if (path.empty())
        {
            throw std::invalid_argument("No file path specified.");
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

    return std::tuple<std::string, int, bool>(path, num_threads, early_halt);
}

int main(int argc, char **argv)
{
    auto [path, num_threads, early_halt] = validate_arguments(argc, argv);

    auto graph = Graph::load(path);

    evaluate(graph, num_threads, early_halt);
}

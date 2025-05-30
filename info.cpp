#include "macros.h"

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef GRAPH
#define GRAPH
#include "src/graph/graph.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "src/util/util.hpp"
#endif

/// @brief Validates CLI arguments.
std::string validate_arguments(int argc, char **argv)
{
    std::vector<std::string> args(argv, argv + argc);

    std::string path;

    bool exit = false;

    const char *help =
        "                                                                                                 \n"
        "Usage: info                                                                                      \n"
        "                                                                                                 \n"
        "Options                                                                                          \n"
        "-h, --help                 Show this help message.                                               \n"
        "-i, --input                Input file path.                                                      \n";
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
                path = args[i + 1];
            }
        }

        if (path.empty())
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

    return std::string(path);
}

int main(int argc, char **argv)
{
    auto path = validate_arguments(argc, argv);

    auto paths = get_paths(path);

    for (auto path : paths)
    {
        Graph graph = Graph::load(path);

        auto n = graph.get_n();
        auto m = graph.get_m();

        std::cout << "path............: " << path << std::endl;
        std::cout << "n...............: " << n << std::endl;
        std::cout << "m...............: " << m << std::endl;
        std::cout << "girth...........: " << graph.get_girth() << std::endl;
        std::cout << "smallest e-cycle: " << graph.get_smallest_e_cycle() << std::endl;
        std::cout << "spanning trees..: " << kirchhoff(graph) << std::endl;

        long long average = 0;

        for (int s = 0; s < 1000; s++)
        {
            auto random = Graph::random_spanning_tree(n);

            random.insert_random_edges(m - (n - 1));

            average += kirchhoff(random) / 1000;
        }

        std::cout << "average spanning trees: " << average << std::endl;

        std::cout << std::endl;
    }
}

// g++ -g info.cpp src/graph/graph.cpp src/disjoint_sets/disjoint_sets.cpp src/spanning_tree/spanning_tree.cpp src/util/util.cpp -o info.exe -Wall -O3 -Wno-unused-variable -fopenmp -Iinclude -I /usr/include/eigen3
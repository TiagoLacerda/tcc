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

std::ofstream log_file;

void log(std::string message)
{
    std::stringstream stringstream;

    stringstream << "[" << now() << "]: " << message;

    std::string string = stringstream.str();

    std::cout << string << std::endl;
    log_file << string << std::endl;
}

int main(int argc, char **argv)
{
    // Validate arguments

    if (argc < 2)
    {
        throw std::invalid_argument("No index file specified.");
    }

    //

    std::string date = now();

    // Open log file

    std::replace(date.begin(), date.end(), ':', '-');

    auto log_file_name = "log-" + date + ".txt";

    log_file.open(log_file_name, std::ios::out | std::ios::trunc);

    if (!log_file.is_open())
    {
        throw std::runtime_error("Failed to open log file: " + log_file_name);
    }

    // Open result file

    std::replace(date.begin(), date.end(), ':', '-');

    auto result_file_name = "result-" + date + ".json";

    std::ofstream result_file(result_file_name);

    if (!result_file.is_open())
    {
        throw std::runtime_error("Failed to open result file: " + result_file_name);
    }

    // Get graph paths

    auto index_file_name = std::string(argv[1]);

    std::ifstream index_file(index_file_name);

    if (!index_file.is_open())
    {
        throw std::runtime_error("Failed to open index file: " + index_file_name);
    }

    std::string line;
    std::vector<std::string> paths;

    while (std::getline(index_file, line))
    {
        paths.push_back(line);
    }

    index_file.close();

    //

    std::chrono::_V2::system_clock::time_point t0, t1;

    std::chrono::microseconds duration;

    result_file << "[";

    log("Found " + std::to_string(paths.size()) + " files to load");

    for (size_t i = 0; i < paths.size(); i++)
    {
        auto path = paths[i];

        log("Loading \"" + path + "\"");

        result_file << "{";
        // result_file << "\"path\":\"" << path << "\",";

        // Construction

        t0 = std::chrono::high_resolution_clock::now();

        auto graph = Graph::load(path);

        t1 = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        log("  Construction (" + std::to_string(duration.count()) + " us)");

        result_file << "\"n\":" << graph.get_n() << ",";
        result_file << "\"m\":" << graph.get_m() << ",";
        result_file << "\"construction\":" << duration.count() << ",";

        // Girth

        t0 = std::chrono::high_resolution_clock::now();

        auto girth = graph.get_girth();

        t1 = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        log("  Girth (" + std::to_string(duration.count()) + " us): " + std::to_string(girth));

        result_file << "\"girth\":" << girth << ",";
        result_file << "\"girth_microseconds\":" << duration.count() << ",";

        // Smallest-e-cycle

        t0 = std::chrono::high_resolution_clock::now();

        auto smallest_e_cycle = graph.get_smallest_e_cycle();

        t1 = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

        log("  Smallest-e-cycle (" + std::to_string(duration.count()) + " us): " + std::to_string(smallest_e_cycle));

        result_file << "\"smallest_e_cycle\":" << smallest_e_cycle << ",";
        result_file << "\"smallest_e_cycle_microseconds\":" << duration.count() << ",";

        // Tree generation

        std::mutex mutex;

        int stretch_index = graph.get_n() - 1;

        int count = 0;

        int lower_bound = std::min(girth, smallest_e_cycle - 1);

        int num_threads = 8;

        auto callback = [graph, &mutex, &stretch_index, &count](int stretch_factor)
        {
            mutex.lock();

            count++;

            if (stretch_factor < stretch_index)
            {
                stretch_index = stretch_factor;
            }

            mutex.unlock();
        };

        t0 = std::chrono::high_resolution_clock::now();

        spanning_tree::generate(graph, callback, lower_bound, num_threads);

        t1 = std::chrono::high_resolution_clock::now();

        duration = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0);

        log("  Tree generation (" + std::to_string(duration.count()) + " us)");
        log("    Generated " + std::to_string(count) + " trees");
        log("    Used " + std::to_string(num_threads) + " thread(s)");

        result_file << "\"trees_generated\":" << count << ",";
        result_file << "\"trees_generated_microseconds\":" << duration.count() << ",";
        result_file << "\"stretch_index\":" << stretch_index << ",";
        result_file << "\"num_threads\":" << num_threads;

        //

        result_file << "}";

        if (i < paths.size() - 1)
        {
            result_file << ",";
        }
    }

    result_file << "]";
}

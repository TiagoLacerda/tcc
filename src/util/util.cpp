#ifndef ATOMIC
#define ATOMIC
#include <atomic>
#endif

#ifndef ALGORITHM
#define ALGORITHM
#include <algorithm>
#endif

#ifndef NUMERIC
#define NUMERIC
#include <numeric>
#endif

#ifndef THREAD
#define THREAD
#include <thread>
#endif

#ifndef FILESYSTEM
#define FILESYSTEM
#include <filesystem>
#endif

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif

#ifndef FSTREAM
#define FSTREAM
#include <fstream>
#endif

#ifndef LIMITS
#define LIMITS
#include <limits.h>
#endif

#ifndef CHRONO
#define CHRONO
#include <chrono>
#endif

#ifndef CTIME
#define CTIME
#include <ctime>
#endif

#ifndef IOMANIP
#define IOMANIP
#include <iomanip>
#endif

#ifndef SSTREAM
#define SSTREAM
#include <sstream>
#endif

#ifndef EIGEN
#define EIGEN
#include <Eigen/Dense>
#include <Eigen/Core>
#endif

#ifndef CPP_INT
#define CPP_INT
#include <boost/multiprecision/cpp_int.hpp>
#endif

#ifndef GRAPH
#define GRAPH
#include "../graph/graph.hpp"
#endif

#ifndef UTIL
#define UTIL
#include "util.hpp"
#endif

using boost::multiprecision::cpp_int;
using Mat = Eigen::Matrix<cpp_int, Eigen::Dynamic, Eigen::Dynamic>;

int **floyd_warshall(const Graph &graph)
{
    int n = graph.get_n();

    int **distances = new int *[n];

    for (int i = 0; i < n; i++)
    {
        distances[i] = new int[n];

        for (int j = 0; j < n; j++)
        {
            distances[i][j] = INT_MAX / 2;
        }

        distances[i][i] = 0;
    }

    auto edges = graph.get_edges();

    for (auto edge = edges.begin(); edge != edges.end(); edge++)
    {
        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);

        distances[u][v] = 1;
        distances[v][u] = 1;
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                auto candidate = distances[i][k] + distances[k][j];

                if (distances[i][j] > candidate)
                {
                    distances[i][j] = candidate;
                    distances[j][i] = candidate;
                }
            }
        }
    }

    return distances;
}

int stretch(const Graph &graph, const Graph &tree)
{
    if (tree.get_m() == 0)
    {
        return 0;
    }

    int **distances = floyd_warshall(tree);

    // Iterate through [graph]'s edges to ... TODO: Document

    int stretch = 0;

    auto edges = graph.get_edges();

    for (auto edge = edges.begin(); edge != edges.end(); edge++)
    {
        auto u = std::get<0>(*edge);
        auto v = std::get<1>(*edge);

        if (distances[u][v] > stretch)
        {
            stretch = distances[u][v];
        }
    }

    // Free dynamically allocated memory

    for (auto i = 0; i < tree.get_n(); i++)
    {
        delete[] distances[i];
    }

    delete[] distances;

    //

    return stretch;
}

std::string now()
{
    auto now = std::chrono::system_clock::now();

    auto now_time_t = std::chrono::system_clock::to_time_t(now);

    auto *now_tm = std::gmtime(&now_time_t);

    std::stringstream stream;

    stream << std::put_time(now_tm, "%FT%T%z");

    return stream.str();
}

long long kirchhoff(const Graph &graph)
{
    auto n = graph.get_n();

    if (n < 2)
    {
        return 0;
    }

    auto matrix = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>(n - 1, n - 1);

    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (i == j)
            {
                matrix(i, j) = graph.get_degree(i);
            }
            else if (graph.has_edge(i, j))
            {
                matrix(i, j) = -1.0;
            }
            else
            {
                matrix(i, j) = 0.0;
            }
        }
    }

    auto determinant = matrix.determinant();

    return static_cast<long long>(std::round(determinant));
}

// fraction-free Bareiss (exact, integer-only)
static cpp_int determinant(Mat m)
{
    const int n = m.rows();

    for (int k = 0; k < n - 1; ++k)
    {
        if (m(k, k) == 0)
        {
            return 0;
        }
        for (int i = k + 1; i < n; ++i)
        {
            for (int j = k + 1; j < n; ++j)
            {
                m(i, j) = (m(i, j) * m(k, k) - m(i, k) * m(k, j)) /
                          (k ? m(k - 1, k - 1) : cpp_int(1));
            }
        }
    }

    return m(n - 1, n - 1);
}

cpp_int kirchhoff_boost(const Graph &graph)
{
    const int n = graph.get_n();

    if (n < 2)
    {
        return 0;
    }

    Mat L(n - 1, n - 1); // Laplacian Matrix of [graph].

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - 1; ++j)
        {
            if (i == j)
            {
                L(i, j) = cpp_int(graph.get_degree(i));
            }
            else if (graph.has_edge(i, j))
            {
                L(i, j) = cpp_int(-1);
            }
            else
            {
                L(i, j) = cpp_int(0);
            }
        }
    }

    return determinant(L); // exact # spanning trees
}

std::vector<std::string> get_paths(const std::string &path)
{
    std::ifstream file(path);

    if (!file)
    {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::vector<std::string> paths;
    std::string line;

    if (std::getline(file, line) && std::filesystem::exists(line))
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

std::thread track_progress(const int &count, const int &total, const std::atomic<bool> &abort)
{
    auto callback = [&]()
    {
        int progress;

        while (!abort.load())
        {
            progress = count * 100 / total;

            std::cout << "\r\033[2KProgress: " << progress << "%" << std::flush;

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }

        progress = count * 100 / total;

        std::cout << "\r\033[2KProgress: " << progress << "%" << std::flush;

        std::cout << std::endl;
    };

    return std::thread(callback);
}

std::thread track_progress(const std::vector<int> &counts, const int &total, const std::atomic<bool> &abort)
{
    auto callback = [&]()
    {
        int count;
        int progress;

        while (!abort.load())
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

    return std::thread(callback);
}

bool confirm(const std::string &prompt, const bool &value)
{
    std::string input;

    std::cout << prompt << " [" << (value ? "Y/n" : "y/N") << "]: ";
    std::getline(std::cin, input);

    while (true)
    {
        if (input.empty())
            return value;

        if (input == "y" || input == "Y")
            return true;

        if (input == "n" || input == "N")
            return false;

        std::cout << "Please respond with 'y' or 'n': ";
        std::getline(std::cin, input);
    }
}
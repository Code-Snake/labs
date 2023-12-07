#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "graph.h"
#include <tuple>
#include <random>
#include <iostream>
#include "tsp.h"
#include <chrono>
#include <vector> 
#include <unordered_set>

using namespace std::chrono;
using namespace std;

int random_number(int size) {
    srand(time(NULL));

    return rand() % size;
}

Graph random_graph(int size) {
    Graph graph;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution_distance(10, 100);

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            graph.add_edge(i, j, distribution_distance(generator) / 10.0);
        }
    }
    return graph;
}

int main(int argc, char* argv[]) {
    Catch::Session session;
    session.applyCommandLine(argc, argv);
    int result1 = session.run();
    double time_stop = 20;
    double time1 = 0;
    int N = 3;
    int muli = 1;
    do {
        Graph graph = random_graph(N);
        auto t1 = std::chrono::high_resolution_clock::now(); 
        vector<int> result = TSP(graph);
        cout << Length(result,graph) << endl;
        auto t2 = std::chrono::high_resolution_clock::now(); 
        time1 = std::chrono::duration<double>(t2 - t1).count(); 
        cout << "tsp: " << "N = " << N << ", " << "time = " << time1 << " seconds" << endl;

        t1 = std::chrono::high_resolution_clock::now(); 
        result = tsp_BranchesANDBounds(graph);
        std::cout << Length(result, graph) << std::endl;
        t2 = std::chrono::high_resolution_clock::now(); 
        time1 = std::chrono::duration<double>(t2 - t1).count(); 
        std::cout << "tsp_BranchesANDBounds: " << "N = " << N << ", " << "time = " << time1 << " seconds" << std::endl;

        t1 = std::chrono::high_resolution_clock::now(); 
        result = TSP_Greedy(graph,0);
        std::cout << Length(result, graph) << std::endl;
        t2 = std::chrono::high_resolution_clock::now(); 
        time1 = std::chrono::duration<double>(t2 - t1).count();
        std::cout << "tsp_greedy: " << "N = " << N << ", " << "time = " << time1 << " seconds" << std::endl;
        std::cout << "---------------------------------" << std::endl;
        N += muli;

    } while (time1 < time_stop);

    return result1;
}

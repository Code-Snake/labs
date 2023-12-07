#define CATCH_CONFIG_RUNNER

#include "catch.hpp"
#include "shortest_path.h"
#include "graph.h"
#include <iostream>
#include <random>
#include <numeric>
#include <chrono>
#include <ctime>

using namespace std;
using namespace std::chrono;


int random_number(int size) {
    srand(time(NULL));

    return rand() % size;
}

Graph random_graph(int size) {
    Graph graph;
    int vertices = size - 1;
    default_random_engine generator;
    uniform_int_distribution<int> distribution_distance(10, 100);

    for (int i = 0; i < vertices; ++i) {
        graph.add_edge(i, i + 1, distribution_distance(generator) / 10.0);
    }

    int additional_edges = random_number(size);

    for (int i = 0; i < additional_edges; ++i) {
        int vertex_1 = random_number(size);
        int vertex_2 = random_number(size);
        if (!(graph.has_edge(vertex_1, vertex_2) || vertex_1 == vertex_2)) {
            graph.add_edge(vertex_1, vertex_2, distribution_distance(generator) / 10.0);
        }
    }

    return graph;
}

int main(int argc, char* argv[]) {
    int result = Catch::Session().run(argc, argv);
    double time_stop = 0.1;
    double time1 = 0;
    int N = 100;
    int muli = 10;
    while (N <= 10000000) {
        Graph graph = random_graph(N);
        int vertex_1 = random_number(N);
        int vertex_2 = random_number(N);
        auto t1 = chrono::high_resolution_clock::now();
        vector<int> result = shortest_path(graph, vertex_1, vertex_2);
        auto t2 = chrono::high_resolution_clock::now();
        double time1 = chrono::duration<double>(t2 - t1).count();
        cout << "shortest_path: " << "N = " << N << ", " << "time = " << time1 << endl;
        N *= muli;
    }

    return result;

}

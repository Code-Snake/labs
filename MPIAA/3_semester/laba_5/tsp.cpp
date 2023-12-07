#include "graph.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <tuple>
#include "tsp.h"
#include <iostream>
#include <unordered_set>

using namespace std;

//полный перебор
vector<int> TSP(const Graph& graph) {
    vector<int> vertices = graph.get_vertices();
    vector<int> bestPath;
    double minCost = numeric_limits<double>::max();

    if(vertices.size() == 0){
        return vector<int> {} ;
    }

    do {
        double currentCost = 0.0;
        bool validPath = true;

        for (int i = 0; i < vertices.size() - 1; ++i) {
            if (!graph.has_edge(vertices[i], vertices[i + 1])) {
                validPath = false;
                break;
            }

            currentCost += graph.edge_weight(vertices[i], vertices[i + 1]);
        }
        if (validPath && graph.has_edge(vertices.back(), vertices.front())) {
            currentCost += graph.edge_weight(vertices.back(), vertices.front());

            if (currentCost < minCost) {
                minCost = currentCost;
                bestPath = vertices;
            }
        }

    } while (next_permutation(vertices.begin(), vertices.end()));

    return bestPath;
}

double Length(const vector<int>& path, const Graph& graph) {
    if (path.size() == 1) {
        return std::numeric_limits<double>::infinity();
    }
    double length = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        length += graph.edge_weight(path[i], path[i + 1]);
    }
    if (path.back() != path.front()) {
        length += graph.edge_weight(path.back(), path.front());
    }
    return length;
}

double LowerBound(const Graph& graph, const vector<int>& visited) {
    double lowerBound = 0.0;
    for (int v : graph.get_vertices()) {
        if (find(visited.begin(), visited.end(), v) == visited.end()) {
            vector<pair<int, double>> edges = graph.get_adjacent_edges(v);
            sort(edges.begin(), edges.end(), [](const pair<int, double>& a, const pair<int, double>& b) {
                return a.second < b.second;
                });

            if (edges.size() >= 2) {
                lowerBound += (edges[0].second + edges[1].second) / 2;
            }
            else if (!edges.empty()) {
                lowerBound += edges[0].second;
            }
        }
    }
    return lowerBound;
}

vector<int> MinPath(const vector<int>& path1, const vector<int>& path2, const Graph& graph) {
    return Length(path1, graph) < Length(path2, graph) ? path1 : path2;
}


vector<int> BnB(const Graph& graph, vector<int> visited, vector<int>& bestPath) {
    if (visited.size() == graph.get_vertices().size()) {
        return MinPath(bestPath, visited, graph);
    }

    for (int v : graph.get_vertices()) {
        if (find(visited.begin(), visited.end(), v) == visited.end()) {
            vector<int> nextVisited = visited;
            nextVisited.push_back(v);
            if (LowerBound(graph, nextVisited) < Length(bestPath, graph)) {
                vector<int> newPath = BnB(graph, nextVisited, bestPath);
                bestPath = MinPath(bestPath, newPath, graph);
            }
        }
    }
    return bestPath;
}

//брэнчес и баундс
vector<int> tsp_BranchesANDBounds(const Graph& graph) { //O(n!) - worst  
    vector<int> vertices = graph.get_vertices();
    if (vertices.empty()) {
        return vector<int>();  
    }

    if (vertices.size() == 1) {
        return vector<int>();
    }

    vector<int> initialPath = { vertices[0] }; 
    vector<int> bestPath = { 0};
    return BnB(graph, initialPath, bestPath);
}


//Жадный
vector<int> TSP_Greedy(const Graph& G, int start) {
    std::vector<int> path;
    if (G.get_vertices().empty()) {
        return path;
    }

    int current = start;
    path.push_back(current);
    std::unordered_set<int> visited;
    visited.insert(current);

    while (visited.size() < G.get_vertices().size()) {
        double min_weight = std::numeric_limits<double>::infinity();
        int next_vertex = -1;

        for (const auto& edge : G.get_adjacent_edges(current)) {
            int neighbor = edge.first;
            double weight = edge.second;
            if (visited.find(neighbor) == visited.end() && weight < min_weight) {
                min_weight = weight;
                next_vertex = neighbor;
            }
        }

        if (next_vertex == -1) {
            // Если не удалось найти следующую вершину, вернемся к начальной
            next_vertex = start;
        }

        path.push_back(next_vertex);
        visited.insert(next_vertex);
        current = next_vertex;
    }

    return path;
}
#include "graph.h"
#include <algorithm>
#include <vector>
#include <limits>
#include <numeric>
#include <queue>
#include <unordered_set>

std::vector<int> TSP(const Graph& graph);
double Length(const std::vector<int>& path, const Graph& graph);
std::vector<int> MinPath(const std::vector<int>& path1, const std::vector<int>& path2, const Graph& graph);
double LowerBound(const Graph& graph, const std::vector<int>& visited);
std::vector<int> BnB(const Graph& graph, std::vector<int> visited, std::vector<int>& bestPath);
std::vector<int> tsp_BranchesANDBounds(const Graph& graph);
std::vector<int> TSP_Greedy(const Graph& G, int start);
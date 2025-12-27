#include "graph.hpp"
#include <algorithm>

Graph::Graph(int n) : vertices_count(n) {
    adjacency_list.resize(n + 1);
}

void Graph::addEdge(int u, int v) {
    adjacency_list[u].push_back(v);
    adjacency_list[v].push_back(u);
}

const std::vector<int>& Graph::getNeighbors(int v) const {
    return adjacency_list[v];
}

int Graph::getVerticesCount() const {
    return vertices_count;
}

const std::vector<std::vector<int>>& Graph::getAdjacencyList() const {
    return adjacency_list;
}

void Graph::sortAdjacencyLists() {
    for (int i = 1; i <= vertices_count; ++i) {
        std::sort(adjacency_list[i].begin(), adjacency_list[i].end());
    }
}
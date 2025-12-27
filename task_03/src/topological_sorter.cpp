#include "topological_sorter.hpp"
#include <iostream>
#include <algorithm>

TopologicalSorter::TopologicalSorter(int n) 
    : vertices_count(n),
      has_cycle(false) {
    
    graph.resize(n);
    visited.assign(n, 0);
}

void TopologicalSorter::addEdge(int from, int to) {
    int u = from - 1;
    int v = to - 1;
    
    graph[u].push_back(v);
}

void TopologicalSorter::dfs(int v) {
    if (has_cycle) return;
    
    visited[v] = 1;
    
    for (int to : graph[v]) {
        if (visited[to] == 0) {
            dfs(to);
        } else if (visited[to] == 1) {
            has_cycle = true;
            return;
        }
    }
    
    visited[v] = 2;
    result.push_back(v);
}

bool TopologicalSorter::sort() {
    result.clear();
    has_cycle = false;
    visited.assign(vertices_count, 0);
    
    for (int v = 0; v < vertices_count; ++v) {
        if (visited[v] == 0) {
            dfs(v);
            if (has_cycle) {
                return false;
            }
        }
    }
    
    std::reverse(result.begin(), result.end());
    return true;
}

const std::vector<int>& TopologicalSorter::getOrder() const {
    return result;
}

void TopologicalSorter::solveTopologicalSort() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    TopologicalSorter sorter(n);
    
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        sorter.addEdge(u, v);
    }
    
    if (sorter.sort()) {
        const auto& order = sorter.getOrder();
        for (size_t i = 0; i < order.size(); ++i) {
            if (i > 0) std::cout << " ";
            std::cout << order[i] + 1;
        }
        std::cout << "\n";
    } else {
        std::cout << "-1\n";
    }
}
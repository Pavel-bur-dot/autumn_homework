#include "topological_sorter.hpp"
#include <iostream>
#include <algorithm>

TopologicalSorter::TopologicalSorter(int n) 
    : vertices_count(n),
      has_cycle(false) {
    
    graph.resize(n);
    visited.assign(n, 0); // 0 - White, 1 - Gray, 2 - Black
}

void TopologicalSorter::addEdge(int from, int to) {
    // Приводим к 0-based индексации (ввод с 1)
    int u = from - 1;
    int v = to - 1;
    
    graph[u].push_back(v);
}

void TopologicalSorter::dfs(int v) {
    // Если уже нашли цикл, прекращаем
    if (has_cycle) return;
    
    visited[v] = 1; // Gray - в обработке
    
    for (int to : graph[v]) {
        if (visited[to] == 0) {
            dfs(to);
        } else if (visited[to] == 1) {
            // Нашли обратное ребро - цикл!
            has_cycle = true;
            return;
        }
        // Если visited[to] == 2 (Black) - просто пропускаем
    }
    
    visited[v] = 2; // Black - обработана
    result.push_back(v);  // Добавляем в обратном порядке
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
    
    // Разворачиваем результат (DFS добавлял в обратном порядке)
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
        // Выводим результат (переводим обратно к 1-based)
        const auto& order = sorter.getOrder();
        for (size_t i = 0; i < order.size(); ++i) {
            if (i > 0) std::cout << " ";
            std::cout << order[i] + 1;  // +1 для 1-based вывода
        }
        std::cout << "\n";
    } else {
        std::cout << "-1\n";
    }
}
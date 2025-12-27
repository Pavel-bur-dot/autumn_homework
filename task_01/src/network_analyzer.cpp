#include "network_analyzer.hpp"
#include <iostream>
#include <algorithm>

NetworkAnalyzer::NetworkAnalyzer(int n) : Graph(n) {}

void NetworkAnalyzer::dfsCritical(
    int vertex,
    int parent,
    std::vector<int>& tin,
    std::vector<int>& low,
    std::vector<Color>& colors,
    int& timer,
    std::set<int>& articulation_points,
    std::vector<std::pair<int, int>>& bridges
) const {
    colors[vertex] = Color::Gray;
    tin[vertex] = low[vertex] = timer++;
    int children_count = 0;
    
    for (int neighbor : adjacency_list[vertex]) {
        if (neighbor == parent) continue;
        
        switch (colors[neighbor]) {
            case Color::White: {
                dfsCritical(neighbor, vertex, tin, low, colors, timer, 
                           articulation_points, bridges);
                low[vertex] = std::min(low[vertex], low[neighbor]);
                
                // Проверка на мост
                if (low[neighbor] > tin[vertex]) {
                    bridges.push_back({
                        std::min(vertex, neighbor),
                        std::max(vertex, neighbor)
                    });
                }
                
                // Проверка на точку сочленения (не корень)
                if (low[neighbor] >= tin[vertex] && parent != -1) {
                    articulation_points.insert(vertex);
                }
                
                children_count++;
                break;
            }
            case Color::Gray: {
                // Обратное ребро в текущем DFS дереве
                low[vertex] = std::min(low[vertex], tin[neighbor]);
                break;
            }
            case Color::Black: {
                // Уже обработанная вершина (из другой компоненты связности)
                low[vertex] = std::min(low[vertex], tin[neighbor]);
                break;
            }
        }
    }
    
    // Проверка для корня DFS
    if (parent == -1 && children_count > 1) {
        articulation_points.insert(vertex);
    }
    
    colors[vertex] = Color::Black;
}

std::pair<std::vector<int>, std::vector<std::pair<int, int>>> 
NetworkAnalyzer::findCriticalElements() const {
    std::vector<int> tin(vertices_count + 1, 0);
    std::vector<int> low(vertices_count + 1, 0);
    std::vector<Color> colors(vertices_count + 1, Color::White);
    std::set<int> articulation_points_set;
    std::vector<std::pair<int, int>> bridges;
    int timer = 0;
    
    // Запускаем DFS для каждой компоненты связности
    for (int v = 1; v <= vertices_count; ++v) {
        if (colors[v] == Color::White) {
            dfsCritical(v, -1, tin, low, colors, timer, 
                       articulation_points_set, bridges);
        }
    }
    
    // Конвертируем set в vector
    std::vector<int> articulation_points(
        articulation_points_set.begin(), 
        articulation_points_set.end()
    );
    
    // Сортируем мосты лексикографически
    std::sort(bridges.begin(), bridges.end());
    
    return {articulation_points, bridges};
}

void NetworkAnalyzer::solveNetworkProblem() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    NetworkAnalyzer analyzer(n);
    
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        analyzer.addEdge(u, v);
    }
    
    // Сортируем списки смежности (по желанию)
    analyzer.sortAdjacencyLists();
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    // Вывод результатов
    // 1. Количество критических устройств
    std::cout << articulation_points.size() << "\n";
    
    // 2. Список критических устройств
    if (articulation_points.empty()) {
        std::cout << "-\n";
    } else {
        for (size_t i = 0; i < articulation_points.size(); ++i) {
            if (i > 0) std::cout << " ";
            std::cout << articulation_points[i];
        }
        std::cout << "\n";
    }
    
    // 3. Количество критических соединений
    std::cout << bridges.size() << "\n";
    
    // 4. Список критических соединений
    if (bridges.empty()) {
        std::cout << "-\n";
    } else {
        for (size_t i = 0; i < bridges.size(); ++i) {
            if (i > 0) std::cout << "; ";
            std::cout << bridges[i].first << " " << bridges[i].second;
        }
        std::cout << "\n";
    }
}
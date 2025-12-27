#include <gtest/gtest.h>
#include <vector>
#include <utility>
#include <algorithm> // Предполагаем, что основное решение в solution.h/cpp

// Вспомогательная функция для нормализации ребра (u < v)
std::pair<int, int> NormalizeEdge(int u, int v) {
    return std::make_pair(std::min(u, v), std::max(u, v));
}

// Класс для тестирования
class NetworkAnalyzer {
private:
    int n;
    std::vector<std::vector<int>> adj;
    
    void dfs(int v, int parent, std::vector<int>& tin, std::vector<int>& low, 
             std::vector<bool>& visited, int& timer,
             std::vector<std::pair<int, int>>& bridges,
             std::vector<int>& articulation_points) {
        
        visited[v] = true;
        tin[v] = low[v] = timer++;
        int children = 0;
        bool is_articulation = false;
        
        for (int to : adj[v]) {
            if (to == parent) continue;
            
            if (!visited[to]) {
                dfs(to, v, tin, low, visited, timer, bridges, articulation_points);
                low[v] = std::min(low[v], low[to]);
                
                // Проверка на мост
                if (low[to] > tin[v]) {
                    bridges.push_back(NormalizeEdge(v, to));
                }
                
                // Проверка на точку сочленения
                if (low[to] >= tin[v] && parent != -1) {
                    is_articulation = true;
                }
                
                children++;
            } else {
                low[v] = std::min(low[v], tin[to]);
            }
        }
        
        // Для корня DFS
        if (parent == -1 && children > 1) {
            is_articulation = true;
        }
        
        if (is_articulation) {
            articulation_points.push_back(v);
        }
    }
    
public:
    NetworkAnalyzer(int vertices) : n(vertices) {
        adj.resize(n + 1); // Индексация с 1
    }
    
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    
    std::pair<std::vector<int>, std::vector<std::pair<int, int>>> findCriticalElements() {
        std::vector<int> tin(n + 1, 0);
        std::vector<int> low(n + 1, 0);
        std::vector<bool> visited(n + 1, false);
        std::vector<std::pair<int, int>> bridges;
        std::vector<int> articulation_points;
        int timer = 0;
        
        for (int i = 1; i <= n; i++) {
            if (!visited[i]) {
                dfs(i, -1, tin, low, visited, timer, bridges, articulation_points);
            }
        }
        
        // Сортируем результаты
        std::sort(articulation_points.begin(), articulation_points.end());
        std::sort(bridges.begin(), bridges.end());
        
        return {articulation_points, bridges};
    }
};

// Тест 1: Цикл из 4 вершин (cycle_no_bridges)
TEST(NetworkCriticalTest, CycleNoBridges) {
    NetworkAnalyzer analyzer(4);
    analyzer.addEdge(1, 2);
    analyzer.addEdge(2, 3);
    analyzer.addEdge(3, 4);
    analyzer.addEdge(4, 1);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    EXPECT_TRUE(articulation_points.empty());
    EXPECT_TRUE(bridges.empty());
}

// Тест 2: Простая цепочка из 4 вершин (simple_chain)
TEST(NetworkCriticalTest, SimpleChain) {
    NetworkAnalyzer analyzer(4);
    analyzer.addEdge(1, 2);
    analyzer.addEdge(2, 3);
    analyzer.addEdge(3, 4);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {2, 3};
    EXPECT_EQ(articulation_points, expected_articulations);
    
    std::vector<std::pair<int, int>> expected_bridges = {
        {1, 2}, {2, 3}, {3, 4}
    };
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 3: Одно ребро (single_edge)
TEST(NetworkCriticalTest, SingleEdge) {
    NetworkAnalyzer analyzer(2);
    analyzer.addEdge(1, 2);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    EXPECT_TRUE(articulation_points.empty());
    
    std::vector<std::pair<int, int>> expected_bridges = {{1, 2}};
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 4: Звезда с центром в вершине 1 (star_graph)
TEST(NetworkCriticalTest, StarGraph) {
    NetworkAnalyzer analyzer(5);
    analyzer.addEdge(1, 2);
    analyzer.addEdge(1, 3);
    analyzer.addEdge(1, 4);
    analyzer.addEdge(1, 5);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {1};
    EXPECT_EQ(articulation_points, expected_articulations);
    
    std::vector<std::pair<int, int>> expected_bridges = {
        {1, 2}, {1, 3}, {1, 4}, {1, 5}
    };
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 5: Сбалансированное дерево (tree_balanced)
TEST(NetworkCriticalTest, TreeBalanced) {
    NetworkAnalyzer analyzer(7);
    analyzer.addEdge(1, 2);
    analyzer.addEdge(1, 3);
    analyzer.addEdge(2, 4);
    analyzer.addEdge(2, 5);
    analyzer.addEdge(3, 6);
    analyzer.addEdge(3, 7);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {1, 2, 3};
    std::sort(expected_articulations.begin(), expected_articulations.end());
    
    std::sort(articulation_points.begin(), articulation_points.end());
    EXPECT_EQ(articulation_points, expected_articulations);
    
    std::vector<std::pair<int, int>> expected_bridges = {
        {1, 2}, {1, 3}, {2, 4}, {2, 5}, {3, 6}, {3, 7}
    };
    std::sort(expected_bridges.begin(), expected_bridges.end());
    
    std::sort(bridges.begin(), bridges.end());
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 6: Две несвязные цепочки (two_components)
TEST(NetworkCriticalTest, TwoComponents) {
    NetworkAnalyzer analyzer(6);
    analyzer.addEdge(1, 2);
    analyzer.addEdge(2, 3);
    analyzer.addEdge(4, 5);
    analyzer.addEdge(5, 6);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {2, 5};
    std::sort(expected_articulations.begin(), expected_articulations.end());
    
    std::sort(articulation_points.begin(), articulation_points.end());
    EXPECT_EQ(articulation_points, expected_articulations);
    
    std::vector<std::pair<int, int>> expected_bridges = {
        {1, 2}, {2, 3}, {4, 5}, {5, 6}
    };
    std::sort(expected_bridges.begin(), expected_bridges.end());
    
    std::sort(bridges.begin(), bridges.end());
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 7: Полный граф K5 (complete_graph)
TEST(NetworkCriticalTest, CompleteGraph) {
    NetworkAnalyzer analyzer(5);
    // Все рёбра полного графа K5
    for (int i = 1; i <= 5; i++) {
        for (int j = i + 1; j <= 5; j++) {
            analyzer.addEdge(i, j);
        }
    }
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    EXPECT_TRUE(articulation_points.empty());
    EXPECT_TRUE(bridges.empty());
}

// Тест 8: Цикл с хордой (cycle_with_chord)
TEST(NetworkCriticalTest, CycleWithChord) {
    NetworkAnalyzer analyzer(5);
    // Цикл 1-2-3-4-1
    analyzer.addEdge(1, 2);
    analyzer.addEdge(2, 3);
    analyzer.addEdge(3, 4);
    analyzer.addEdge(4, 1);
    // Хорда 1-3
    analyzer.addEdge(1, 3);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    EXPECT_TRUE(articulation_points.empty());
    EXPECT_TRUE(bridges.empty());
}

// Тест 9: Два цикла, соединённые мостом (bridge_between_cycles)
TEST(NetworkCriticalTest, BridgeBetweenCycles) {
    NetworkAnalyzer analyzer(6);
    // Первый цикл: 1-2-3-1
    analyzer.addEdge(1, 2);
    analyzer.addEdge(2, 3);
    analyzer.addEdge(3, 1);
    
    // Второй цикл: 4-5-6-4
    analyzer.addEdge(4, 5);
    analyzer.addEdge(5, 6);
    analyzer.addEdge(6, 4);
    
    // Мост: 3-4
    analyzer.addEdge(3, 4);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {3, 4};
    std::sort(expected_articulations.begin(), expected_articulations.end());
    
    std::sort(articulation_points.begin(), articulation_points.end());
    EXPECT_EQ(articulation_points, expected_articulations);
    
    std::vector<std::pair<int, int>> expected_bridges = {{3, 4}};
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 10: Длинная цепочка (long_chain)
TEST(NetworkCriticalTest, LongChain) {
    NetworkAnalyzer analyzer(8);
    analyzer.addEdge(1, 2);
    analyzer.addEdge(2, 3);
    analyzer.addEdge(3, 4);
    analyzer.addEdge(4, 5);
    analyzer.addEdge(5, 6);
    analyzer.addEdge(6, 7);
    analyzer.addEdge(7, 8);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {2, 3, 4, 5, 6, 7};
    EXPECT_EQ(articulation_points, expected_articulations);
    
    std::vector<std::pair<int, int>> expected_bridges = {
        {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}
    };
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 11: Одна вершина (single_vertex)
TEST(NetworkCriticalTest, SingleVertex) {
    NetworkAnalyzer analyzer(1);
    // Нет рёбер
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    EXPECT_TRUE(articulation_points.empty());
    EXPECT_TRUE(bridges.empty());
}

// Тест 12: Дерево сложной структуры (complex_tree)
TEST(NetworkCriticalTest, ComplexTree) {
    NetworkAnalyzer analyzer(10);
    analyzer.addEdge(1, 2);
    analyzer.addEdge(1, 3);
    analyzer.addEdge(2, 4);
    analyzer.addEdge(2, 5);
    analyzer.addEdge(3, 6);
    analyzer.addEdge(3, 7);
    analyzer.addEdge(4, 8);
    analyzer.addEdge(4, 9);
    analyzer.addEdge(7, 10);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {1, 2, 3, 4, 7};
    std::sort(expected_articulations.begin(), expected_articulations.end());
    
    std::sort(articulation_points.begin(), articulation_points.end());
    EXPECT_EQ(articulation_points, expected_articulations);
    
    std::vector<std::pair<int, int>> expected_bridges = {
        {1, 2}, {1, 3}, {2, 4}, {2, 5}, {3, 6}, {3, 7}, {4, 8}, {4, 9}, {7, 10}
    };
    std::sort(expected_bridges.begin(), expected_bridges.end());
    
    std::sort(bridges.begin(), bridges.end());
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 13: Треугольник с хвостом (triangle_with_tail)
TEST(NetworkCriticalTest, TriangleWithTail) {
    NetworkAnalyzer analyzer(5);
    // Треугольник 1-2-3-1
    analyzer.addEdge(1, 2);
    analyzer.addEdge(2, 3);
    analyzer.addEdge(3, 1);
    // Хвост 3-4-5
    analyzer.addEdge(3, 4);
    analyzer.addEdge(4, 5);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {3, 4};
    std::sort(expected_articulations.begin(), expected_articulations.end());
    
    std::sort(articulation_points.begin(), articulation_points.end());
    EXPECT_EQ(articulation_points, expected_articulations);
    
    std::vector<std::pair<int, int>> expected_bridges = {
        {3, 4}, {4, 5}
    };
    std::sort(expected_bridges.begin(), expected_bridges.end());
    
    std::sort(bridges.begin(), bridges.end());
    EXPECT_EQ(bridges, expected_bridges);
}

// Тест 14: Вершина, соединяющая 3 треугольника (articulation_with_3_triangles)
TEST(NetworkCriticalTest, ArticulationWithThreeTriangles) {
    NetworkAnalyzer analyzer(7);
    // Вершина 1 соединяет 3 треугольника
    analyzer.addEdge(1, 2);
    analyzer.addEdge(1, 3);
    analyzer.addEdge(2, 3); // Первый треугольник
    
    analyzer.addEdge(1, 4);
    analyzer.addEdge(1, 5);
    analyzer.addEdge(4, 5); // Второй треугольник
    
    analyzer.addEdge(1, 6);
    analyzer.addEdge(1, 7);
    analyzer.addEdge(6, 7); // Третий треугольник
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    std::vector<int> expected_articulations = {1};
    EXPECT_EQ(articulation_points, expected_articulations);
    
    // Нет мостов - все рёбра входят в циклы
    EXPECT_TRUE(bridges.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
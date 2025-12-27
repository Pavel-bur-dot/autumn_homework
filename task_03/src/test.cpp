#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <unordered_set>

// Простая реализация для тестов
class SimpleTopologicalSorter {
private:
    int n;
    std::vector<std::vector<int>> graph;
    std::vector<int> visited; // 0 - white, 1 - gray, 2 - black
    std::vector<int> result;
    bool has_cycle;
    
    void dfs(int v) {
        if (has_cycle) return;
        
        visited[v] = 1; // gray
        
        for (int to : graph[v]) {
            if (visited[to] == 0) {
                dfs(to);
            } else if (visited[to] == 1) {
                has_cycle = true;
                return;
            }
        }
        
        visited[v] = 2; // black
        result.push_back(v);
    }
    
public:
    SimpleTopologicalSorter(int vertices) : n(vertices), has_cycle(false) {
        graph.resize(n);
        visited.resize(n, 0);
    }
    
    void addEdge(int from, int to) {
        graph[from].push_back(to);
    }
    
    bool sort() {
        result.clear();
        has_cycle = false;
        std::fill(visited.begin(), visited.end(), 0);
        
        for (int v = 0; v < n; ++v) {
            if (visited[v] == 0) {
                dfs(v);
                if (has_cycle) return false;
            }
        }
        
        std::reverse(result.begin(), result.end());
        return true;
    }
    
    const std::vector<int>& getOrder() const {
        return result;
    }
};

// Вспомогательная функция проверки
bool checkOrder(int n, const std::vector<std::pair<int, int>>& edges, const std::vector<int>& order) {
    if (order.size() != (size_t)n) return false;
    
    std::vector<int> pos(n);
    for (size_t i = 0; i < order.size(); ++i) {
        pos[order[i]] = i;
    }
    
    for (auto [u, v] : edges) {
        if (pos[u] >= pos[v]) return false;
    }
    
    return true;
}

// ТЕСТ 1: Самый простой DAG - одна вершина
TEST(TopologicalSortTest, SingleVertex) {
    SimpleTopologicalSorter sorter(1);
    
    EXPECT_TRUE(sorter.sort());
    auto order = sorter.getOrder();
    EXPECT_EQ(order.size(), 1);
    EXPECT_EQ(order[0], 0);
}

// ТЕСТ 2: Две вершины, одно ребро
TEST(TopologicalSortTest, TwoVerticesOneEdge) {
    SimpleTopologicalSorter sorter(2);
    sorter.addEdge(0, 1); // 0 -> 1
    
    EXPECT_TRUE(sorter.sort());
    auto order = sorter.getOrder();
    
    EXPECT_EQ(order.size(), 2);
    EXPECT_EQ(order[0], 0);
    EXPECT_EQ(order[1], 1);
}

// ТЕСТ 3: Три вершины в цепочке
TEST(TopologicalSortTest, ThreeVerticesChain) {
    SimpleTopologicalSorter sorter(3);
    sorter.addEdge(0, 1);
    sorter.addEdge(1, 2); // 0 -> 1 -> 2
    
    EXPECT_TRUE(sorter.sort());
    auto order = sorter.getOrder();
    
    std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}};
    EXPECT_TRUE(checkOrder(3, edges, order));
}

// ТЕСТ 4: Цикл из 2 вершин
TEST(TopologicalSortTest, CycleTwoVertices) {
    SimpleTopologicalSorter sorter(2);
    sorter.addEdge(0, 1);
    sorter.addEdge(1, 0); // 0 <-> 1 цикл
    
    EXPECT_FALSE(sorter.sort()); // Должен обнаружить цикл
}

// ТЕСТ 5: Петля (self-loop)
TEST(TopologicalSortTest, SelfLoop) {
    SimpleTopologicalSorter sorter(1);
    sorter.addEdge(0, 0); // петля
    
    EXPECT_FALSE(sorter.sort()); // Петля = цикл
}

// ТЕСТ 6: Несвязный граф
TEST(TopologicalSortTest, DisconnectedTwoComponents) {
    SimpleTopologicalSorter sorter(4);
    sorter.addEdge(0, 1); // 0 -> 1
    sorter.addEdge(2, 3); // 2 -> 3 (отдельная компонента)
    
    EXPECT_TRUE(sorter.sort());
    auto order = sorter.getOrder();
    
    // Проверяем что все вершины присутствуют
    std::vector<bool> found(4, false);
    for (int v : order) {
        EXPECT_TRUE(v >= 0 && v < 4);
        found[v] = true;
    }
    for (bool f : found) EXPECT_TRUE(f);
}

// ТЕСТ 7: Граф без рёбер
TEST(TopologicalSortTest, NoEdges) {
    SimpleTopologicalSorter sorter(3);
    
    EXPECT_TRUE(sorter.sort());
    auto order = sorter.getOrder();
    
    EXPECT_EQ(order.size(), 3);
    // Любой порядок допустим, главное все вершины присутствуют
    std::vector<bool> found(3, false);
    for (int v : order) {
        EXPECT_TRUE(v >= 0 && v < 3);
        found[v] = true;
    }
    for (bool f : found) EXPECT_TRUE(f);
}

// ТЕСТ 8: Два независимых ребра
TEST(TopologicalSortTest, TwoIndependentEdges) {
    SimpleTopologicalSorter sorter(4);
    sorter.addEdge(0, 1);
    sorter.addEdge(2, 3);
    
    EXPECT_TRUE(sorter.sort());
    auto order = sorter.getOrder();
    
    std::vector<std::pair<int, int>> edges = {{0, 1}, {2, 3}};
    EXPECT_TRUE(checkOrder(4, edges, order));
}

// ТЕСТ 9: Бинарное дерево (три вершины)
TEST(TopologicalSortTest, BinaryTree) {
    SimpleTopologicalSorter sorter(3);
    sorter.addEdge(0, 1); // корень -> левый
    sorter.addEdge(0, 2); // корень -> правый
    
    EXPECT_TRUE(sorter.sort());
    auto order = sorter.getOrder();
    
    // Корень должен быть перед детьми
    int root_pos = -1, left_pos = -1, right_pos = -1;
    for (size_t i = 0; i < order.size(); ++i) {
        if (order[i] == 0) root_pos = i;
        if (order[i] == 1) left_pos = i;
        if (order[i] == 2) right_pos = i;
    }
    
    EXPECT_TRUE(root_pos < left_pos);
    EXPECT_TRUE(root_pos < right_pos);
    // left и right могут быть в любом порядке
}

// ТЕСТ 10: Обратная цепочка
TEST(TopologicalSortTest, ReverseChain) {
    SimpleTopologicalSorter sorter(3);
    sorter.addEdge(2, 1);
    sorter.addEdge(1, 0); // 2 -> 1 -> 0
    
    EXPECT_TRUE(sorter.sort());
    auto order = sorter.getOrder();
    
    EXPECT_EQ(order[0], 2);
    EXPECT_EQ(order[1], 1);
    EXPECT_EQ(order[2], 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
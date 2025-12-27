#include <gtest/gtest.h>
#include "city_connector.hpp"

// Вспомогательная функция для создания графа из списка рёбер
CityConnector createConnectorFromEdges(int n, const std::vector<std::pair<int, int>>& edges) {
    CityConnector connector(n);
    for (const auto& [from, to] : edges) {
        connector.addRoad(from, to);
    }
    return connector;
}

// Тест 1: Пример из условия (2 вершины, 1 ребро)
TEST(CityConnectorTest, ReadmeCase1) {
    std::vector<std::pair<int, int>> edges = {{1, 2}};
    CityConnector connector = createConnectorFromEdges(2, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 1);
}

// Тест 2: Пример из условия (4 вершины, 3 ребра)
TEST(CityConnectorTest, ReadmeCase2) {
    std::vector<std::pair<int, int>> edges = {{1, 2}, {3, 2}, {2, 4}};
    CityConnector connector = createConnectorFromEdges(4, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 2);
}

// Тест 3: Пример из условия (5 вершин, 4 ребра)
TEST(CityConnectorTest, ReadmeCase3) {
    std::vector<std::pair<int, int>> edges = {{1, 2}, {3, 2}, {2, 4}, {2, 5}};
    CityConnector connector = createConnectorFromEdges(5, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 2);
}

// Тест 4: Сильно связный компонент (цикл из 3 вершин)
TEST(CityConnectorTest, StronglyConnectedCycle) {
    std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 1}};
    CityConnector connector = createConnectorFromEdges(3, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 0);
}

// Тест 5: Цепочка из 3 SCC (исправленный)
TEST(CityConnectorTest, ThreeSCCsChain) {
    std::vector<std::pair<int, int>> edges = {
        {1, 2}, {2, 1},  // SCC1: {1, 2}
        {3, 4}, {4, 3},  // SCC2: {3, 4}
        {2, 3},          // SCC1 -> SCC2
        {4, 5}           // SCC2 -> {5} (SCC3)
    };
    CityConnector connector = createConnectorFromEdges(5, edges);  // 5 вершин, не 6!
    EXPECT_EQ(connector.findMinRoadsToConnect(), 1);
}

// Тест 6: Две независимые SCC
TEST(CityConnectorTest, TwoIndependentSCCs) {
    std::vector<std::pair<int, int>> edges = {
        {1, 2}, {2, 1},  // SCC1: {1, 2}
        {3, 4}, {4, 3}   // SCC2: {3, 4}
    };
    CityConnector connector = createConnectorFromEdges(4, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 2);
}

// Тест 7: Две изолированные вершины (нет рёбер)
TEST(CityConnectorTest, TwoIsolatedVertices) {
    std::vector<std::pair<int, int>> edges = {};
    CityConnector connector = createConnectorFromEdges(2, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 2);
}

// Тест 8: Одна вершина с петлёй
TEST(CityConnectorTest, SingleVertexWithSelfLoop) {
    std::vector<std::pair<int, int>> edges = {{1, 1}};
    CityConnector connector = createConnectorFromEdges(1, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 0);
}

// Тест 9: Полный сильно связный граф
TEST(CityConnectorTest, CompleteStronglyConnectedGraph) {
    std::vector<std::pair<int, int>> edges;
    for (int i = 1; i <= 4; ++i) {
        for (int j = 1; j <= 4; ++j) {
            if (i != j) {
                edges.push_back({i, j});
            }
        }
    }
    CityConnector connector = createConnectorFromEdges(4, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 0);
}

// Тест 10: Длинная цепочка DAG
TEST(CityConnectorTest, LongChainDAG) {
    std::vector<std::pair<int, int>> edges = {{1, 2}, {2, 3}, {3, 4}, {4, 5}};
    CityConnector connector = createConnectorFromEdges(5, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 1);
}

// Тест 11: Две параллельные цепочки
TEST(CityConnectorTest, TwoParallelChains) {
    std::vector<std::pair<int, int>> edges = {
        {1, 2}, {2, 3},
        {4, 5}, {5, 6}
    };
    CityConnector connector = createConnectorFromEdges(6, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 2);
}

// Тест 12: Смешанный случай с несколькими SCC
TEST(CityConnectorTest, MixedMultipleSCCs) {
    std::vector<std::pair<int, int>> edges = {
        {1, 2}, {2, 1},  // SCC1
        {4, 5}, {5, 4},  // SCC3
        {2, 3},          // SCC1 -> SCC2
        {3, 4}           // SCC2 -> SCC3
    };
    CityConnector connector = createConnectorFromEdges(5, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 1);
}

// Тест 13: Одна вершина без рёбер
TEST(CityConnectorTest, SingleVertexNoEdges) {
    CityConnector connector(1);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 0);
}

// Тест 14: Множество изолированных вершин
TEST(CityConnectorTest, MultipleIsolatedVertices) {
    CityConnector connector(5);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 5);
}

// Тест 15: Граф с петлями
TEST(CityConnectorTest, GraphWithSelfLoops) {
    std::vector<std::pair<int, int>> edges = {
        {1, 2}, {2, 3},
        {1, 1}, {2, 2}, {3, 3}
    };
    CityConnector connector = createConnectorFromEdges(3, edges);
    EXPECT_EQ(connector.findMinRoadsToConnect(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#include <gtest/gtest.h>
#include "graph.hpp"
#include "network_analyzer.hpp"

// Вспомогательная функция для нормализации ребра
std::pair<int, int> normalizeEdge(int u, int v) {
    return std::make_pair(std::min(u, v), std::max(u, v));
}

TEST(GraphTest, ConstructionAndEdges) {
    Graph graph(5);
    
    EXPECT_EQ(graph.getVerticesCount(), 5);
    
    graph.addEdge(1, 2);
    graph.addEdge(2, 3);
    
    EXPECT_EQ(graph.getNeighbors(1).size(), 1);
    EXPECT_EQ(graph.getNeighbors(2).size(), 2);
    EXPECT_EQ(graph.getNeighbors(3).size(), 1);
}

TEST(NetworkAnalyzerTest, SingleEdgeGraph) {
    NetworkAnalyzer analyzer(2);
    analyzer.addEdge(1, 2);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    EXPECT_TRUE(articulation_points.empty());
    ASSERT_EQ(bridges.size(), 1);
    
    auto normalized_bridge = normalizeEdge(bridges[0].first, bridges[0].second);
    EXPECT_EQ(normalized_bridge, std::make_pair(1, 2));
}

TEST(NetworkAnalyzerTest, CycleGraph) {
    NetworkAnalyzer analyzer(4);
    analyzer.addEdge(1, 2);
    analyzer.addEdge(2, 3);
    analyzer.addEdge(3, 4);
    analyzer.addEdge(4, 1);
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    EXPECT_TRUE(articulation_points.empty());
    EXPECT_TRUE(bridges.empty());
}

TEST(NetworkAnalyzerTest, SimpleChain) {
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

TEST(NetworkAnalyzerTest, StarGraph) {
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
    
    std::sort(bridges.begin(), bridges.end());
    std::sort(expected_bridges.begin(), expected_bridges.end());
    EXPECT_EQ(bridges, expected_bridges);
}

TEST(NetworkAnalyzerTest, TreeGraph) {
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

TEST(NetworkAnalyzerTest, TwoDisconnectedComponents) {
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

TEST(NetworkAnalyzerTest, SingleVertex) {
    NetworkAnalyzer analyzer(1);
    // Нет рёбер
    
    auto [articulation_points, bridges] = analyzer.findCriticalElements();
    
    EXPECT_TRUE(articulation_points.empty());
    EXPECT_TRUE(bridges.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
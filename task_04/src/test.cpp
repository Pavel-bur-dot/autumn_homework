#include <gtest/gtest.h>
#include "johnson_algorithm.hpp"
#include <vector>

// Тест 1: Граф с одной вершиной
TEST(JohnsonAlgorithmTest, SingleVertex) {
    JohnsonAlgorithm solver(1);
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty()); // Нет отрицательных циклов
    EXPECT_EQ(distances.size(), 1);
    EXPECT_EQ(distances[0][0], 0);
}

// Тест 2: Две вершины, положительное ребро
TEST(JohnsonAlgorithmTest, TwoVerticesPositiveEdge) {
    JohnsonAlgorithm solver(2);
    solver.addEdge(1, 2, 5);
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty());
    EXPECT_EQ(distances[0][0], 0); // 1->1
    EXPECT_EQ(distances[0][1], 5); // 1->2
    EXPECT_EQ(distances[1][0], JohnsonAlgorithm::INF); // 2->1 нет пути
    EXPECT_EQ(distances[1][1], 0); // 2->2
}

// Тест 3: Треугольник с положительными весами
TEST(JohnsonAlgorithmTest, TrianglePositiveWeights) {
    JohnsonAlgorithm solver(3);
    solver.addEdge(1, 2, 1);
    solver.addEdge(2, 3, 2);
    solver.addEdge(1, 3, 4);
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty());
    // 1->3 через 2 быстрее (1+2=3), чем прямо (4)
    EXPECT_EQ(distances[0][2], 3);
}

// Тест 4: Граф с отрицательными весами (но без отрицательных циклов)
TEST(JohnsonAlgorithmTest, NegativeWeightsNoCycle) {
    JohnsonAlgorithm solver(3);
    solver.addEdge(1, 2, -1);
    solver.addEdge(2, 3, -2);
    solver.addEdge(1, 3, 2);
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty());
    // 1->3 через 2: -1 + -2 = -3, что меньше 2
    EXPECT_EQ(distances[0][2], -3);
}

// Тест 5: Обнаружение отрицательного цикла
TEST(JohnsonAlgorithmTest, NegativeCycleDetection) {
    JohnsonAlgorithm solver(3);
    // Цикл с отрицательным суммарным весом: 1->2->3->1 = -1
    solver.addEdge(1, 2, 1);
    solver.addEdge(2, 3, -3);
    solver.addEdge(3, 1, 1);
    
    auto distances = solver.findAllShortestPaths();
    
    // Должен обнаружить отрицательный цикл и вернуть пустой результат
    EXPECT_TRUE(distances.empty());
}

// Тест 6: Граф без рёбер
TEST(JohnsonAlgorithmTest, NoEdges) {
    JohnsonAlgorithm solver(4);
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty());
    // Все диагонали должны быть 0
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(distances[i][i], 0);
    }
    // Все недиагональные элементы должны быть INF (нет пути)
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i != j) {
                EXPECT_EQ(distances[i][j], JohnsonAlgorithm::INF);
            }
        }
    }
}

// Тест 7: Простой путь из 4 вершин
TEST(JohnsonAlgorithmTest, SimplePath) {
    JohnsonAlgorithm solver(4);
    solver.addEdge(1, 2, 1);
    solver.addEdge(2, 3, 2);
    solver.addEdge(3, 4, 3);
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty());
    // Проверяем расстояния
    EXPECT_EQ(distances[0][3], 6); // 1->4: 1+2+3=6
    EXPECT_EQ(distances[1][3], 5); // 2->4: 2+3=5
    EXPECT_EQ(distances[2][3], 3); // 3->4: 3
}

// Тест 8: Граф с несколькими путями между вершинами
TEST(JohnsonAlgorithmTest, MultiplePaths) {
    JohnsonAlgorithm solver(4);
    solver.addEdge(1, 2, 2);
    solver.addEdge(1, 3, 5);
    solver.addEdge(2, 4, 3);
    solver.addEdge(3, 4, 1);
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty());
    // 1->4: через 2: 2+3=5, через 3: 5+1=6, выбираем минимум 5
    EXPECT_EQ(distances[0][3], 5);
}

// Тест 9: Отрицательное ребро с компенсацией
TEST(JohnsonAlgorithmTest, NegativeEdgeWithCompensation) {
    JohnsonAlgorithm solver(3);
    solver.addEdge(1, 2, -5);
    solver.addEdge(2, 3, 8);
    solver.addEdge(1, 3, 4);
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty());
    // 1->3: прямо 4, через 2: -5+8=3, выбираем 3
    EXPECT_EQ(distances[0][2], 3);
}

// Тест 10: Изолированная вершина
TEST(JohnsonAlgorithmTest, IsolatedVertex) {
    JohnsonAlgorithm solver(3);
    solver.addEdge(1, 2, 1);
    // Вершина 3 изолирована
    
    auto distances = solver.findAllShortestPaths();
    
    ASSERT_FALSE(distances.empty());
    // К изолированной вершине нет путей
    EXPECT_EQ(distances[0][2], JohnsonAlgorithm::INF); // 1->3 нет пути
    EXPECT_EQ(distances[2][0], JohnsonAlgorithm::INF); // 3->1 нет пути
    EXPECT_EQ(distances[2][2], 0); // 3->3 = 0
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
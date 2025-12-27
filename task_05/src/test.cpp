#include <gtest/gtest.h>
#include "mst.hpp"

// Вспомогательная функция для создания графа
LimitedDegreeMST createSolver(int n, int d, const std::vector<std::tuple<int, int, int>>& edges) {
    LimitedDegreeMST solver(n, d);
    for (const auto& [u, v, w] : edges) {
        solver.addEdge(u, v, w);
    }
    return solver;
}

// Тест 1: Простой граф с 2 вершинами
TEST(LimitedDegreeMSTTest, TwoVertices) {
    // Граф: 1 --(5)-- 2
    auto solver = createSolver(2, 1, {{1, 2, 5}});
    
    int result = solver.findLimitedDegreeMST();
    
    // Должно получиться MST весом 5
    EXPECT_EQ(result, 5);
}

// Тест 2: Треугольник с разными весами
TEST(LimitedDegreeMSTTest, TriangleGraph) {
    // Граф: треугольник
    // 1--2: 1
    // 2--3: 2  
    // 1--3: 3
    // d = 2 (любая степень допустима)
    auto solver = createSolver(3, 2, {
        {1, 2, 1},
        {2, 3, 2},
        {1, 3, 3}
    });
    
    int result = solver.findLimitedDegreeMST();
    
    // MST: 1-2 (1) + 2-3 (2) = 3
    EXPECT_EQ(result, 3);
}

// Тест 3: Слишком строгое ограничение степени
TEST(LimitedDegreeMSTTest, TooStrictDegreeLimit) {
    // Граф: 3 вершины в цепочке
    // 1--2--3
    // d = 1 (каждая вершина может иметь максимум 1 ребро)
    auto solver = createSolver(3, 1, {
        {1, 2, 1},
        {2, 3, 1}
    });
    
    int result = solver.findLimitedDegreeMST();
    
    // Невозможно построить дерево: вершина 2 будет иметь степень 2
    EXPECT_EQ(result, -1);
}

// Тест 4: Граф без рёбер (одна вершина)
TEST(LimitedDegreeMSTTest, SingleVertex) {
    auto solver = createSolver(1, 0, {});
    
    int result = solver.findLimitedDegreeMST();
    
    // MST для одной вершины имеет вес 0
    EXPECT_EQ(result, 0);
}

// Тест 5: Квадрат (4 вершины)
TEST(LimitedDegreeMSTTest, SquareGraph) {
    // Граф: квадрат
    // 1--2: 1
    // 2--3: 2
    // 3--4: 3
    // 4--1: 4
    // 1--3: 5 (диагональ)
    // d = 2
    auto solver = createSolver(4, 2, {
        {1, 2, 1},
        {2, 3, 2},
        {3, 4, 3},
        {4, 1, 4},
        {1, 3, 5}
    });
    
    int result = solver.findLimitedDegreeMST();
    
    // MST: 1-2(1) + 2-3(2) + 3-4(3) = 6
    // Или: 1-2(1) + 1-4(4) + 2-3(2) = 7
    // Алгоритм должен найти минимальный = 6
    EXPECT_EQ(result, 6);
}

// Тест 6: Звезда (star graph)
TEST(LimitedDegreeMSTTest, StarGraph) {
    // Граф: звезда с центром в вершине 1
    // d = 3 (центр может иметь 3 ребра, остальные 1)
    auto solver = createSolver(4, 3, {
        {1, 2, 1},
        {1, 3, 2},
        {1, 4, 3},
        {2, 3, 10}, // лишние рёбра
        {3, 4, 20}
    });
    
    int result = solver.findLimitedDegreeMST();
    
    // MST: все рёбра из центра 1-2(1) + 1-3(2) + 1-4(3) = 6
    EXPECT_EQ(result, 6);
}

// Тест 7: Несвязный граф
TEST(LimitedDegreeMSTTest, DisconnectedGraph) {
    // Две отдельные компоненты: 1-2 и 3-4
    auto solver = createSolver(4, 2, {
        {1, 2, 1},
        {3, 4, 1}
    });
    
    int result = solver.findLimitedDegreeMST();
    
    // Невозможно построить MST для несвязного графа
    EXPECT_EQ(result, -1);
}

// Тест 8: Граф где нужно выбрать "правильные" рёбра
TEST(LimitedDegreeMSTTest, SmartEdgeSelection) {
    // Граф: 
    // 1--2: 1
    // 1--3: 100
    // 2--3: 2
    // 3--4: 1
    // 2--4: 100
    // d = 2
    auto solver = createSolver(4, 2, {
        {1, 2, 1},
        {1, 3, 100},
        {2, 3, 2},
        {3, 4, 1},
        {2, 4, 100}
    });
    
    int result = solver.findLimitedDegreeMST();
    
    // Оптимальное MST: 1-2(1) + 2-3(2) + 3-4(1) = 4
    // Нельзя взять 1-3(100) или 2-4(100)
    EXPECT_EQ(result, 4);
}

// Тест 9: Все рёбра одинакового веса
TEST(LimitedDegreeMSTTest, UniformWeights) {
    // Полный граф K4 с весом 1 на всех рёбрах
    // d = 2
    auto solver = createSolver(4, 2, {
        {1, 2, 1},
        {1, 3, 1},
        {1, 4, 1},
        {2, 3, 1},
        {2, 4, 1},
        {3, 4, 1}
    });
    
    int result = solver.findLimitedDegreeMST();
    
    // Любое дерево из 3 рёбер весом 1 = 3
    EXPECT_EQ(result, 3);
}

// Тест 10: Ограничение степени = n-1 (нет ограничений)
TEST(LimitedDegreeMSTTest, NoDegreeLimit) {
    // Граф как в тесте 5, но d = 3 (максимум для 4 вершин)
    auto solver = createSolver(4, 3, {
        {1, 2, 1},
        {2, 3, 2},
        {3, 4, 3},
        {4, 1, 4},
        {1, 3, 5}
    });
    
    int result = solver.findLimitedDegreeMST();
    
    // Должен найти обычное MST
    EXPECT_EQ(result, 6);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
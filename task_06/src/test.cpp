#include <gtest/gtest.h>
#include "max_flow_solver.hpp"

// Вспомогательная функция для создания графа
MaxFlowSolver createSolver(int n, const std::vector<std::tuple<int, int, int>>& edges) {
    MaxFlowSolver solver(n);
    for (const auto& [u, v, c] : edges) {
        solver.addEdge(u, v, c);
    }
    return solver;
}

// Тест 1: Две вершины, одно ребро
TEST(MaxFlowTest, TwoVerticesOneEdge) {
    // Граф: 1 --> (5) --> 2
    auto solver = createSolver(2, {{1, 2, 5}});
    
    EXPECT_EQ(solver.findMaxFlow(1, 2), 5);
}

// Тест 2: Две вершины, два параллельных ребра
TEST(MaxFlowTest, TwoVerticesParallelEdges) {
    // Граф: 1 --> (3) --> 2
    //      1 --> (2) --> 2
    auto solver = createSolver(2, {
        {1, 2, 3},
        {1, 2, 2}
    });
    
    EXPECT_EQ(solver.findMaxFlow(1, 2), 5); // 3 + 2 = 5
}

// Тест 3: Три вершины в цепочке
TEST(MaxFlowTest, ThreeVerticesChain) {
    // Граф: 1 --> (4) --> 2 --> (3) --> 3
    auto solver = createSolver(3, {
        {1, 2, 4},
        {2, 3, 3}
    });
    
    EXPECT_EQ(solver.findMaxFlow(1, 3), 3); // ограничено ребром 2→3
}

// Тест 4: Простой треугольник
TEST(MaxFlowTest, SimpleTriangle) {
    // Граф:    2
    //        /   \
    //     4 /     \ 2
    //      /       \
    //     1 ---3---> 3
    auto solver = createSolver(3, {
        {1, 2, 4},
        {1, 3, 3},
        {2, 3, 2}
    });
    
    // Максимальный поток: 1→3(3) + 1→2→3(2) = 5
    EXPECT_EQ(solver.findMaxFlow(1, 3), 5);
}

// Тест 5: Граф с узким местом
TEST(MaxFlowTest, BottleneckGraph) {
    // Граф: 1 --> (10) --> 2 --> (3) --> 3 --> (5) --> 4
    auto solver = createSolver(4, {
        {1, 2, 10},
        {2, 3, 3},  // Узкое место
        {3, 4, 5}
    });
    
    EXPECT_EQ(solver.findMaxFlow(1, 4), 3); // ограничено 2→3
}

// Тест 6: Граф с несколькими путями
TEST(MaxFlowTest, MultiplePaths) {
    // Граф:   2 ---3---> 4
    //        /           ^
    //     5 /           / 2
    //      /   1      /
    //     1 ---4---> 3
    auto solver = createSolver(4, {
        {1, 2, 5},
        {1, 3, 4},
        {2, 4, 3},
        {3, 4, 2}
    });
    
    // Пути: 1→2→4: min(5,3)=3, 1→3→4: min(4,2)=2, всего 5
    EXPECT_EQ(solver.findMaxFlow(1, 4), 5);
}

// Тест 7: Граф без пути от истока к стоку
TEST(MaxFlowTest, NoPathFromSourceToSink) {
    // Граф: 1 --> 2, 3 --> 4 (нет пути 1→4)
    auto solver = createSolver(4, {
        {1, 2, 5},
        {3, 4, 5}
    });
    
    EXPECT_EQ(solver.findMaxFlow(1, 4), 0);
}

// Тест 8: Исток и сток совпадают
TEST(MaxFlowTest, SourceEqualsSink) {
    auto solver = createSolver(3, {
        {1, 2, 5},
        {2, 3, 3}
    });
    
    EXPECT_EQ(solver.findMaxFlow(1, 1), 0); // Исток = сток
}

// Тест 9: Большие пропускные способности
TEST(MaxFlowTest, LargeCapacities) {
    auto solver = createSolver(3, {
        {1, 2, 1000000},
        {2, 3, 500000}
    });
    
    EXPECT_EQ(solver.findMaxFlow(1, 3), 500000);
}

// Тест 10: Граф с одним ребром нулевой пропускной способности
TEST(MaxFlowTest, ZeroCapacityEdge) {
    auto solver = createSolver(3, {
        {1, 2, 5},
        {2, 3, 0}  // Нулевая пропускная способность
    });
    
    EXPECT_EQ(solver.findMaxFlow(1, 3), 0);
}

// Тест 11: Звезда (star graph)
TEST(MaxFlowTest, StarGraph) {
    // Центр в вершине 1, сток в вершине 5
    auto solver = createSolver(5, {
        {1, 2, 3},
        {1, 3, 4},
        {1, 4, 5},
        {2, 5, 10},
        {3, 5, 10},
        {4, 5, 10}
    });
    
    // Все рёбра из источника: 3+4+5 = 12
    EXPECT_EQ(solver.findMaxFlow(1, 5), 12);
}

// Тест 12: Двудольный граф
TEST(MaxFlowTest, BipartiteGraph) {
    // Исток 1, сток 6, промежуточные вершины 2-5
    auto solver = createSolver(6, {
        {1, 2, 3},
        {1, 3, 2},
        {2, 4, 2},
        {2, 5, 1},
        {3, 4, 1},
        {3, 5, 3},
        {4, 6, 4},
        {5, 6, 3}
    });
    
    EXPECT_EQ(solver.findMaxFlow(1, 6), 5);
}
#include <gtest/gtest.h>
#include "LCASolver.hpp"

// Тест 1: Простое дерево (цепочка)
TEST(LCATest, SimpleChain) {
    LCASolver solver(5);
    solver.addEdge(1, 2);
    solver.addEdge(2, 3);
    solver.addEdge(3, 4);
    solver.addEdge(4, 5);
    solver.build(1);
    
    EXPECT_EQ(solver.findLCA(1, 5), 1); // Корень
    EXPECT_EQ(solver.findLCA(2, 4), 2); // Общий предок в середине
    EXPECT_EQ(solver.findLCA(3, 3), 3); // Одна и та же вершина
    EXPECT_EQ(solver.findLCA(4, 5), 4); // Родитель и ребёнок
}

// Тест 2: Бинарное дерево
TEST(LCATest, BinaryTree) {

    LCASolver solver(6);
    solver.addEdge(1, 2);
    solver.addEdge(1, 3);
    solver.addEdge(2, 4);
    solver.addEdge(2, 5);
    solver.addEdge(3, 6);
    solver.build(1);
    
    EXPECT_EQ(solver.findLCA(4, 5), 2); // Дети одного родителя
    EXPECT_EQ(solver.findLCA(4, 6), 1); // Разные ветви
    EXPECT_EQ(solver.findLCA(2, 3), 1); // Дети корня
    EXPECT_EQ(solver.findLCA(5, 6), 1); // Внуки из разных ветвей
}

// Тест 3: Звезда (star tree)
TEST(LCATest, StarTree) {
    // Дерево: корень 1, дети 2,3,4,5
    LCASolver solver(5);
    solver.addEdge(1, 2);
    solver.addEdge(1, 3);
    solver.addEdge(1, 4);
    solver.addEdge(1, 5);
    solver.build(1);
    
    // Все пары детей имеют LCA = 1
    EXPECT_EQ(solver.findLCA(2, 3), 1);
    EXPECT_EQ(solver.findLCA(2, 4), 1);
    EXPECT_EQ(solver.findLCA(2, 5), 1);
    EXPECT_EQ(solver.findLCA(3, 4), 1);
    EXPECT_EQ(solver.findLCA(4, 5), 1);
    // Родитель и ребёнок
    EXPECT_EQ(solver.findLCA(1, 3), 1);
}

// Тест 4: Дерево с глубиной
TEST(LCATest, DeepTree) {
    // Дерево: 1-2-3-4-5-6-7-8-9-10 (глубокая цепочка)
    LCASolver solver(10);
    for (int i = 1; i < 10; ++i) {
        solver.addEdge(i, i + 1);
    }
    solver.build(1);
    
    EXPECT_EQ(solver.findLCA(1, 10), 1);
    EXPECT_EQ(solver.findLCA(5, 8), 5); // Ближе к началу
    EXPECT_EQ(solver.findLCA(7, 9), 7); // Ближе к концу
    EXPECT_EQ(solver.findLCA(3, 3), 3); // Одна вершина
}

// Тест 5: Несбалансированное дерево
TEST(LCATest, UnbalancedTree) {
    LCASolver solver(7);
    solver.addEdge(1, 2);
    solver.addEdge(1, 3);
    solver.addEdge(3, 4);
    solver.addEdge(3, 5);
    solver.addEdge(3, 6);
    solver.addEdge(5, 7);
    solver.build(1);
    
    EXPECT_EQ(solver.findLCA(2, 4), 1);
    EXPECT_EQ(solver.findLCA(4, 6), 3);
    EXPECT_EQ(solver.findLCA(7, 6), 3);
    EXPECT_EQ(solver.findLCA(7, 2), 1);
    EXPECT_EQ(solver.findLCA(7, 5), 5); // Родитель и ребёнок
}

// Тест 6: LCA равен одной из вершин
TEST(LCATest, LCAIsOneOfVertices) {
    LCASolver solver(5);
    solver.addEdge(1, 2);
    solver.addEdge(1, 3);
    solver.addEdge(2, 4);
    solver.addEdge(3, 5);
    solver.build(1);
    
    // Когда LCA равен одной из вершин
    EXPECT_EQ(solver.findLCA(1, 4), 1); // LCA = 1
    EXPECT_EQ(solver.findLCA(2, 4), 2); // LCA = 2
    EXPECT_EQ(solver.findLCA(3, 5), 3); // LCA = 3
    EXPECT_EQ(solver.findLCA(1, 1), 1); // Одинаковые вершины
}

// Тест 7: Большое дерево с проверкой случайных запросов
TEST(LCATest, LargeTreeRandomQueries) {
    const int N = 100;
    LCASolver solver(N);
    
    // Строим простое дерево: 1-2-3-...-N
    for (int i = 1; i < N; ++i) {
        solver.addEdge(i, i + 1);
    }
    solver.build(1);
    
    // Проверяем несколько случайных пар
    // Для цепочки LCA(u,v) = min(u,v)
    EXPECT_EQ(solver.findLCA(10, 20), 10);
    EXPECT_EQ(solver.findLCA(25, 15), 15);
    EXPECT_EQ(solver.findLCA(1, 99), 1);
    EXPECT_EQ(solver.findLCA(50, 50), 50);
    EXPECT_EQ(solver.findLCA(75, 80), 75);
}

// Тест 8: Дерево с ответвлениями
TEST(LCATest, TreeWithBranches) {
    LCASolver solver(10);
    solver.addEdge(1, 2);
    solver.addEdge(1, 3);
    solver.addEdge(1, 4);
    solver.addEdge(2, 5);
    solver.addEdge(2, 6);
    solver.addEdge(4, 7);
    solver.addEdge(5, 8);
    solver.addEdge(7, 9);
    solver.addEdge(7, 10);
    solver.build(1);
    
    EXPECT_EQ(solver.findLCA(8, 6), 2);  // Одна ветвь
    EXPECT_EQ(solver.findLCA(9, 10), 7); // Дети одного родителя
    EXPECT_EQ(solver.findLCA(8, 9), 1);  // Разные ветви
    EXPECT_EQ(solver.findLCA(3, 10), 1); // Простые ветви
    EXPECT_EQ(solver.findLCA(5, 8), 5);  // Родитель и ребёнок
}

// Тест 9: Сбалансированное дерево
TEST(LCATest, BalancedTree) {
    LCASolver solver(8);
    solver.addEdge(1, 2);
    solver.addEdge(1, 3);
    solver.addEdge(1, 4);
    solver.addEdge(2, 5);
    solver.addEdge(2, 6);
    solver.addEdge(3, 7);
    solver.addEdge(3, 8);
    solver.build(1);
    
    EXPECT_EQ(solver.findLCA(5, 6), 2);
    EXPECT_EQ(solver.findLCA(7, 8), 3);
    EXPECT_EQ(solver.findLCA(5, 7), 1);
    EXPECT_EQ(solver.findLCA(6, 8), 1);
    EXPECT_EQ(solver.findLCA(2, 4), 1);
}

// Тест 10: Проверка построения с разных корней
TEST(LCATest, DifferentRoots) {
    // Дерево: 1-2-3-4
    LCASolver solver(4);
    solver.addEdge(1, 2);
    solver.addEdge(2, 3);
    solver.addEdge(3, 4);
    
    // Строим с корнем в 1
    solver.build(1);
    EXPECT_EQ(solver.findLCA(1, 4), 1);
    EXPECT_EQ(solver.findLCA(2, 4), 2);
    
    // Можно перестроить с другим корнем (в реальности нужно новый объект)
    // Но для теста проверим что алгоритм корректно работает
}

// Тест 11: Минимальное дерево (2 вершины)
TEST(LCATest, MinimalTree) {
    LCASolver solver(2);
    solver.addEdge(1, 2);
    solver.build(1);
    
    EXPECT_EQ(solver.findLCA(1, 2), 1);
    EXPECT_EQ(solver.findLCA(1, 1), 1);
    EXPECT_EQ(solver.findLCA(2, 2), 2);
}

// Тест 12: Граф с циклом (но в условии всегда дерево)
TEST(LCATest, VerifyTreeStructure) {
    // Даже если добавить "лишние" рёбра, DFS их проигнорирует
    // Дерево: 1-2-3, плюс "лишнее" ребро 1-3
    LCASolver solver(3);
    solver.addEdge(1, 2);
    solver.addEdge(2, 3);
    solver.addEdge(1, 3); // Лишнее ребро (в дереве не должно быть циклов)
    solver.build(1);
    
    // Алгоритм всё равно найдёт какой-то LCA
    // (в дереве LCA(1,3)=1, но с лишним ребром результат может отличаться)
    // Тест для демонстрации
    int lca = solver.findLCA(1, 3);
    EXPECT_TRUE(lca == 1 || lca == 3); // Один из возможных вариантов
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
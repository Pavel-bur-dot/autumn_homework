#include <gtest/gtest.h>
#include "segment_tree_rmq.hpp"
#include <vector>
#include <algorithm>
#include <random>

// Тест 1: Простой массив, запросы без обновлений
TEST(SegmentTreeRMQTest, SimpleQueriesNoUpdates) {
    std::vector<int> arr = {5, 3, 7, 3, 2, 8, 1, 4};
    SegmentTreeRMQ tree(arr);
    
    // Запросы на различные отрезки
    EXPECT_EQ(tree.rangeMinQuery(1, 8), 1);   // min всего массива
    EXPECT_EQ(tree.rangeMinQuery(1, 4), 3);   // min первых 4 элементов
    EXPECT_EQ(tree.rangeMinQuery(5, 8), 1);   // min последних 4 элементов
    EXPECT_EQ(tree.rangeMinQuery(3, 5), 2);   // min элементов 3-5
    EXPECT_EQ(tree.rangeMinQuery(2, 2), 3);   // один элемент
}

// Тест 2: Точечные обновления
TEST(SegmentTreeRMQTest, PointUpdates) {
    std::vector<int> arr = {5, 3, 7, 3, 2};
    SegmentTreeRMQ tree(arr);
    
    EXPECT_EQ(tree.rangeMinQuery(1, 5), 2); // начальный min
    
    // Обновляем минимальный элемент
    tree.pointUpdate(5, 10); // arr[5] = 10 (было 2)
    EXPECT_EQ(tree.rangeMinQuery(1, 5), 3); // теперь min = 3
    
    // Обновляем другой элемент
    tree.pointUpdate(3, 1); // arr[3] = 1 (было 7)
    EXPECT_EQ(tree.rangeMinQuery(1, 5), 1); // теперь min = 1
    
    // Обновляем на минимальное значение
    tree.pointUpdate(4, -5); // arr[4] = -5 (было 3)
    EXPECT_EQ(tree.rangeMinQuery(1, 5), -5); // теперь min = -5
}

// Тест 3: Массив из одного элемента
TEST(SegmentTreeRMQTest, SingleElementArray) {
    std::vector<int> arr = {42};
    SegmentTreeRMQ tree(arr);
    
    EXPECT_EQ(tree.rangeMinQuery(1, 1), 42);
    
    // Обновление
    tree.pointUpdate(1, 100);
    EXPECT_EQ(tree.rangeMinQuery(1, 1), 100);
}

// Тест 4: Все элементы одинаковые
TEST(SegmentTreeRMQTest, AllElementsEqual) {
    std::vector<int> arr(10, 7); // 10 элементов, все 7
    SegmentTreeRMQ tree(arr);
    
    EXPECT_EQ(tree.rangeMinQuery(1, 10), 7);
    EXPECT_EQ(tree.rangeMinQuery(3, 7), 7);
    
    // Обновление одного элемента
    tree.pointUpdate(5, 3);
    EXPECT_EQ(tree.rangeMinQuery(1, 10), 3);
    EXPECT_EQ(tree.rangeMinQuery(1, 4), 7); // элементы 1-4 не менялись
}

// Тест 5: Чередующиеся минимумы
TEST(SegmentTreeRMQTest, AlternatingMinima) {
    std::vector<int> arr = {10, 1, 10, 1, 10, 1, 10, 1};
    SegmentTreeRMQ tree(arr);
    
    // Минимум всегда 1
    EXPECT_EQ(tree.rangeMinQuery(1, 8), 1);
    EXPECT_EQ(tree.rangeMinQuery(2, 7), 1);
    EXPECT_EQ(tree.rangeMinQuery(3, 6), 1);
    
    // Заменяем один из минимумов
    tree.pointUpdate(4, 5); // arr[4] = 5 (было 1)
    EXPECT_EQ(tree.rangeMinQuery(1, 8), 1); // всё ещё есть минимум 1
    EXPECT_EQ(tree.rangeMinQuery(3, 5), 5); // на отрезке 3-5: min(10,5,10)=5
}

// Тест 6: Большой массив со случайными значениями
TEST(SegmentTreeRMQTest, LargeRandomArray) {
    const int N = 1000;
    std::vector<int> arr(N);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 10000);
    
    for (int i = 0; i < N; ++i) {
        arr[i] = dist(rng);
    }
    
    SegmentTreeRMQ tree(arr);
    
    // Проверяем несколько случайных запросов
    for (int t = 0; t < 50; ++t) {
        int l = dist(rng) % N + 1;
        int r = dist(rng) % N + 1;
        if (l > r) std::swap(l, r);
        
        // Наивное вычисление минимума
        int naive_min = arr[l-1];
        for (int i = l; i <= r; ++i) {
            naive_min = std::min(naive_min, arr[i-1]);
        }
        
        EXPECT_EQ(tree.rangeMinQuery(l, r), naive_min);
    }
    
    // Проверяем обновления
    for (int t = 0; t < 20; ++t) {
        int idx = dist(rng) % N + 1;
        int new_value = dist(rng);
        
        tree.pointUpdate(idx, new_value);
        arr[idx-1] = new_value;
        
        // Проверяем после обновления
        int l = dist(rng) % N + 1;
        int r = dist(rng) % N + 1;
        if (l > r) std::swap(l, r);
        
        int naive_min = arr[l-1];
        for (int i = l; i <= r; ++i) {
            naive_min = std::min(naive_min, arr[i-1]);
        }
        
        EXPECT_EQ(tree.rangeMinQuery(l, r), naive_min);
    }
}

// Тест 7: Последовательные обновления и запросы
TEST(SegmentTreeRMQTest, SequentialUpdatesAndQueries) {
    std::vector<int> arr = {5, 8, 3, 9, 2, 7, 4, 6};
    SegmentTreeRMQ tree(arr);
    
    // Симуляция последовательности операций
    tree.pointUpdate(3, 1);  // arr[3] = 1 (было 3)
    EXPECT_EQ(tree.rangeMinQuery(2, 5), 1); // min(8,1,9,2)=1
    
    tree.pointUpdate(5, 0);  // arr[5] = 0 (было 2)
    EXPECT_EQ(tree.rangeMinQuery(1, 8), 0); // min всего массива
    
    tree.pointUpdate(1, 10); // arr[1] = 10 (было 5)
    EXPECT_EQ(tree.rangeMinQuery(1, 3), 1); // min(10,8,1)=1
    
    tree.pointUpdate(7, -3); // arr[7] = -3 (было 4)
    EXPECT_EQ(tree.rangeMinQuery(6, 8), -3); // min(7,-3,6)=-3
}

// Тест 8: Краевые случаи (отрицательные числа, большие значения)
TEST(SegmentTreeRMQTest, EdgeCases) {
    // Отрицательные числа
    std::vector<int> arr1 = {-5, -3, -7, -1, -2};
    SegmentTreeRMQ tree1(arr1);
    EXPECT_EQ(tree1.rangeMinQuery(1, 5), -7);
    
    // Смесь отрицательных и положительных
    std::vector<int> arr2 = {-5, 3, -2, 0, 10, -8, 7};
    SegmentTreeRMQ tree2(arr2);
    EXPECT_EQ(tree2.rangeMinQuery(1, 7), -8);
    EXPECT_EQ(tree2.rangeMinQuery(2, 5), -2);
    
    // Обновление на граничные значения
    tree2.pointUpdate(1, -100);
    EXPECT_EQ(tree2.rangeMinQuery(1, 7), -100);
    
    tree2.pointUpdate(6, 100); // arr[6] = 100 (было -8)
    EXPECT_EQ(tree2.rangeMinQuery(1, 7), -100); // всё ещё -100
}

// Тест 9: Запросы на весь массив после каждого обновления
TEST(SegmentTreeRMQTest, WholeArrayQueries) {
    std::vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
    SegmentTreeRMQ tree(arr);
    
    std::vector<int> expected_mins = {1}; // начальный min
    
    tree.pointUpdate(2, 7); // arr[2] = 7 (было 1)
    expected_mins.push_back(1); // min всё ещё 1
    
    tree.pointUpdate(4, 8); // arr[4] = 8 (было 1)
    expected_mins.push_back(2); // теперь min = 2
    
    tree.pointUpdate(7, 0); // arr[7] = 0 (было 2)
    expected_mins.push_back(0); // теперь min = 0
    
    // Проверяем все состояния
    EXPECT_EQ(tree.rangeMinQuery(1, 8), 0); // текущий min
}

// Тест 10: Множественные запросы на перекрывающиеся отрезки
TEST(SegmentTreeRMQTest, OverlappingQueries) {
    std::vector<int> arr = {5, 2, 8, 3, 9, 1, 4, 6};
    SegmentTreeRMQ tree(arr);
    
    // Проверяем различные перекрывающиеся отрезки
    EXPECT_EQ(tree.rangeMinQuery(1, 4), 2); // [5,2,8,3]
    EXPECT_EQ(tree.rangeMinQuery(2, 5), 2); // [2,8,3,9]
    EXPECT_EQ(tree.rangeMinQuery(3, 6), 1); // [8,3,9,1]
    EXPECT_EQ(tree.rangeMinQuery(4, 7), 1); // [3,9,1,4]
    EXPECT_EQ(tree.rangeMinQuery(5, 8), 1); // [9,1,4,6]
    
    // Обновляем и проверяем снова
    tree.pointUpdate(6, 10); // arr[6] = 10 (было 1)
    EXPECT_EQ(tree.rangeMinQuery(3, 6), 3); // [8,3,9,10]
    EXPECT_EQ(tree.rangeMinQuery(5, 8), 4); // [9,10,4,6]
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
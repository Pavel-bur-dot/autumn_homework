#ifndef TOPOLOGICAL_SORTER_HPP
#define TOPOLOGICAL_SORTER_HPP

#include <vector>

class TopologicalSorter {
private:
    int vertices_count;
    std::vector<std::vector<int>> graph;
    
    // Состояния для DFS: 0 - White, 1 - Gray, 2 - Black
    std::vector<int> visited;
    std::vector<int> result;
    bool has_cycle;
    
    // DFS с обнаружением циклов
    void dfs(int v);
    
public:
    TopologicalSorter(int n);
    
    void addEdge(int from, int to);
    
    // Основной метод: выполняет топологическую сортировку
    // Возвращает true, если сортировка возможна (нет циклов)
    // false, если есть цикл
    bool sort();
    
    // Получить результат сортировки (только если sort() вернул true)
    const std::vector<int>& getOrder() const;
    
    // Статический метод для полного решения задачи
    static void solveTopologicalSort();
};

#endif // TOPOLOGICAL_SORTER_HPP
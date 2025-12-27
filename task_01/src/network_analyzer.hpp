#ifndef NETWORK_ANALYZER_HPP
#define NETWORK_ANALYZER_HPP

#include "graph.hpp"
#include <vector>
#include <utility>
#include <set>

class NetworkAnalyzer : public Graph {
private:
    // DFS для поиска критических элементов
    void dfsCritical(
        int vertex, 
        int parent, 
        std::vector<int>& tin, 
        std::vector<int>& low,
        std::vector<Color>& colors,
        int& timer,
        std::set<int>& articulation_points,
        std::vector<std::pair<int, int>>& bridges
    ) const;
    
public:
    NetworkAnalyzer(int n);
    
    // Основной метод для поиска критических элементов
    std::pair<std::vector<int>, std::vector<std::pair<int, int>>> 
    findCriticalElements() const;
    
    // Статический метод для решения задачи (полный ввод/вывод)
    static void solveNetworkProblem();
};

#endif // NETWORK_ANALYZER_HPP
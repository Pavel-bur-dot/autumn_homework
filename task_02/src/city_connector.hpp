#ifndef CITY_CONNECTOR_HPP
#define CITY_CONNECTOR_HPP

#include <vector>

class CityConnector {
private:
    int vertices_count;
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> reversed_graph;
    
    // Для алгоритма Косарайю
    std::vector<bool> visited;
    std::vector<int> order;
    std::vector<int> component;
    
    // Для конденсации графа
    std::vector<int> comp_id;
    int components_count;
    
    // Вспомогательные методы
    void dfsFirst(int v);
    void dfsSecond(int v);
    void buildCondensedGraph();
    
public:
    CityConnector(int n);
    
    void addRoad(int from, int to);
    
    // Основной метод: находит минимальное количество дорог для соединения
    int findMinRoadsToConnect();
    
    // Статический метод для полного решения задачи
    static void solveCityProblem();
};

#endif // CITY_CONNECTOR_HPP
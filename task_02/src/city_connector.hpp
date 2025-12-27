#ifndef CITY_CONNECTOR_HPP
#define CITY_CONNECTOR_HPP

#include <vector>

class CityConnector {
private:
    int vertices_count;
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> reversed_graph;
    
    std::vector<bool> visited;
    std::vector<int> order;
    std::vector<int> component;
    
    std::vector<int> comp_id;
    int components_count;
    
    void dfsFirst(int v);
    void dfsSecond(int v);
    void buildCondensedGraph();
    
public:
    CityConnector(int n);
    
    void addRoad(int from, int to);
    
    int findMinRoadsToConnect();
    
    static void solveCityProblem();
};

#endif
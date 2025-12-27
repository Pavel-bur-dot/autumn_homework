#include "city_connector.hpp"
#include <iostream>
#include <algorithm>

CityConnector::CityConnector(int n) 
    : vertices_count(n), 
      components_count(0) {
    
    graph.resize(n);
    reversed_graph.resize(n);
    visited.assign(n, false);
    comp_id.assign(n, -1);
}

void CityConnector::addRoad(int from, int to) {
    int u = from - 1;
    int v = to - 1;
    
    graph[u].push_back(v);
    reversed_graph[v].push_back(u);
}

void CityConnector::dfsFirst(int v) {
    visited[v] = true;
    
    for (int to : graph[v]) {
        if (!visited[to]) {
            dfsFirst(to);
        }
    }
    
    order.push_back(v);
}

void CityConnector::dfsSecond(int v) {
    visited[v] = true;
    component.push_back(v);
    comp_id[v] = components_count;
    
    for (int to : reversed_graph[v]) {
        if (!visited[to]) {
            dfsSecond(to);
        }
    }
}

void CityConnector::buildCondensedGraph() {
    visited.assign(vertices_count, false);
    order.clear();
    
    for (int v = 0; v < vertices_count; ++v) {
        if (!visited[v]) {
            dfsFirst(v);
        }
    }
    
    visited.assign(vertices_count, false);
    std::reverse(order.begin(), order.end());
    components_count = 0;
    
    for (int v : order) {
        if (!visited[v]) {
            component.clear();
            dfsSecond(v);
            components_count++;
        }
    }
}

int CityConnector::findMinRoadsToConnect() {
    if (vertices_count <= 1) {
        return 0;
    }
    
    buildCondensedGraph();
    
    if (components_count == 1) {
        return 0;
    }
    
    std::vector<int> in_degree(components_count, 0);
    std::vector<int> out_degree(components_count, 0);
    
    for (int u = 0; u < vertices_count; ++u) {
        for (int v : graph[u]) {
            if (comp_id[u] != comp_id[v]) {
                out_degree[comp_id[u]]++;
                in_degree[comp_id[v]]++;
            }
        }
    }
    
    int sources = 0, sinks = 0;
    for (int i = 0; i < components_count; ++i) {
        if (in_degree[i] == 0) sources++;
        if (out_degree[i] == 0) sinks++;
    }
    
    return std::max(sources, sinks);
}

void CityConnector::solveCityProblem() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    CityConnector connector(n);
    
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        connector.addRoad(a, b);
    }
    
    int result = connector.findMinRoadsToConnect();
    std::cout << result << "\n";
}
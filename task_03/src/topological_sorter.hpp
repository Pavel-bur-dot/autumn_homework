#ifndef TOPOLOGICAL_SORTER_HPP
#define TOPOLOGICAL_SORTER_HPP

#include <vector>

class TopologicalSorter {
private:
    int vertices_count;
    std::vector<std::vector<int>> graph;
    
    std::vector<int> visited;
    std::vector<int> result;
    bool has_cycle;
    
    void dfs(int v);
    
public:
    TopologicalSorter(int n);
    
    void addEdge(int from, int to);

    bool sort();

    const std::vector<int>& getOrder() const;
    
    static void solveTopologicalSort();
};

#endif
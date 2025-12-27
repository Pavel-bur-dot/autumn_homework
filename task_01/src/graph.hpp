#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

enum class Color {
    White,
    Gray,
    Black
};

class Graph {
protected:
    int vertices_count;
    std::vector<std::vector<int>> adjacency_list;
    
public:
    Graph(int n);
    virtual ~Graph() = default;
    
    void addEdge(int u, int v);
    const std::vector<int>& getNeighbors(int v) const;
    int getVerticesCount() const;
    const std::vector<std::vector<int>>& getAdjacencyList() const;
    
    void sortAdjacencyLists();
};

#endif
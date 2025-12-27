#ifndef LIMITED_DEGREE_MST_HPP
#define LIMITED_DEGREE_MST_HPP

#include <vector>

class LimitedDegreeMST {
private:
    int vertices_count;
    int max_degree;
    
    struct Edge {
        int to;
        int weight;
        Edge(int t, int w) : to(t), weight(w) {}
    };
    
    std::vector<std::vector<Edge>> graph;
    
public:
    LimitedDegreeMST(int n, int d);

    void addEdge(int u, int v, int weight);
    
    int findLimitedDegreeMST();

    static void solveLimitedDegreeMST();
};

#endif
#ifndef JOHNSON_ALGORITHM_HPP
#define JOHNSON_ALGORITHM_HPP

#include <vector>
#include <limits>

class JohnsonAlgorithm {
private:
    int vertices_count;

    struct Edge {
        int from;
        int to;
        long long weight;
    };
    
    std::vector<Edge> edges;
    
    bool bellmanFord(int source, std::vector<long long>& distances) const;
    
    void dijkstra(int source, const std::vector<long long>& h, 
                  std::vector<long long>& distances) const;
    
public:
    JohnsonAlgorithm(int n);
    
    void addEdge(int from, int to, long long weight);
    
    std::vector<std::vector<long long>> findAllShortestPaths();
    
    static void solveJohnsonAlgorithm();
    
    static constexpr long long INF = std::numeric_limits<long long>::max() / 2;
};

#endif
#ifndef MAX_FLOW_SOLVER_HPP
#define MAX_FLOW_SOLVER_HPP

#include <vector>

class MaxFlowSolver {
private:
    int vertices_count;
    
    struct Edge {
        int to;
        int capacity;
        int flow;
        int rev;
    };
    
    std::vector<std::vector<Edge>> graph;
    std::vector<int> level;
    std::vector<int> ptr;
    
    bool bfs(int source, int sink);
    
    int dfs(int v, int sink, int flow);
    
public:
    MaxFlowSolver(int n);
    
    void addEdge(int from, int to, int capacity);
    
    int findMaxFlow(int source, int sink);
    
    static void solveMaxFlow();
};

#endif
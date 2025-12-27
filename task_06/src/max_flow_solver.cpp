#include "max_flow_solver.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

MaxFlowSolver::MaxFlowSolver(int n) 
    : vertices_count(n) {
    
    graph.resize(n);
    level.resize(n);
    ptr.resize(n);
}

void MaxFlowSolver::addEdge(int from, int to, int capacity) {
    int u = from - 1;
    int v = to - 1;
    
    Edge forward = {v, capacity, 0, (int)graph[v].size()};
    Edge backward = {u, 0, 0, (int)graph[u].size()};
    
    graph[u].push_back(forward);
    graph[v].push_back(backward);
}

bool MaxFlowSolver::bfs(int source, int sink) {
    std::fill(level.begin(), level.end(), -1);
    std::queue<int> q;
    
    level[source] = 0;
    q.push(source);
    
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        
        for (const Edge& edge : graph[v]) {
            if (level[edge.to] < 0 && edge.flow < edge.capacity) {
                level[edge.to] = level[v] + 1;
                q.push(edge.to);
            }
        }
    }
    
    return level[sink] >= 0;
}

int MaxFlowSolver::dfs(int v, int sink, int flow) {
    if (v == sink || flow == 0) {
        return flow;
    }
    
    for (int& i = ptr[v]; i < (int)graph[v].size(); ++i) {
        Edge& edge = graph[v][i];
        
        if (level[edge.to] == level[v] + 1 && edge.flow < edge.capacity) {
            int pushed = dfs(edge.to, sink, std::min(flow, edge.capacity - edge.flow));
            
            if (pushed > 0) {
                edge.flow += pushed;
                graph[edge.to][edge.rev].flow -= pushed;
                return pushed;
            }
        }
    }
    
    return 0;
}

int MaxFlowSolver::findMaxFlow(int source, int sink) {
    if (source == sink) return 0;
    
    int s = source - 1;
    int t = sink - 1;
    
    int max_flow = 0;
    
    while (bfs(s, t)) {
        std::fill(ptr.begin(), ptr.end(), 0);
        
        while (int pushed = dfs(s, t, std::numeric_limits<int>::max())) {
            max_flow += pushed;
        }
    }
    
    return max_flow;
}

void MaxFlowSolver::solveMaxFlow() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    MaxFlowSolver solver(n);
    
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        std::cin >> u >> v >> c;
        solver.addEdge(u, v, c);
    }
    
    int result = solver.findMaxFlow(1, n);
    std::cout << result << "\n";
}
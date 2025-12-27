#ifndef LCA_SOLVER_HPP
#define LCA_SOLVER_HPP

#include <vector>
#include <cmath>

class LCASolver {
private:
    int n;
    std::vector<std::vector<int>> graph;
    
    std::vector<int> euler_tour;
    std::vector<int> depth;
    std::vector<int> first;
    std::vector<int> parent;
    
    std::vector<std::vector<int>> sparse_table;
    std::vector<int> log_table;
    
    void dfs(int u, int p, int d);
    void buildSparseTable();
    int rmq(int l, int r) const;
    
public:
    LCASolver(int vertices);
    
    void addEdge(int u, int v);
    
    void build(int root = 1);
    
    int findLCA(int u, int v) const;
    
    static void solveLCA();
};

#endif
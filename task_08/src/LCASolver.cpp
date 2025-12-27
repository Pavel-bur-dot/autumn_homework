#include "LCASolver.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

LCASolver::LCASolver(int vertices) : n(vertices) {
    graph.resize(n + 1);
    parent.resize(n + 1, 0);
    first.resize(n + 1, -1);
}

void LCASolver::addEdge(int u, int v) {
    graph[u].push_back(v);
    graph[v].push_back(u);
}

void LCASolver::dfs(int u, int p, int d) {
    parent[u] = p;
    first[u] = euler_tour.size();
    euler_tour.push_back(u);
    depth.push_back(d);
    
    for (int v : graph[u]) {
        if (v != p) {
            dfs(v, u, d + 1);
            euler_tour.push_back(u);
            depth.push_back(d);
        }
    }
}

void LCASolver::buildSparseTable() {
    int m = depth.size();

    log_table.resize(m + 1);
    log_table[1] = 0;
    for (int i = 2; i <= m; ++i) {
        log_table[i] = log_table[i / 2] + 1;
    }
    
    int k = log_table[m] + 1;
    sparse_table.assign(k, std::vector<int>(m));
    
    for (int i = 0; i < m; ++i) {
        sparse_table[0][i] = i;
    }
    
    for (int j = 1; j < k; ++j) {
        for (int i = 0; i + (1 << j) <= m; ++i) {
            int left = sparse_table[j - 1][i];
            int right = sparse_table[j - 1][i + (1 << (j - 1))];
            
            if (depth[left] < depth[right]) {
                sparse_table[j][i] = left;
            } else {
                sparse_table[j][i] = right;
            }
        }
    }
}

int LCASolver::rmq(int l, int r) const {
    if (l > r) std::swap(l, r);
    int j = log_table[r - l + 1];
    
    int left = sparse_table[j][l];
    int right = sparse_table[j][r - (1 << j) + 1];
    
    return (depth[left] < depth[right]) ? left : right;
}

void LCASolver::build(int root) {
    euler_tour.clear();
    depth.clear();
    dfs(root, 0, 0);
    buildSparseTable();
}

int LCASolver::findLCA(int u, int v) const {
    if (first[u] > first[v]) {
        std::swap(u, v);
    }
    
    int idx = rmq(first[u], first[v]);
    return euler_tour[idx];
}

void LCASolver::solveLCA() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    LCASolver solver(n);
    
    for (int i = 2; i <= n; ++i) {
        int parent;
        std::cin >> parent;
        solver.addEdge(i, parent);
    }
    
    solver.build(1);
    
    for (int i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;
        std::cout << solver.findLCA(u, v) << "\n";
    }
}
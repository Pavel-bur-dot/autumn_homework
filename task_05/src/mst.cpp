#include "mst.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <functional>

LimitedDegreeMST::LimitedDegreeMST(int n, int d) 
    : vertices_count(n), max_degree(d) {
    graph.resize(n);
}

void LimitedDegreeMST::addEdge(int u, int v, int weight) {
    int u0 = u - 1;
    int v0 = v - 1;
    graph[u0].push_back(Edge(v0, weight));
    graph[v0].push_back(Edge(u0, weight));
}

int LimitedDegreeMST::findLimitedDegreeMST() {
    if (vertices_count <= 1) return 0;
    
    int n = vertices_count;
    int d = max_degree;

    if (d < 1 || (n > 1 && d == 0)) {
        return -1;
    }

    std::vector<std::tuple<int, int, int>> all_edges; // вес, u, v
    for (int u = 0; u < n; ++u) {
        for (const auto& edge : graph[u]) {
            if (u < edge.to) { // Каждое ребро один раз
                all_edges.push_back({edge.weight, u, edge.to});
            }
        }
    }

    std::sort(all_edges.begin(), all_edges.end());

    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    std::vector<int> degree(n, 0);
    
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    std::function<int(int)> find = [&](int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };
    
    auto unite = [&](int x, int y) -> bool {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return false;
        
        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            rank[rootX]++;
        }
        return true;
    };
    
    int mst_weight = 0;
    int edges_added = 0;
    
    for (const auto& edge : all_edges) {
        int weight = std::get<0>(edge);
        int u = std::get<1>(edge);
        int v = std::get<2>(edge);

        if (degree[u] < d && degree[v] < d) {
            // Проверяем, не создаст ли цикл
            if (find(u) != find(v)) {
                // Добавляем ребро
                if (unite(u, v)) {
                    degree[u]++;
                    degree[v]++;
                    mst_weight += weight;
                    edges_added++;
                    
                    if (edges_added == n - 1) {
                        break;
                    }
                }
            }
        }
    }

    if (edges_added != n - 1) {
        return -1;
    }
    
    return mst_weight;
}

void LimitedDegreeMST::solveLimitedDegreeMST() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m, d;
    std::cin >> n >> m >> d;
    
    LimitedDegreeMST solver(n, d);
    
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        std::cin >> u >> v >> w;
        solver.addEdge(u, v, w);
    }
    
    int result = solver.findLimitedDegreeMST();
    
    if (result == -1) {
        std::cout << "Невозможно построить остовное дерево с заданным ограничением степени\n";
    } else {
        std::cout << result << "\n";
    }
}
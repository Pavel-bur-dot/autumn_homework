#include "johnson_algorithm.hpp"
#include <iostream>
#include <vector>
#include <queue>

JohnsonAlgorithm::JohnsonAlgorithm(int n) 
    : vertices_count(n) {
}

void JohnsonAlgorithm::addEdge(int from, int to, long long weight) {
    edges.push_back({from - 1, to - 1, weight});
}

bool JohnsonAlgorithm::bellmanFord(int source, std::vector<long long>& distances) const {
    int n = vertices_count + 1;
    distances.assign(n, INF);
    distances[source] = 0;
    
    for (int i = 0; i < n - 1; ++i) {
        bool updated = false;
        for (const auto& edge : edges) {
            if (distances[edge.from] < INF && 
                distances[edge.to] > distances[edge.from] + edge.weight) {
                distances[edge.to] = distances[edge.from] + edge.weight;
                updated = true;
            }
        }
        if (!updated) break;
    }
    
    for (const auto& edge : edges) {
        if (distances[edge.from] < INF && 
            distances[edge.to] > distances[edge.from] + edge.weight) {
            return false;
        }
    }
    
    return true;
}

void JohnsonAlgorithm::dijkstra(int source, const std::vector<long long>& h,
                               std::vector<long long>& distances) const {
    int n = vertices_count;
    distances.assign(n, INF);
    distances[source] = 0;
    
    using Pair = std::pair<long long, int>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pq;
    pq.push({0, source});
    
    std::vector<std::vector<Pair>> adj(n);
    for (const auto& edge : edges) {
        if (h[edge.from] < INF && h[edge.to] < INF) {
            long long new_weight = edge.weight + h[edge.from] - h[edge.to];
            adj[edge.from].push_back({new_weight, edge.to});
        }
    }
    
    while (!pq.empty()) {
        auto [current_dist, u] = pq.top();
        pq.pop();
        
        if (current_dist > distances[u]) continue;
        
        for (const auto& [weight, v] : adj[u]) {
            long long new_dist = current_dist + weight;
            if (new_dist < distances[v]) {
                distances[v] = new_dist;
                pq.push({new_dist, v});
            }
        }
    }
}

std::vector<std::vector<long long>> JohnsonAlgorithm::findAllShortestPaths() {
    int n = vertices_count;
    
    int dummy_vertex = n;
    
    auto original_edges = edges;
    
    for (int v = 0; v < n; ++v) {
        edges.push_back({dummy_vertex, v, 0});
    }
    
    std::vector<long long> h(n + 1, 0); // Потенциалы (+1 для фиктивной)
    bool has_negative_cycle = !bellmanFord(dummy_vertex, h);
    
    edges = original_edges;
    
    if (has_negative_cycle) {
        return {};
    }
    
    h.pop_back();
    
    std::vector<std::vector<long long>> result(n, std::vector<long long>(n, INF));
    
    for (int u = 0; u < n; ++u) {
        std::vector<long long> dist;
        dijkstra(u, h, dist);
        
        for (int v = 0; v < n; ++v) {
            if (dist[v] < INF) {
                if (h[u] < INF && h[v] < INF && dist[v] < INF) {
                    result[u][v] = dist[v] - h[u] + h[v];
                }
            }
        }
        result[u][u] = 0;
    }
    
    return result;
}

void JohnsonAlgorithm::solveJohnsonAlgorithm() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    JohnsonAlgorithm solver(n);
    
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long w;
        std::cin >> u >> v >> w;
        solver.addEdge(u, v, w);
    }
    
    auto distances = solver.findAllShortestPaths();
    
    if (distances.empty()) {
        std::cout << "-1\n";
        return;
    }
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j > 0) std::cout << " ";
            if (distances[i][j] >= INF / 2) {
                std::cout << "INF";
            } else {
                std::cout << distances[i][j];
            }
        }
        std::cout << "\n";
    }
}
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using Graph = std::vector<std::vector<int>>;

enum Color {
    White,
    Black
};

int timer;
std::vector<int> tin, low;
std::vector<Color> colors;
std::set<int> critical_devices;
std::vector<std::pair<int, int>> critical_connections;

void find_critical(const Graph& greph, int vertex, int parent = -1) {
    colors[vertex] = Color::Black;
    tin[vertex] = low[vertex] = timer++;
    int child_count = 0;
    
    for (int neighbor : greph[vertex]) {
        if (neighbor == parent) continue;
        
        switch (colors[neighbor]) {
        case Color::White: {
            find_critical(greph, neighbor, vertex);
            low[vertex] = std::min(low[vertex], low[neighbor]);
            
            if (low[neighbor] > tin[vertex]) {
                critical_connections.push_back({
                    std::min(vertex, neighbor), 
                    std::max(vertex, neighbor)
                });
            }
            
            if (low[neighbor] >= tin[vertex] && parent != -1) {
                critical_devices.insert(vertex);
            }
            
            child_count++;
            break;
        }
        case Color::Black: {
            low[vertex] = std::min(low[vertex], tin[neighbor]);
            break;
        }
        }
    }
    
    if (parent == -1 && child_count > 1) {
        critical_devices.insert(vertex);
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int routers_count, cables_count;
    std::cin >> routers_count >> cables_count;
    
    Graph greph(routers_count + 1);
    
    for (int i = 0; i < cables_count; ++i) {
        int router1, router2;
        std::cin >> router1 >> router2;
        greph[router1].push_back(router2);
        greph[router2].push_back(router1);
    }
    
    for (int v = 1; v <= routers_count; ++v) {
        std::sort(greph[v].begin(), greph[v].end());
    }
    
    colors.assign(routers_count + 1, Color::White);
    tin.assign(routers_count + 1, 0);
    low.assign(routers_count + 1, 0);
    timer = 0;
    
    for (int v = 1; v <= routers_count; ++v) {
        if (colors[v] == Color::White) {
            find_critical(greph, v);
        }
    }
    
    // Вывод результатов
    std::cout << critical_devices.size() << "\n";
    
    if (critical_devices.empty()) {
        std::cout << "-\n";
    } else {
        bool first = true;
        for (int device : critical_devices) {
            if (!first) std::cout << " ";
            std::cout << device;
            first = false;
        }
        std::cout << "\n";
    }
    
    std::cout << critical_connections.size() << "\n";
    
    if (critical_connections.empty()) {
        std::cout << "-\n";
    } else {
        std::sort(critical_connections.begin(), critical_connections.end());
        
        for (size_t i = 0; i < critical_connections.size(); ++i) {
            if (i > 0) std::cout << "; ";
            std::cout << critical_connections[i].first 
                      << " " 
                      << critical_connections[i].second;
        }
        std::cout << "\n";
    }
    
    return 0;
}
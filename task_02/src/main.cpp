#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using Graph = std::vector<std::vector<int>>;

enum Color {
    White,
    Gray,
    Black
};

class CityConnector {
private:
    int n;
    Graph graph;
    std::vector<Color> colors;
    std::vector<int> tin, low, comp_id;
    std::vector<bool> in_stack;
    std::stack<int> stack;
    int timer;
    int scc_count;
    
    void dfs(int v) {
        colors[v] = Color::Gray;
        tin[v] = low[v] = timer++;
        stack.push(v);
        in_stack[v] = true;
        
        for (int to : graph[v]) {
            switch (colors[to]) {
                case Color::White:
                    dfs(to);
                    low[v] = std::min(low[v], low[to]);
                    break;
                case Color::Gray:
                    low[v] = std::min(low[v], tin[to]);
                    break;
            }
        }
        
        if (low[v] == tin[v]) {
            // Новая сильно связная компонента
            while (true) {
                int u = stack.top();
                stack.pop();
                in_stack[u] = false;
                comp_id[u] = scc_count;
                if (u == v) break;
            }
            scc_count++;
        }
        
        colors[v] = Color::Black;
    }
    
public:
    CityConnector(int vertices) : n(vertices) {
        graph.resize(n);
        colors.assign(n, Color::White);
        tin.assign(n, 0);
        low.assign(n, 0);
        comp_id.assign(n, -1);
        in_stack.assign(n, false);
        timer = 0;
        scc_count = 0;
    }
    
    void addRoad(int from, int to) {
        graph[from].push_back(to);
    }
    
    int findMinRoadsToConnect() {
        // 1. Находим все SCC
        for (int v = 0; v < n; ++v) {
            if (colors[v] == Color::White) {
                dfs(v);
            }
        }
        
        // Если весь граф уже сильно связный
        if (scc_count == 1) {
            return 0;
        }
        
        // 2. Строим граф компонент
        std::vector<int> in_degree(scc_count, 0);
        std::vector<int> out_degree(scc_count, 0);
        
        for (int v = 0; v < n; ++v) {
            for (int to : graph[v]) {
                if (comp_id[v] != comp_id[to]) {
                    out_degree[comp_id[v]]++;
                    in_degree[comp_id[to]]++;
                }
            }
        }
        
        // 3. Считаем истоки и стоки
        int sources = 0, sinks = 0;
        for (int i = 0; i < scc_count; ++i) {
            if (in_degree[i] == 0) sources++;
            if (out_degree[i] == 0) sinks++;
        }
        
        // 4. Ответ
        return std::max(sources, sinks);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n, m;
    std::cin >> n >> m;
    
    CityConnector connector(n);
    
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        // Приводим к 0-based индексации
        connector.addRoad(a - 1, b - 1);
    }
    
    int result = connector.findMinRoadsToConnect();
    std::cout << result << "\n";
    
    return 0;
}
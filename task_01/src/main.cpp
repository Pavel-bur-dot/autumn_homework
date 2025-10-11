#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using Edge = std::pair<int, int>;

static void depthFirstSearch(int currentVertex, int parentVertex,
                             const std::vector<std::vector<int>> &adjacencyList,
                             std::vector<int> &discoveryTime,
                             std::vector<int> &lowLinkValue, int &globalTimer,
                             std::vector<bool> &isArticulationPoint,
                             std::vector<Edge> &bridges) {
  discoveryTime[currentVertex] = lowLinkValue[currentVertex] = ++globalTimer;
  int numChildrenInDfsTree = 0;

  for (int neighborVertex : adjacencyList[currentVertex]) {
    if (neighborVertex == parentVertex)
      continue;
    if (discoveryTime[neighborVertex] != 0) {
      lowLinkValue[currentVertex] =
          std::min(lowLinkValue[currentVertex], discoveryTime[neighborVertex]);
    } else {
      ++numChildrenInDfsTree;
      depthFirstSearch(neighborVertex, currentVertex, adjacencyList,
                       discoveryTime, lowLinkValue, globalTimer,
                       isArticulationPoint, bridges);
      lowLinkValue[currentVertex] =
          std::min(lowLinkValue[currentVertex], lowLinkValue[neighborVertex]);

      if (lowLinkValue[neighborVertex] > discoveryTime[currentVertex]) {
        int u = std::min(currentVertex, neighborVertex);
        int v = std::max(currentVertex, neighborVertex);
        bridges.emplace_back(u, v);
      }

      if (parentVertex != -1 &&
          lowLinkValue[neighborVertex] >= discoveryTime[currentVertex]) {
        isArticulationPoint[currentVertex] = true;
      }
    }
  }

  if (parentVertex == -1 && numChildrenInDfsTree > 1) {
    isArticulationPoint[currentVertex] = true;
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int numVertices = 0, numEdges = 0;
  if (!(std::cin >> numVertices >> numEdges)) {
    return 0;
  }

  std::vector<std::vector<int>> adjacencyList(numVertices + 1);
  for (int i = 0; i < numEdges; ++i) {
    int u = 0, v = 0;
    std::cin >> u >> v;
    if (u < 1 || u > numVertices || v < 1 || v > numVertices)
      continue;
    if (u == v)
      continue; // ignore self-loops for criticality purposes
    adjacencyList[u].push_back(v);
    adjacencyList[v].push_back(u);
  }

  for (int vertex = 1; vertex <= numVertices; ++vertex) {
    std::sort(adjacencyList[vertex].begin(), adjacencyList[vertex].end());
    adjacencyList[vertex].erase(
        std::unique(adjacencyList[vertex].begin(), adjacencyList[vertex].end()),
        adjacencyList[vertex].end());
  }

  std::vector<int> discoveryTime(numVertices + 1, 0);
  std::vector<int> lowLinkValue(numVertices + 1, 0);
  std::vector<bool> isArticulationPoint(numVertices + 1, false);
  std::vector<Edge> bridges;
  int globalTimer = 0;

  for (int vertex = 1; vertex <= numVertices; ++vertex) {
    if (discoveryTime[vertex] == 0) {
      depthFirstSearch(vertex, -1, adjacencyList, discoveryTime, lowLinkValue,
                       globalTimer, isArticulationPoint, bridges);
    }
  }

  std::vector<int> articulationVertices;
  articulationVertices.reserve(numVertices);
  for (int vertex = 1; vertex <= numVertices; ++vertex) {
    if (isArticulationPoint[vertex])
      articulationVertices.push_back(vertex);
  }
  std::sort(articulationVertices.begin(), articulationVertices.end());

  std::sort(bridges.begin(), bridges.end());
  bridges.erase(std::unique(bridges.begin(), bridges.end()), bridges.end());

  std::cout << static_cast<int>(articulationVertices.size()) << '\n';
  if (articulationVertices.empty()) {
    std::cout << "-\n";
  } else {
    for (std::size_t i = 0; i < articulationVertices.size(); ++i) {
      if (i)
        std::cout << ' ';
      std::cout << articulationVertices[i];
    }
    std::cout << '\n';
  }

  std::cout << static_cast<int>(bridges.size()) << '\n';
  if (bridges.empty()) {
    std::cout << "-\n";
  } else {
    for (std::size_t i = 0; i < bridges.size(); ++i) {
      if (i)
        std::cout << "; ";
      std::cout << bridges[i].first << ' ' << bridges[i].second;
    }
    std::cout << '\n';
  }

  return 0;
}

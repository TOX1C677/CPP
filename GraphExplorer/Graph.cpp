#include "pch.h"
#include "Graph.h"
#include "Logger.h"
#include <queue>
#include <set>
#include <fstream>
#include <sstream>

bool Graph::LoadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return false;

    adjList.clear();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int vertex;
        iss >> vertex;
        int neighbor;
        while (iss >> neighbor) {
            adjList[vertex].push_back(neighbor);
            adjList[neighbor].push_back(vertex); // Неориентированный граф
        }
    }

    Logger::GetInstance().LogInfo("Граф успешно загружен из файла: " + path);
    return true;
}

std::map<int, std::vector<int>> Graph::GetGraph() const {
    return adjList;
}

std::vector<std::vector<int>> Graph::BFS(int startVertex) const {
    std::vector<std::vector<int>> layers;
    if (adjList.find(startVertex) == adjList.end()) {
        throw std::runtime_error("Начальная вершина не найдена в графе");
    }

    std::queue<int> q;
    std::set<int> visited;

    q.push(startVertex);
    visited.insert(startVertex);
    layers.push_back({ startVertex });

    while (!q.empty()) {
        size_t levelSize = q.size();
        std::vector<int> currentLevel;

        for (size_t i = 0; i < levelSize; ++i) {
            int current = q.front();
            q.pop();

            for (int neighbor : adjList.at(current)) {
                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    q.push(neighbor);
                    currentLevel.push_back(neighbor);
                }
            }
        }

        if (!currentLevel.empty()) {
            layers.push_back(currentLevel);
        }
    }

    Logger::GetInstance().LogInfo("Обход графа (BFS) завершен");
    return layers;
}
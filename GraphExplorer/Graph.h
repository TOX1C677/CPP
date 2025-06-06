#pragma once
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Graph {
public:
    bool LoadFromFile(const std::string& path);
    std::map<int, std::vector<int>> GetGraph() const;
    std::vector<std::vector<int>> BFS(int startVertex) const;  // Измененная сигнатура

private:
    std::map<int, std::vector<int>> adjList;
};
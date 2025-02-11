#include "Map.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // Initialize all distances to a value representing no direct connection
    distanceMatrix = std::vector<std::vector<int>>(MAX_SIZE, std::vector<int>(MAX_SIZE, -1));

    // Initialize all provinces as unvisited
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open the file: " + filename);
    }

    std::string line;
    for (int row = 0; row < MAX_SIZE && std::getline(file, line); ++row) {
        std::stringstream lineStream(line);
        std::string cell;

        for (int col = 0; col < MAX_SIZE && std::getline(lineStream, cell, ','); ++col) {
            if (!cell.empty()) {
                distanceMatrix[row][col] = std::stoi(cell);
            }
        }
    }

    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    return distanceMatrix[provinceA][provinceB] != -1 && distanceMatrix[provinceA][provinceB] <= maxDistance;
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    if (province >= 0 && province < MAX_SIZE) {
        visited[province] = true;
    } else {
        return;
    }
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    if (province >= 0 && province < MAX_SIZE) {
        return visited[province];
    }
    else {
        throw std::out_of_range("Province index out of range");
    }
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    int count = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (visited[i]) {
            ++count;
        }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    if(provinceA >= 0 && provinceA < MAX_SIZE && provinceB >= 0 && provinceB < MAX_SIZE) {
        return distanceMatrix[provinceA][provinceB];
    }
    else {
        throw std::out_of_range("Province index out of range");
    }
}
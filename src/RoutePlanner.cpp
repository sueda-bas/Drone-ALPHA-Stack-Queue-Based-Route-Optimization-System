#include "RoutePlanner.h"
#include <iostream>
#include <fstream>
#include "Map.h"
#include "Stack.h"
#include "Queue.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <regex>

// Array to help you out with name of the cities in order
const std::string cities[81] = { 
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik", 
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne", 
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta", 
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya", "Kutahya", 
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya", 
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat", 
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir", 
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce" 
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string& distance_data, const std::string& priority_data, const std::string& restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {

    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially
    // Load priority provinces
    // Load restricted provinces
    map.loadDistanceData(distance_data);
    map.resetVisited();
    for (int i = 0; i < MAX_PRIORITY_PROVINCES; ++i) {
        priorityProvinces[i] = -1;
    }
    for (int i = 0; i < MAX_WEATHER_RESTRICTED_PROVINCES; ++i) {
        weatherRestrictedProvinces[i] = -1;
    }
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);

}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the priority provinces file: " << filename << std::endl;
        return;
    }
    numPriorityProvinces = 0;
    std::string code = "";

    char ch;
    while (file.get(ch)) {
        if (std::isdigit(ch)) {
            code += ch;
        } else if (!code.empty()) {
            int provinceCode = std::stoi(code);
            if (provinceCode >= 0 && provinceCode < MAX_SIZE) {
                priorityProvinces[numPriorityProvinces] = provinceCode;
                numPriorityProvinces++;
            }
            code = "";
        }
    }
    if (!code.empty()) {
        int provinceCode = std::stoi(code);
        if (provinceCode >= 0 && provinceCode < MAX_SIZE) {
            priorityProvinces[numPriorityProvinces] = provinceCode;
            numPriorityProvinces++;
        }
    }
    file.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the weather-restricted provinces file: " << filename << std::endl;
        return;
    }
    std::string code = "";
    numWeatherRestrictedProvinces = 0;
    char ch;
    while (file.get(ch)) {
        if (std::isdigit(ch)) {
            code += ch;
        } else if (!code.empty()) {
            int restricted = std::stoi(code);
            if (restricted >= 0 && restricted < MAX_SIZE) {
                weatherRestrictedProvinces[numWeatherRestrictedProvinces] = restricted;
                numWeatherRestrictedProvinces++;
            }
            code = "";
        }
    }
    if (!code.empty()) {
        int restricted = std::stoi(code);
        if (restricted >= 0 && restricted < MAX_SIZE) {
            weatherRestrictedProvinces[numWeatherRestrictedProvinces] = restricted;
            numWeatherRestrictedProvinces++;
        }
    }

    file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    for(int priorityProvince : priorityProvinces) {
        if(priorityProvince == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    for(int weatherRestricted : weatherRestrictedProvinces) {
        if(weatherRestricted == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    stack.push(startingCity);
    queue.enqueue(startingCity);
    map.markAsVisited(startingCity);
    route.push_back(startingCity);
    while (!isExplorationComplete()) {
        if (!queue.isEmpty()) {
            int currentProvince = queue.dequeue();
            if (!map.isVisited(currentProvince)) {
                totalDistanceCovered += map.getDistance(route.back(), currentProvince);
                stack.push(currentProvince);
                route.push_back(currentProvince);
                map.markAsVisited(currentProvince);
            }
            exploreFromProvince(currentProvince);
        } else if (!stack.isEmpty()) {
            backtrack();
        }
    }
    displayResults();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    enqueueNeighbors(province);
    if (queue.isEmpty()) {
        backtrack();
    }
}

void RoutePlanner::enqueueNeighbors(int province) {
    int numProvinces = MAX_SIZE;
    for (int j = 0; j < numProvinces; j++) {
        if (map.getDistance(province, j) <= maxDistance) {
            if (!map.isVisited(j) && !isWeatherRestricted(j)) {

                if (isPriorityProvince(j)) {
                        queue.enqueuePriority(j);
                } else {
                        queue.enqueue(j);
                }
            } else if (isWeatherRestricted(j)) {
                std::cout << "Province " << cities[j] << " is weather-restricted. Skipping." << std::endl;
                }
            }
        }
    }


void RoutePlanner::backtrack() {
    // If you reach a dead-end province
    int previousProvince = stack.pop();
    bool hasUnvisitedNeighbors = false;
    int j = 0;
    while (j < MAX_SIZE) {
        if(map.distanceMatrix[previousProvince][j] <= maxDistance) {
            if(!map.isVisited(j) && !isWeatherRestricted(j)) {
                hasUnvisitedNeighbors = true;
                break;
            }
        }
        j ++;
    }
    if(!hasUnvisitedNeighbors) {
        if(!stack.isEmpty()) {
            backtrack();
        }
    }
    else {
        exploreFromProvince(previousProvince);
    }
}

bool RoutePlanner::isExplorationComplete() const {
    return stack.isEmpty() && queue.isEmpty();
}

void RoutePlanner::displayResults() const {
    // Display "Journey Completed!" message
    std::cout << "----------------------------"<< std::endl;
    std::cout <<"Journey Completed!"<< std::endl;
    std::cout << "----------------------------"<< std::endl;
    // Display the total number of provinces visited
    // Display the total distance covered
    std::cout <<"Total Number of Provinces Visited: " << route.size()<< std::endl;
    std::cout <<"Total Distance Covered: " << totalDistanceCovered<<" km"<< std::endl;
    // Display the route in the order visited
    std::cout << "Route Taken:"<< std::endl;
    for(int i = 0; i < route.size(); i++) {
        std::cout << cities[route[i]];
        if (i < route.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
    // Priority Province Summary
    int visitedPriorityProvinces = 0;
    std::cout << "\nPriority Province Status:" << std::endl;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        int province = priorityProvinces[i];
        std::cout << "- " << cities[province] << " (";
        if (map.isVisited(province)) {
            std::cout << "Visited";
            ++visitedPriorityProvinces;
        } else {
            std::cout << "Not Visited";
        }
        std::cout << ")" << std::endl;
    }

    std::cout << "\nTotal Priority Provinces Visited: " << visitedPriorityProvinces << " out of " << numPriorityProvinces << std::endl;
    if (visitedPriorityProvinces == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited." << std::endl;
    } else {
        std::cout << "Warning: Not all priority provinces were visited." << std::endl;
    }
}



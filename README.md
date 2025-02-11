# 🚁 Drone ALPHA – C++ Stack & Queue Based Route Optimization System

Drone ALPHA is a **C++-based autonomous drone navigation system** that uses **Stack (LIFO) and Queue (FIFO) data structures** to plan efficient travel routes across Türkiye’s provinces. The drone must navigate the country while managing **fuel limits, priority provinces, and weather-restricted areas**.

## 📌 Features
- **Backtracking with Stack**:
  - Keeps track of visited provinces for backtracking.
  - Allows the drone to return when it reaches a dead-end.
- **Route Management with Queue**:
  - Stores unvisited neighboring provinces in FIFO order.
  - Ensures efficient traversal of all possible paths.
- **Static, Array-Based Data Structures**:
  - Uses **a stack for backtracking** and **a circular queue for route planning**.
- **Fuel Constraints**:
  - The drone must refuel **every 250 km**.
  - Can only travel to provinces within **250 km of its current location**.
- **Priority & Restricted Provinces**:
  - Priority provinces must be visited as part of the mission.
  - Weather-restricted provinces must be **avoided**.
- **2D Distance Matrix Representation**:
  - A **CSV file** contains distance data between provinces.
  - The program loads and processes these distances dynamically.
- **Final Journey Report**:
  - Displays **total distance traveled, number of provinces visited, and priority provinces reached**.

## 🎮 How It Works
1. **Drone ALPHA starts from Ankara (configurable).**
2. **Explores provinces using a Queue for traversal** and **a Stack for backtracking**.
3. **Fuel constraints (max 250 km per move) limit movement**.
4. **Priority provinces are visited first** if reachable.
5. **Weather-restricted provinces are skipped**.
6. **Exploration stops when no further moves are possible**.

## 🚀 Running the Program
Compile and run the program using:
```bash
g++ -std=c++11 *.cpp *.h -o DroneALPHA
./DroneALPHA 250 5 distance_data.csv priority_provinces.txt weather_restricted_provinces.txt



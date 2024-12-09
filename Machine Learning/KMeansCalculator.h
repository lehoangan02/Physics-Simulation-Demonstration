//
// Created by Le Hoang An on 30/11/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_KMEANSCALCULATOR_H
#define PHYSICS_SIMULATION_DEMONSTRATION_KMEANSCALCULATOR_H
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include "raylib.h"
#include "raymath.h"
class KMeansCalculator { // singleton
public:
    static KMeansCalculator* getKMeansCalculator();
    std::vector<Vector2> operator()(const std::vector<Vector2>& Data, const int& K);
    std::vector<int> getAssignment();
private:
    KMeansCalculator() = default;
    ~KMeansCalculator() = default;
    KMeansCalculator(const KMeansCalculator&) = delete;
    KMeansCalculator& operator=(const KMeansCalculator&) = delete;
    static float calculateSquareDistance(Vector2 A, Vector2 B);
    float calculateCost(const std::vector<Vector2> &Data, const std::vector<Vector2> &Centroids, const std::vector<int> &Assignment);
private:
    int m_Limit = 5;
    int m_Attempt = 1;
    std::vector<Vector2> m_Centroids;
    std::vector<int> m_Assignment;

};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_KMEANSCALCULATOR_H

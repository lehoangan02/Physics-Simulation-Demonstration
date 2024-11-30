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
    std::vector<Vector2> operator()(std::vector<Vector2> Data, int K);
private:
    KMeansCalculator() = default;
    ~KMeansCalculator() = default;
    KMeansCalculator(const KMeansCalculator&) = delete;
    KMeansCalculator& operator=(const KMeansCalculator&) = delete;
    float calculateSquareDistance(Vector2 A, Vector2 B);
    float calculateCost(std::vector<Vector2> Data, std::vector<Vector2> Centroids, std::vector<int> Assignment);
private:
    int m_Limit = 10;
    int m_Attempt = 10;

};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_KMEANSCALCULATOR_H

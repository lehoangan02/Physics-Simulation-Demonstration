//
// Created by Le Hoang An on 18/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_EQUATIONSSOLVER_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_EQUATIONSSOLVER_HPP
#include <iostream>
#include <cmath>
class LinearEquationsSolver
{
private:
    float m_a1;
    float m_b1;
    float m_c1;
    float m_a2;
    float m_b2;
    float m_c2;
public:
    LinearEquationsSolver(float a1, float b1, float c1, float a2, float b2, float c2);
    bool solve(float& x, float& y);
};
class QuadraticEquationsSolver
{
public:
    float operator()(float a, float b, float c);
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_EQUATIONSSOLVER_HPP

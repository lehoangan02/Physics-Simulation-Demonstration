//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMTRIANGLE_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMTRIANGLE_HPP
#include "raylib.h"
//#include "raymath.h"
#include "Math/Geometry.hpp"

#include <string>
#include <iostream>
using namespace std;
class PlatformTriangle
{
public:
    Vector2 m_A;
    Vector2 m_B;
    Vector2 m_C;
private:
    Color m_Color;
public:
    PlatformTriangle(Vector2 A, Vector2 B, Vector2 C, Color Color);
    void draw();
    PlatformTriangle createDilation(float Distance);
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMTRIANGLE_HPP

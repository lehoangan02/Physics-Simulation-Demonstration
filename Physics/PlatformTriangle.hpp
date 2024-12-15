//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMTRIANGLE_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMTRIANGLE_HPP

#include <string>
#include <iostream>
#include "../Math/Geometry.hpp"
#include "raylib.h"
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
    virtual void draw();
    PlatformTriangle createDilation(float Distance) const;
    void setColor(Color Color) {m_Color = Color;}
};
class SATPlatformTriangle : public PlatformTriangle
{
private:
    struct Point
    {
        Vector2 Position;
        float Angle;
    };
private:
    Point m_A1;
    Point m_B1;
    Point m_C1;
public:
    SATPlatformTriangle(Vector2 A, Vector2 B, Vector2 C, Color Color);
    void draw() override;
    static const Color Color1;
    static const Color Color2;
    static const Color Color3;
    std::vector<Vector2> getVertices() const;
private:
    void sortVerticesCounterClockWise();
    Vector2 getCenter() const;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMTRIANGLE_HPP

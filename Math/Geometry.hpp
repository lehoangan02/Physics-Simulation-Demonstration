//
// Created by Le Hoang An on 18/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP
#include "raylib.h"
#include "LinearEquationsSolver.hpp"
#include "../PlatformTriangle.hpp"
#include <vector>
#include <iostream>
using namespace std;
class PlatformTriangle;
class LineSegment
{
public:
    Vector2 A;
    Vector2 B;
private:
public:
    LineSegment(Vector2 A, Vector2 B);
    void print();
};
class Line
{
public:
    // ax + by + m_c =0;
    float m_a;
    float m_b;
    float m_c;
public:
    Line(float a, float b, float c);
    Line(const Vector2& A, const Vector2& B);
    Line(const LineSegment& LineSegment);
    float distanceToPoint(const Vector2& Point);
    Vector2 intersection(const Line& OtherLine);
    Vector2 projection(const Vector2& Point);
    Line createParallel(const Vector2& Point, const float& Distance, const bool& SameSide = true);
    void print();
};
bool isInsidePolygon(const Vector2& Point, const vector<LineSegment>& LineSegmentList);
bool isInsideTriangle(const Vector2& Point, const PlatformTriangle& Triangle);
#endif //PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP

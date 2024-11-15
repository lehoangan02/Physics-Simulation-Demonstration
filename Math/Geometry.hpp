//
// Created by Le Hoang An on 18/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP
#include "raylib.h"
#include "raymath.h"
#include "EquationsSolver.hpp"
#include "../Physics/PlatformTriangle.hpp"
#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
using namespace std;
class PlatformTriangle;
class LineSegment
{
public:
    Vector2 A;
    Vector2 B;
public:
    LineSegment(const Vector2& A, const Vector2& B);
    void print();
    float getLength();
    Vector2 getMidPoint();
private:
    bool haveProjection(const Vector2& Point);
    Vector2 projection(const Vector2& Point);
    friend class ContinuousEulerianEngine;
    Vector2 getPointWithDistance(const float& Distance, std::string Side);
};
class Line
{
public:
    // m_ax + m_by + m_c =0;
    float m_a;
    float m_b;
    float m_c;
private:
    void Normalized();
public:
    Line(float a, float b, float c);
    Line(const Vector2& A, const Vector2& B);
    Line(const LineSegment& LineSegment);
    Line(std::pair<Vector2, Vector2> DirectionVectorAndPoint);
    float distanceToPoint(const Vector2& Point);
    Vector2 intersection(const Line& OtherLine);
    Vector2 projection(const Vector2& Point);
    Line createParallel(const Vector2& Point, const float& Distance, const bool& SameSide = true);
    void print();
    float findX(float y);
    float findY(float x);
    Line flipHorizontally(Vector2 Point);
    Line flipVertically(Vector2 Point);
    bool isVertical();
    bool isHorizontal();
    bool isParallel(Line Other);
    float distanceToLine(const Line& Other);
};
class Ray2D {
private:
    Vector2 m_Origin;
    Vector2 m_Direction;
public:
    Ray2D(Vector2 Origin, Vector2 Direction);
    bool haveProjection(const Vector2& Point);
    Vector2 projection(const Vector2& Point);
    Ray2D bisector(Ray2D Ray1, Ray2D Ray2);
    Line getLine();
};
bool isInsidePolygon(const Vector2& Point, const vector<LineSegment>& LineSegmentList);
bool isInsideTriangle(const Vector2& Point, const PlatformTriangle& Triangle);
float dotProduct(Vector2 First, Vector2 Second);
float angle(Vector2 A, Vector2 B);
#endif //PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP

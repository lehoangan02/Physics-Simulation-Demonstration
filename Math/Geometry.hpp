//
// Created by Le Hoang An on 18/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP
#include "EquationsSolver.hpp"
#include <vector>
#include <iostream>
#include <utility>
#include <cmath>
#include "raylib.h"
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
    void draw(Color color) { DrawLineEx(A, B, 2, color); }
    bool haveProjection(const Vector2& Point);
private:
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
    void draw(Color color) { DrawLineEx(Vector2{findX(0), 0}, Vector2{findX(1000), 1000}, 2, color); }
    Vector2 getNormalDirection();
    Vector2 getDirection();
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
float dotProduct(const Vector2 &First, const Vector2 &Second);
float angle(Vector2 A, Vector2 B);
float angle360InRadian(Vector2 A, Vector2 B);
float radianToDegree(float Radian);
float degreeToRadian(float Degree);
Vector2 flipVector(Vector2 Initial, Vector2 Mirror);
float calculateAreaTriangle(const Vector2& A, const Vector2& B, const Vector2& C);
float calculateAreaPolygon(const vector<Vector2>& Polygon);
void drawArrow(Vector2 Start, Vector2 End, Color color);
void drawArrow(Vector2 Start, Vector2 End, Color color, float Thickness);
vector<Vector2> dilatePolygon(const vector<Vector2>& Polygon, float Scale);
float calculateAreaPolygon(const vector<Vector2>& Polygon);
void drawSquare(Vector2 Center, float EdgeSize, float Thickness, Color Color);
void drawSquare(Vector2 Center, float EdgeSize, Color Color);
void drawSquare(Vector2 Center);
void drawSquare(Vector2 Center, float EdgeSize);
#endif //PHYSICS_SIMULATION_DEMONSTRATION_GEOMETRY_HPP

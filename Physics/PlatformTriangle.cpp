//
// Created by Le Hoang An on 17/10/24.
//
#include "PlatformTriangle.hpp"
#include "../Math/Geometry.hpp"
#include "raymath.h"
PlatformTriangle::PlatformTriangle(Vector2 A, Vector2 B, Vector2 C, Color Color) : m_A(A), m_B(B), m_C(C), m_Color(Color) {
}
void PlatformTriangle::draw() {
    DrawTriangle(m_A, m_B, m_C, m_Color);
}
PlatformTriangle PlatformTriangle::createDilation(float Distance) const {
    LineSegment AB(m_A, m_B);
    LineSegment BC(m_B, m_C);
    LineSegment CA(m_C, m_A);
//    AB.print();
    Line LineAB(AB);
    Line LineBC(BC);
    Line LineCA(CA);
//    LineAB.print();
//    LineBC.print();
//    LineCA.print();
    Line NewLineAB = LineAB.createParallel(m_C, Distance, false);
    Line NewLineBC = LineBC.createParallel(m_A, Distance, false);
    Line NewLineCA = LineCA.createParallel(m_B, Distance, false);
//    NewLineAB.print();
//    NewLineBC.print();
//    NewLineCA.print();
    Vector2 NewA = NewLineAB.intersection(NewLineCA);
    Vector2 NewB = NewLineAB.intersection(NewLineBC);
    Vector2 NewC = NewLineBC.intersection(NewLineCA);
//    std::cout << "NewA: " << NewA.x << " " << NewA.y << std::endl;
//    std::cout << "NewB: " << NewB.x << " " << NewB.y << std::endl;
//    std::cout << "NewC: " << NewC.x << " " << NewC.y << std::endl;
    return PlatformTriangle(NewA, NewB, NewC, YELLOW);
}
const Color SATPlatformTriangle::Color1 = Color(100, 13, 95, 255);
const Color SATPlatformTriangle::Color2 = Color(217, 22, 86, 255);
const Color SATPlatformTriangle::Color3 = Color(231, 91, 0, 255);
SATPlatformTriangle::SATPlatformTriangle(Vector2 A, Vector2 B, Vector2 C, Color Color) : PlatformTriangle(A, B, C, Color) {
    m_A1.Position = A;
    m_B1.Position = B;
    m_C1.Position = C;
    m_A1.Angle = 0;
    m_B1.Angle = 0;
    m_C1.Angle = 0;
    sortVerticesCounterClockWise();
}
Vector2 SATPlatformTriangle::getCenter() const {
    Vector2 Center;
    Center.x = (m_A1.Position.x + m_B1.Position.x + m_C1.Position.x) / 3;
    Center.y = (m_A1.Position.y + m_B1.Position.y + m_C1.Position.y) / 3;
    return Center;
}
void SATPlatformTriangle::sortVerticesCounterClockWise() {
    Vector2 Center = getCenter();
    Vector2 Reference = Vector2Subtract(m_A1.Position, Center);
    m_A1.Angle = angle360InRadian(Reference, Reference);
    m_B1.Angle = angle360InRadian(Vector2Subtract(m_B1.Position, Center), Reference);
    m_C1.Angle = angle360InRadian(Vector2Subtract(m_C1.Position, Center), Reference);
    if (m_B1.Angle > m_C1.Angle) {
        std::swap(m_B1, m_C1);
    }
}
void SATPlatformTriangle::draw() {
//    std::cout << "Drawing\n";
    if (m_DrawInternal) {
        DrawTriangle(m_A1.Position, m_B1.Position, m_C1.Position, Color{255, 178, 0, 255});
    }
    DrawLineEx(m_A1.Position, m_B1.Position, 5, Color1);
    DrawLineEx(m_B1.Position, m_C1.Position, 5, Color2);
    DrawLineEx(m_C1.Position, m_A1.Position, 5, Color3);
    DrawCircle((int)m_A1.Position.x, (int)m_A1.Position.y, 10, Color1);
    DrawCircle((int)m_B1.Position.x, (int)m_B1.Position.y, 10, Color2);
    DrawCircle((int)m_C1.Position.x, (int)m_C1.Position.y, 10, Color3);
}
std::vector<Vector2> SATPlatformTriangle::getVertices() const {
    std::vector<Vector2> Vertices;
    Vertices.push_back(m_A1.Position);
    Vertices.push_back(m_B1.Position);
    Vertices.push_back(m_C1.Position);
    return Vertices;
}
void SATPlatformTriangle::move(Vector2 Direction) {
    m_A1.Position = Vector2Add(m_A1.Position, Direction);
    m_B1.Position = Vector2Add(m_B1.Position, Direction);
    m_C1.Position = Vector2Add(m_C1.Position, Direction);
}
void SATPlatformTriangle::rotate(float Angle) {
    Vector2 Center = getCenter();
    Vector2 DirectionA = Vector2Subtract(m_A1.Position, Center);
    Vector2 DirectionB = Vector2Subtract(m_B1.Position, Center);
    Vector2 DirectionC = Vector2Subtract(m_C1.Position, Center);
    DirectionA = Vector2Rotate(DirectionA, Angle);
    DirectionB = Vector2Rotate(DirectionB, Angle);
    DirectionC = Vector2Rotate(DirectionC, Angle);
    m_A1.Position = Vector2Add(Center, DirectionA);
    m_B1.Position = Vector2Add(Center, DirectionB);
    m_C1.Position = Vector2Add(Center, DirectionC);
}
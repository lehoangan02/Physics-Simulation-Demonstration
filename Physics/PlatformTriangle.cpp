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
const Color SATPlatformTriangle::Color1 = Color(227, 240, 175, 255);
const Color SATPlatformTriangle::Color2 = Color(93, 185, 150, 255);
const Color SATPlatformTriangle::Color3 = Color(17, 139, 80, 255);
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
    DrawCircle((int)m_A1.Position.x, (int)m_A1.Position.y, 5, Color1);
    DrawCircle((int)m_B1.Position.x, (int)m_B1.Position.y, 5, Color2);
    DrawCircle((int)m_C1.Position.x, (int)m_C1.Position.y, 5, Color3);
}
std::vector<Vector2> SATPlatformTriangle::getVertices() const {
    std::vector<Vector2> Vertices;
    Vertices.push_back(m_A1.Position);
    Vertices.push_back(m_B1.Position);
    Vertices.push_back(m_C1.Position);
    return Vertices;
}
//
// Created by Le Hoang An on 17/10/24.
//
#include "PlatformTriangle.hpp"
PlatformTriangle::PlatformTriangle(Vector2 A, Vector2 B, Vector2 C, Color Color) : m_A(A), m_B(B), m_C(C), m_Color(Color) {
}
void PlatformTriangle::draw() {
    DrawTriangle(m_A, m_B, m_C, m_Color);
}
PlatformTriangle PlatformTriangle::createDilation(float Distance) {
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
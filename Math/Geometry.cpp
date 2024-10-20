//
// Created by Le Hoang An on 18/10/24.
//

#include "Geometry.hpp"
LineSegment::LineSegment(Vector2 A, Vector2 B) {
    this->A = A;
    this->B = B;
}
void LineSegment::print() {
    std::cout << "LineSegment: A(" << A.x << ", " << A.y << ") B(" << B.x << ", " << B.y << ")" << std::endl;
}
Line::Line(float a, float b, float c) : m_a(a), m_b(b), m_c(c) {
}
Line::Line(const LineSegment& LineSegment) {
    // y = ax + m_c
    // ax + by + m_c = 0
    if (LineSegment.A.x == LineSegment.B.x)
    {
        m_a = 1;
        m_b = 0;
        m_c = -LineSegment.A.x;
        return;
    }
    m_b = -1;
    m_a = (LineSegment.A.y - LineSegment.B.y) / (LineSegment.A.x - LineSegment.B.x);
    m_c = LineSegment.A.y - m_a * LineSegment.A.x;
}
Line::Line(const Vector2& A, const Vector2& B) {
    // y = ax + m_c
    // ax + by + m_c = 0
    m_b = -1;
    m_a = (A.y - B.y) / (A.x - B.x);
    m_c = A.y - m_a * A.x;
}
float Line::distanceToPoint(const Vector2& Point) {
    return static_cast<float>(fabs(m_a * Point.x + m_b * Point.y + m_c) / sqrt(m_a * m_a + m_b * m_b));
}
Line Line::createParallel(const Vector2& Point, const float& Distance, const bool& SameSide) {
    float a = m_a;
    float b = m_b;
    float c1, c2;
    c1 = Distance * static_cast<float>(sqrt(m_a * m_a + m_b * m_b)) + m_c;
    c2 = m_c - Distance * static_cast<float>(sqrt(m_a * m_a + m_b * m_b));
    Vector2 Point1(0, - c1 / b);
    Vector2 Point2(0, - c2/ b);
    if (SameSide)
    {
        if ((m_a * Point.x + m_b * Point.y + m_c) * (m_a * Point1.x + m_b * Point1.y + m_c) > 0)
        {
            return Line(a, b, c1);
        }
        else
        {
            return Line(a, b, c2);
        }
    }
    else {
        if ((m_a * Point.x + m_b * Point.y + m_c) * (m_a * Point2.x + m_b * Point2.y + m_c) < 0)
        {
            return Line(a, b, c2);
        }
        else
        {
            return Line(a, b, c1);
        }
    }
}
Vector2 Line::intersection(const Line& OtherLine) {
    float x, y;
    LinearEquationsSolver Solver(m_a, m_b, m_c, OtherLine.m_a, OtherLine.m_b, OtherLine.m_c);
    Solver.solve(x, y);
    return Vector2{ x, y };
}
bool isInsidePolygon(const Vector2& Point, const vector<LineSegment>& LineSegmentList)
{
    int CountIntersection = 0;
    for (auto& LineSegment : LineSegmentList)
    {
        float MaxY = max(LineSegment.A.y, LineSegment.B.y);
        float MinY = min(LineSegment.A.y, LineSegment.B.y);
        if (Point.y > MaxY || Point.y < MinY) continue;
        // calculate the intersection's x-coordinate between the line y = Point.y and the LineSegment
        float TargetX = LineSegment.A.x + (Point.y - LineSegment.A.y) / (LineSegment.B.y - LineSegment.A.y) * (LineSegment.B.x - LineSegment.B.y);
        if (Point.x <= TargetX) ++CountIntersection;
    }
    return CountIntersection % 2;
}
void Line::print() {
    std::cout << "Line: " << m_a << "x + " << m_b << "y + " << m_c << " = 0" << std::endl;
}
bool isInsideTriangle(const Vector2& Point, const PlatformTriangle& Triangle)
{
    LineSegment AB(Triangle.m_A, Triangle.m_B);
    LineSegment BC(Triangle.m_B, Triangle.m_C);
    LineSegment CA(Triangle.m_C, Triangle.m_A);
    vector<LineSegment> LineSegmentList = {AB, BC, CA};
    return isInsidePolygon(Point, LineSegmentList);
}
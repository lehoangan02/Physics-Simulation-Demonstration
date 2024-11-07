//
// Created by Le Hoang An on 18/10/24.
//

#include "Geometry.hpp"
LineSegment::LineSegment(const Vector2& A, const Vector2& B) {
    this->A = A;
    this->B = B;
}
void LineSegment::print() {
    std::cout << "LineSegment: A(" << A.x << ", " << A.y << ") B(" << B.x << ", " << B.y << ")" << std::endl;
}
float LineSegment::getLength() {
    return Vector2Length(Vector2Subtract(B, A));
}
Vector2 LineSegment::getMidPoint() {
    return Vector2Scale(Vector2Add(A, B), 0.5f);
}
Vector2 LineSegment::projection(const Vector2 &Point) {
    Line AB(A, B);
    return AB.projection(Point);
}
bool LineSegment::haveProjection(const Vector2 &Point) {
    Line AB(A, B);
    Vector2 Projection = AB.projection(Point);
    if ((Projection.x - A.x) * (Projection.x - B.x) <= 0 && (Projection.y - A.y) * (Projection.y - B.y) <= 0) return true;
    return false;
}
Vector2 LineSegment::getPointWithDistance(const float &Distance, std::string Side) {
    Vector2 Direction = Vector2Normalize(Vector2Subtract(A, B));
    Vector2 Normalized = Vector2Normalize(Direction);
    Vector2 ScaledDirection = Vector2Scale(Normalized, Distance);
    if (Side == "A")
    {
        return Vector2Add(A, ScaledDirection);
    }
    else
    {
        return Vector2Subtract(B, ScaledDirection);
    }
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
    if (A.x == B.x)
    {
        m_a = 1;
        m_b = 0;
        m_c = -A.x;
        return;
    }
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
        float TargetX = LineSegment.A.x + (Point.y - LineSegment.A.y) / (LineSegment.B.y - LineSegment.A.y) * (LineSegment.B.x - LineSegment.A.x);
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
Vector2 Line::projection(const Vector2 &Point) {
    if (m_a * Point.x + m_b * Point.y + m_c == 0)
    {
        return Point;
    }
    // ax + by + c = 0
    // b(x - x0) - a(y - y0) = 0
    LinearEquationsSolver Solver(m_a, m_b, + m_c, m_b, - m_a, + (m_a * Point.y - m_b * Point.x));
    float x; float y;
//    std::cout << m_a << ' ' << m_b << ' ' << m_c << std::endl;
//    std::cout << m_b << ' ' << - m_a << ' ' << m_a * Point.y - m_b * Point.x << std::endl;
    Solver.solve(x, y);
//    std::cout << "Result: " << x << " " << y << std::endl;
    Vector2 Res{x, y};
    return Res;
}
float Line::findX(float y) {
    return (-m_b * y - m_c) / m_a;
}
float Line::findY(float x) {
    return (-m_a * x - m_c) / m_b;
}
Line Line::flipHorizontally(Vector2 Point) {
    float a = m_a;
    float b = - m_b;
    float c = -(a * Point.x + b * Point.y);
    return Line(a, b, c);
}
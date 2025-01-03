//
// Created by Le Hoang An on 18/10/24.
//

#include "Geometry.hpp"
#include "../Physics/PlatformTriangle.hpp"
#include "raymath.h"
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
    Vector2 Direction = Vector2Subtract(B, A);
//    std::cout << "Direction: " << Direction.x << " " << Direction.y << std::endl;
//    std::cout << "A: " << A.x << " " << A.y << std::endl;
//    std::cout << "B: " << B.x << " " << B.y << std::endl;
    Vector2 Normalized = Vector2Normalize(Direction);
//    std::cout << "Normalized: " << Normalized.x << " " << Normalized.y << std::endl;
    Vector2 ScaledDirection = Vector2Scale(Normalized, Distance);
//    std::cout << "ScaledDirection: " << ScaledDirection.x << " " << ScaledDirection.y << std::endl;
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
Line::Line(std::pair<Vector2, Vector2> DirectionVectorAndPoint) {
    m_a = DirectionVectorAndPoint.first.y;
    m_b = - DirectionVectorAndPoint.first.x;
    m_c = - (m_a * DirectionVectorAndPoint.second.x + m_b * DirectionVectorAndPoint.second.y);
//    Normalized();
}
void Line::Normalized() {
    if (m_a < 0)
    {
        m_a = -m_a;
        m_b = -m_b;
        m_c = -m_c;
    }
    if (m_a == 0)
    {
        m_b = 1;
        m_c = -m_c;
    }
    else if (m_b == 0)
    {
        m_a = 1;
        m_c = -m_c;
    }
    if (fabs(m_a) < fabs(m_b))
    {
        m_b = m_b / m_a;
        m_c = m_c / m_a;
        m_a = 1;
    }
    else
    {
        m_a = m_a / m_b;
        m_c = m_c / m_b;
        m_b = 1;
    }
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
    if (!m_a) return 0;
    return (-m_b * y - m_c) / m_a;
}
float Line::findY(float x) {
    if (!m_b) return 0;
    return (-m_a * x - m_c) / m_b;
}
Line Line::flipHorizontally(Vector2 Point) {
    float a = m_a;
    float b = - m_b;
    float c = -(a * Point.x + b * Point.y);
    return Line(a, b, c);
}
Line Line::flipVertically(Vector2 Point) {
    float a = - m_a;
    float b = m_b;
    float c = -(a * Point.x + b * Point.y);
    return Line(a, b, c);
}
bool Line::isVertical() {
    if (!m_b) return true;
    return false;
}
bool Line::isHorizontal() {
    if (!m_a) return true;
    return false;
}
bool Line::isParallel(Line Other) {
    if (m_a == 0 && Other.m_b == 0) return false;
    if (m_b == 0 && Other.m_a == 0) return false;
    if (m_a * Other.m_b == m_b * Other.m_a) return true;
    return false;
}
float Line::distanceToLine(const Line &Other) {
    float x = 1;
//    float y =

}
Vector2 Line::getNormalDirection() {
    return Vector2{m_a, m_b};
}
Vector2 Line::getDirection() {
    return Vector2{-m_b, m_a};
}
Ray2D::Ray2D(Vector2 Origin, Vector2 Direction) : m_Origin(Origin), m_Direction(Direction) {
}
Ray2D Ray2D::bisector(Ray2D Ray1, Ray2D Ray2) {
    Vector2 Direction1 = Vector2Normalize(Ray1.m_Direction);
    Vector2 Direction2 = Vector2Normalize(Ray2.m_Direction);
    Vector2 Direction = Vector2Normalize(Vector2Add(Direction1, Direction2));
    return Ray2D(Ray1.m_Origin, Direction);
}
Line Ray2D::getLine() {
    std::pair<Vector2, Vector2> DirectionVectorAndPoint(m_Direction, m_Origin);
    return Line(DirectionVectorAndPoint);
}
float dotProduct(const Vector2 &First, const Vector2 &Second) {
    return First.x * Second.x + First.y * Second.y;
}

float crossProduct(const Vector2 &First, const Vector2 &Second) {
    return First.x * Second.y - First.y * Second.x;
}

float angle(Vector2 A, Vector2 B) {
    float dot = dotProduct(A, B) / (Vector2Length(A) * Vector2Length(B));
    dot = fmax(-1.0f, fmin(1.0f, dot));
    return acos(dot);
}

float angle360InRadian(Vector2 A, Vector2 B) {
    float Angle = angle(A, B);
    float cross = A.x * B.y - A.y * B.x;
    if (cross < 0) {
        Angle = 2 * M_PI - Angle;
    }
    return fmod(Angle, 2 * M_PI); // Wrap angle between 0 and 2π
}


float radianToDegree(float Radian)
{
    return Radian * 180 / PI;
}
Vector2 flipVector(Vector2 Initial, Vector2 Mirror) {
    Vector2 Normalized = Vector2Normalize(Mirror);
    float DotProduct = dotProduct(Initial, Normalized);
    Normalized = Vector2Scale(Normalized, DotProduct * 2);
    Vector2 Result = Vector2Subtract(Normalized, Initial);
//    std::cout << "Initial: " << Initial.x << " " << Initial.y << std::endl;
//    std::cout << "Mirror: " << Mirror.x << " " << Mirror.y << std::endl;
//    std::cout << "Result: " << Result.x << " " << Result.y << std::endl;
    return Vector2Negate(Result);
}
float degreeToRadian(float Degree) {
    return Degree * PI / 180;
}
float calculateAreaTriangle(const Vector2& A, const Vector2& B, const Vector2& C) {
    return fabs((A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y)) / 2);
}
float calculateAreaPolygon(const vector<Vector2>& Polygon) {
    float Area = 0;
    Vector2 Center = {0, 0};
    for (int i = 0; i < Polygon.size(); ++i) {
        Center = Vector2Add(Center, Polygon[i]);
    }
    Center = Vector2Scale(Center, 1.0f / Polygon.size());
    for (int i = 0; i < Polygon.size(); ++i) {
        Area += calculateAreaTriangle(Center, Polygon[i], Polygon[(i + 1) % Polygon.size()]);
    }
    return Area;
}
void drawArrow(Vector2 Start, Vector2 End, Color color) {
    if (Vector2Length(Vector2Subtract(End, Start)) == 0) return;
    float BorderThickness = 3;
    float Thickness = 3;
    DrawCircleV(Start, Thickness / 2 + BorderThickness, BLACK);
    DrawLineEx(Start, End, Thickness + BorderThickness * 2, BLACK);
    DrawCircleV(Start, Thickness / 2, color);
    DrawLineEx(Start, End, Thickness, color);
    Vector2 direction = { End.x - Start.x, End.y - Start.y };
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    Vector2 unitDirection = { direction.x / length, direction.y / length };
    const float arrowHeadLength = 20.0f;
    const float arrowHeadAngle = 30.0f * (PI / 180.0f);
    Vector2 arrowLeft = {
            End.x - arrowHeadLength * (cosf(arrowHeadAngle) * unitDirection.x - sinf(arrowHeadAngle) * unitDirection.y),
            End.y - arrowHeadLength * (sinf(arrowHeadAngle) * unitDirection.x + cosf(arrowHeadAngle) * unitDirection.y)
    };
    Vector2 arrowRight = {
            End.x - arrowHeadLength * (cosf(-arrowHeadAngle) * unitDirection.x - sinf(-arrowHeadAngle) * unitDirection.y),
            End.y - arrowHeadLength * (sinf(-arrowHeadAngle) * unitDirection.x + cosf(-arrowHeadAngle) * unitDirection.y)
    };
    DrawCircleV(End, Thickness / 2 + BorderThickness, BLACK);
    DrawCircleV(End, Thickness / 2, color);
    DrawCircleV(arrowLeft, Thickness / 2 + BorderThickness, BLACK);
    DrawCircleV(arrowRight, Thickness / 2 + BorderThickness, BLACK);
    DrawCircleV(arrowLeft, Thickness / 2, color);
    DrawCircleV(arrowRight, Thickness / 2, color);
    DrawLineEx(End, arrowLeft, Thickness + BorderThickness * 2, BLACK);
    DrawLineEx(End, arrowRight, Thickness + BorderThickness * 2, BLACK);
    DrawLineEx(End, arrowLeft, Thickness, color);
    DrawLineEx(End, arrowRight, Thickness, color);
}
void drawArrow(Vector2 Start, Vector2 End, Color color, float Thickness) {
    if (Vector2Length(Vector2Subtract(End, Start)) == 0) return;
    float BorderThickness = Thickness;
    DrawCircleV(Start, Thickness / 2 + BorderThickness, BLACK);
    DrawLineEx(Start, End, Thickness + BorderThickness * 2, BLACK);
    DrawCircleV(Start, Thickness / 2, color);
    DrawLineEx(Start, End, Thickness, color);
    Vector2 direction = { End.x - Start.x, End.y - Start.y };
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    Vector2 unitDirection = { direction.x / length, direction.y / length };
    const float arrowHeadLength = 20.0f;
    const float arrowHeadAngle = 30.0f * (PI / 180.0f);
    Vector2 arrowLeft = {
        End.x - arrowHeadLength * (cosf(arrowHeadAngle) * unitDirection.x - sinf(arrowHeadAngle) * unitDirection.y),
        End.y - arrowHeadLength * (sinf(arrowHeadAngle) * unitDirection.x + cosf(arrowHeadAngle) * unitDirection.y)
};
    Vector2 arrowRight = {
        End.x - arrowHeadLength * (cosf(-arrowHeadAngle) * unitDirection.x - sinf(-arrowHeadAngle) * unitDirection.y),
        End.y - arrowHeadLength * (sinf(-arrowHeadAngle) * unitDirection.x + cosf(-arrowHeadAngle) * unitDirection.y)
};
    DrawCircleV(End, Thickness / 2 + BorderThickness, BLACK);
    DrawCircleV(End, Thickness / 2, color);
    DrawCircleV(arrowLeft, Thickness / 2 + BorderThickness, BLACK);
    DrawCircleV(arrowRight, Thickness / 2 + BorderThickness, BLACK);
    DrawCircleV(arrowLeft, Thickness / 2, color);
    DrawCircleV(arrowRight, Thickness / 2, color);
    DrawLineEx(End, arrowLeft, Thickness + BorderThickness * 2, BLACK);
    DrawLineEx(End, arrowRight, Thickness + BorderThickness * 2, BLACK);
    DrawLineEx(End, arrowLeft, Thickness, color);
    DrawLineEx(End, arrowRight, Thickness, color);
}

vector<Vector2> dilatePolygon(const vector<Vector2>& Polygon, float Scale) {
    Vector2 Center = {0, 0};
    for (int i = 0; i < Polygon.size(); ++i) {
        Center = Vector2Add(Center, Polygon[i]);
    }
    Center = Vector2Scale(Center, 1.0f / Polygon.size());
    vector<Vector2> DilatedPolygon;
    for (int i = 0; i < Polygon.size(); ++i) {
        Vector2 Direction = Vector2Subtract(Polygon[i], Center);
        Direction = Vector2Scale(Direction, Scale);
        DilatedPolygon.push_back(Vector2Add(Center, Direction));
    }
    return DilatedPolygon;
}
float calculateAreaPolygon(const vector<Vector2>& Polygon, const vector<int>& Index) {
    float Area = 0;
    Vector2 Center = {0, 0};
    for (int i = 0; i < Index.size(); ++i) {
        Center = Vector2Add(Center, Polygon[Index[i]]);
    }
    Center = Vector2Scale(Center, 1.0f / Index.size());
    for (int i = 0; i < Index.size(); ++i) {
        Area += calculateAreaTriangle(Center, Polygon[Index[i]], Polygon[Index[(i + 1) % Index.size()]]);
    }
    return Area;
}
void drawSquare(Vector2 Center, float EdgeSize, float Thickness, Color Color) {
    Vector2 A = {Center.x - EdgeSize / 2, Center.y - EdgeSize / 2};
    Vector2 B = {Center.x + EdgeSize / 2, Center.y - EdgeSize / 2};
    Vector2 C = {Center.x + EdgeSize / 2, Center.y + EdgeSize / 2};
    Vector2 D = {Center.x - EdgeSize / 2, Center.y + EdgeSize / 2};
    DrawLineEx(A, B, Thickness, Color);
    DrawLineEx(B, C, Thickness, Color);
    DrawLineEx(C, D, Thickness, Color);
    DrawLineEx(D, A, Thickness, Color);
    DrawCircleV(A, Thickness / 2, Color);
    DrawCircleV(B, Thickness / 2, Color);
    DrawCircleV(C, Thickness / 2, Color);
    DrawCircleV(D, Thickness / 2, Color);
}

void drawSquare(Vector2 Center, float EdgeSize, Color Color) {
    drawSquare(Center, EdgeSize, 3, Color);
}
void drawSquare(Vector2 Center) {
    drawSquare(Center, 50, 3, RED);
}
void drawSquare(Vector2 Center, float EdgeSize) {
    drawSquare(Center, EdgeSize, 3, RED);
}

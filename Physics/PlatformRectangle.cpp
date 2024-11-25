//
// Created by Le Hoang An on 20/11/24.
//

#include "PlatformRectangle.h"
PlatformRectangle::PlatformRectangle(Vector2 TopLeft, float Width, float Height, float Rotation, Color Color) : m_TopLeft(TopLeft), m_Width(Width), m_Height(Height), m_Rotation(Rotation),
                                                                                                                                m_Color(Color) {
    float X;
    float Y;
    float Radian = m_Rotation * M_PI / 180;
    X = std::cos(Radian) * m_Width;
    Y = std::sin(Radian) * m_Width;
    m_TopRight = Vector2Add(m_TopLeft, Vector2{X, Y});
    Y = std::cos(Radian) * m_Height;
    X = std::sin(Radian) * m_Height;
    m_BottomLeft = Vector2Add(m_TopLeft, Vector2{-X, Y});
    m_BottomRight = Vector2Add(m_TopRight, Vector2{-X, Y});
    m_LineSegmentList.push_back(LineSegment(m_TopLeft, m_TopRight));
    m_LineSegmentList.push_back(LineSegment(m_TopRight, m_BottomRight));
    m_LineSegmentList.push_back(LineSegment(m_BottomRight, m_BottomLeft));
    m_LineSegmentList.push_back(LineSegment(m_BottomLeft, m_TopLeft));
}
void PlatformRectangle::draw() {
    DrawRectanglePro(Rectangle{m_TopLeft.x, m_TopLeft.y, m_Width, m_Height}, m_Origin, m_Rotation, m_Color);
//    DrawCircle(m_TopLeft.x, m_TopLeft.y, 5, RED);
//    DrawCircle(m_TopRight.x, m_TopRight.y, 5, RED);
//    DrawCircle(m_BottomRight.x, m_BottomRight.y, 5, RED);
//    DrawCircle(m_BottomLeft.x, m_BottomLeft.y, 5, RED);

}
bool PlatformRectangle::isInside(Vector2 Point) {
    return isInsidePolygon(Point, m_LineSegmentList);
}
void PlatformRectangle::setRotation(float Rotation) {
    m_Rotation = Rotation;
    float X;
    float Y;
    float Radian = m_Rotation * M_PI / 180;
    X = std::cos(Radian) * m_Width;
    Y = std::sin(Radian) * m_Width;
    Vector2 TopRight = Vector2Add(m_TopLeft, Vector2{X, Y});
    X = std::cos(Radian) * m_Height;
    Y = std::sin(Radian) * m_Height;
    Vector2 BottomRight = Vector2Add(TopRight, Vector2{X, Y});
    Vector2 BottomLeft = Vector2Add(m_TopLeft, Vector2{X, Y});
    m_LineSegmentList[0] = LineSegment(m_TopLeft, TopRight);
    m_LineSegmentList[1] = LineSegment(TopRight, BottomRight);
    m_LineSegmentList[2] = LineSegment(BottomRight, BottomLeft);
    m_LineSegmentList[3] = LineSegment(BottomLeft, m_TopLeft);
}
void PlatformRectangle::setOrigin(Vector2 Origin) {
    m_Origin = Origin;
}


//
// Created by Le Hoang An on 20/11/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMRECTANGLE_H
#define PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMRECTANGLE_H

#include "../Math/Geometry.hpp"
#include "raylib.h"
#include <vector>
#include <cmath>
class PlatformRectangle {
    friend class DiscreteEulerianEngine;
private:
    Vector2 m_TopLeft;
    Vector2 m_TopRight;
    Vector2 m_BottomRight;
    Vector2 m_BottomLeft;
    float m_Width;
    float m_Height;
    Vector2 m_Origin = {0, 0};
    float m_Rotation; // degree
    Color m_Color;
    vector<LineSegment> m_LineSegmentList;
public:
    void draw();
    PlatformRectangle(Vector2 TopLeft, float Width, float Height, float Rotation, Color Color);
    bool isInside(Vector2 Point);
    void setRotation(float Rotation);
    void setOrigin(Vector2 Origin);
};


#endif //PHYSICS_SIMULATION_DEMONSTRATION_PLATFORMRECTANGLE_H

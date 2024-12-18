//
// Created by Le Hoang An on 16/12/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_SATPLATFORM_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_SATPLATFORM_HPP
#include "raylib.h"
#include "PlatformTriangle.hpp"
#include "RoundBall.hpp"
struct Point
{
    Vector2 Position;
    float Angle;
};
class SATPlatformTriangle : public PlatformTriangle
{
private:

private:
    Point m_A1;
    Point m_B1;
    Point m_C1;
public:
    SATPlatformTriangle(Vector2 A, Vector2 B, Vector2 C, Color Color);
    void draw() override;
    static const Color Color1;
    static const Color Color2;
    static const Color Color3;
    std::vector<Vector2> getVertices() const;
    void move(Vector2 Direction);
    void rotate(float Angle);
    void setActive(bool Active) {m_DrawInternal = Active;}
    bool isActive() const {return m_DrawInternal;}
private:
    void sortVerticesCounterClockWise();
    Vector2 getCenter() const;
    bool m_DrawInternal = false;
};
class SATPlatformPolygon
{
public:
    SATPlatformPolygon(const std::vector<Vector2>& Vertices, Color Color);
    void draw();
    void move(Vector2 Direction);
    void rotate(float Angle);
    void update(float DeltaTime);
    void setActive(bool Active) {m_DrawInternal = Active;}
    bool isActive() const {return m_DrawInternal;}
    std::vector<Vector2> getVertices() const;
    void setCustomColor(Color CustomColor) {m_CustomColor = CustomColor; m_UseCustomColor = true;}
    void accelerate(Vector2 Acceleration);
    void setVelocity(Vector2 Velocity) {m_Velocity = Velocity;}
    Vector2 getVelocity() const {return m_Velocity;}
    void addVelocity(Vector2 Velocity);
    Vector2 getCenter() const;
private:
    const Color m_VertexColor{33, 53, 85, 255};
    const Color m_EdgeColor{62, 88, 121, 255};
    const Color m_ActiveInternalColor{216, 196, 182, 255};
    const Color m_InactiveInternalColor{245, 239, 231, 255};
    bool m_UseCustomColor = false;
    Color m_CustomColor;
    int m_NumberOfVertices = 0;
    bool m_DrawInternal = false;
    std::vector<Point> m_Vertices;
    Vector2 m_Velocity = {0, 0};
    Vector2 m_Acceleration = {0, 0};
private:
    void sortVerticesCounterClockWise();
};
class SATPlatformCircle : public EulerianRoundBall
{
    friend class SATCirclePolygonCollider;
public:
    SATPlatformCircle(Vector2 Position, Color Color, float Mass);
    void draw() override;
    void move(Vector2 Direction);
    void update(float DeltaTime);
    void setActive(bool Active) {m_DrawInternal = Active;}
    Vector2 getCenter() const {return m_CurrentPosition;}
    float getRadius() const {return m_Radius;}
    void setRadius(float Radius);
    void accelerate(Vector2 Acceleration);
    Vector2 getVelocity() const {return m_Velocity;}
    float getMass() const {return m_Mass;}
    void addVelocity(Vector2 Velocity);
    void setVelocity(Vector2 Velocity);
    Vector2 getCenter() {return m_CurrentPosition;}
private:
    bool m_DrawInternal = false;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SATPLATFORM_HPP

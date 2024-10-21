//
// Created by Le Hoang An on 17/10/24.
//
#include "RoundBall.hpp"
#include <random>
RoundBall::RoundBall(Vector2 Position, Color Color) : m_CurrentPosition(Position), m_PreviousPosition(Position), m_Acceleration(Vector2{0, 0})
{
    m_Color = Color;
//    m_Color = Color(static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), 255);
}
void RoundBall::giveSpeed(Vector2 Speed) {
    m_PreviousPosition = Vector2Subtract(m_CurrentPosition, Speed);
    m_Velocity = Speed;
}
void RoundBall::draw() {
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius, m_Color);
//    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius, Color(0, 0,0, 255));
}
void RoundBall::update(float DeltaTime) {
    Vector2 DeltaPosition = Vector2Subtract(m_CurrentPosition, m_PreviousPosition);
    m_Velocity = Vector2Scale(Vector2Subtract(m_CurrentPosition, m_PreviousPosition), 1 / DeltaTime);
    m_PreviousPosition = m_CurrentPosition;
    m_CurrentPosition = Vector2Add(m_CurrentPosition, Vector2Add(DeltaPosition, Vector2Scale(m_Acceleration, 0.5f * DeltaTime * DeltaTime)));
    m_Acceleration = Vector2{0, 0};
}
float RoundBall::m_Radius = 10.0f;
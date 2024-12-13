//
// Created by Le Hoang An on 17/10/24.
//
#include "RoundBall.hpp"
#include <random>
VerletRoundBall::VerletRoundBall(Vector2 Position, Color Color) : m_CurrentPosition(Position), m_PreviousPosition(Position), m_Acceleration(Vector2{0, 0})
{
    m_Color = Color;
//    m_Color = Color(static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), static_cast<unsigned char>(rand() % 256), 255);
}
void VerletRoundBall::giveSpeed(Vector2 Speed) {
    m_PreviousPosition = Vector2Subtract(m_CurrentPosition, Speed);
    m_Velocity = Speed;
}
void VerletRoundBall::draw() {
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius, m_Color);
//    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius, Color(0, 0,0, 255));
}
void VerletRoundBall::update(float DeltaTime) {
    Vector2 DeltaPosition = Vector2Subtract(m_CurrentPosition, m_PreviousPosition);
    m_Velocity = Vector2Scale(Vector2Subtract(m_CurrentPosition, m_PreviousPosition), m_SpeedScale / DeltaTime);
    m_PreviousPosition = m_CurrentPosition;
    m_CurrentPosition = Vector2Add(m_CurrentPosition, Vector2Add(Vector2Scale(m_Velocity, DeltaTime), Vector2Scale(m_Acceleration, 0.5f * DeltaTime * DeltaTime)));
    m_Acceleration = Vector2{0, 0};
}
float VerletRoundBall::m_Radius = 10.0f;
EulerianRoundBall::EulerianRoundBall(Vector2 Position, Color Color, float Mass) : m_CurrentPosition(Position), m_PreviousPosition(Position), m_Acceleration(Vector2{0, 0}), m_Mass(Mass)
{
    m_CurrentPosition = Position;
    m_PreviousPosition = Position;
    m_Color = Color;
}
void EulerianRoundBall::accelerate(Vector2 Acceleration) {
    m_Acceleration = Vector2Add(m_Acceleration, Acceleration);
}
void EulerianRoundBall::applyForce(Vector2 Force) {
    m_Acceleration = Vector2Add(m_Acceleration, Vector2Scale(Force, 1 / m_Mass));
}
void EulerianRoundBall::update(float DeltaTime) {
    m_Velocity = Vector2Add(m_Velocity, Vector2Scale(m_Acceleration, DeltaTime));
//    m_Velocity = Vector2Scale(m_Velocity, 0.999994f);
//    m_Velocity = Vector2Scale(m_Velocity, 1.01f); // for stress testing
    m_PreviousPosition = m_CurrentPosition;
    m_CurrentPosition = Vector2Add(m_CurrentPosition, Vector2Scale(m_Velocity, DeltaTime));
    m_Acceleration = Vector2{0, 0};
}
void EulerianRoundBall::draw() {
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius, m_Color);
}
void EulerianRoundBall::drawCustomRadius(float Radius) {
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, Radius, m_Color);
}
void EulerianRoundBall::setVisibility(bool Visible) {
    m_Visible = Visible;
}
bool EulerianRoundBall::isVisble() {
    return m_Visible;
}
bool ClickableEulerianRoundBall::isInside(Vector2 Point) {
    return Vector2Distance(Point, m_CurrentPosition) < m_Radius;
}
ClickableEulerianRoundBall::ClickableEulerianRoundBall(Vector2 Position, Color Color, float Mass) : EulerianRoundBall(Position, Color, Mass) {

}
void ClickableEulerianRoundBall::draw() {
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius - 10.0f, m_Color);
}
VelocityVisualizer::VelocityVisualizer(Vector2 Position, Color Color, float Mass) : EulerianRoundBall(Position, Color, Mass) {
    m_Velocity = {100, 100};
}
void VelocityVisualizer::draw() {
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius, m_Color);
    if (m_VelocityVisualizerOn) {
        float Length = Vector2Length(m_Velocity) / 3000;
        DrawLineEx(m_CurrentPosition, Vector2Add(m_CurrentPosition, Vector2Scale(m_Velocity, Length)), 5, GRAY);
    }
}
void VelocityVisualizer::turnOffVelocityVisualizer() {
    m_VelocityVisualizerOn = false;
}
void VelocityVisualizer::turnOnVelocityVisualizer() {
    m_VelocityVisualizerOn = true;
}
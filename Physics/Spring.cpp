//
// Created by Le Hoang An on 27/10/24.
//

#include "Spring.hpp"
#include "raymath.h"
Spring::Spring(EulerianRoundBall* Ball1, EulerianRoundBall* Ball2, float Length, float k, Color Color) : m_Ball1(Ball1), m_Ball2(Ball2), m_Length(Length), m_k(k),
                                                                                                         m_Color(Color) {

}
void Spring::update() {
    Vector2 Direction = Vector2Subtract(m_Ball2->m_CurrentPosition, m_Ball1->m_CurrentPosition);
    float Distance = Vector2Length(Direction);
    if (Distance != m_Length)
    {
        float Delta = Distance - m_Length;
        Vector2 Normalized = Vector2Normalize(Direction);
        Vector2 Force = Vector2Scale(Normalized, m_k * Delta);
        if (!(m_Ball1 -> m_Fixed)) m_Ball1->applyForce(Force);
        if (!(m_Ball2 -> m_Fixed)) m_Ball2->applyForce(Vector2Negate(Force));
        if (m_Damping)
        {
            Vector2 DampingForce = calculateDampingForce();
            if (!(m_Ball1 -> m_Fixed)) m_Ball1->applyForce(DampingForce);
            if (!(m_Ball2 -> m_Fixed)) m_Ball2->applyForce(Vector2Negate(DampingForce));
        }
    }
}
Vector2 Spring::calculateDampingForce() {
    Vector2 Direction = Vector2Subtract(m_Ball2->m_CurrentPosition, m_Ball1->m_CurrentPosition);
    Direction = Vector2Normalize(Direction);
    float DotProduct = Vector2DotProduct(Vector2Subtract(m_Ball2->m_Velocity, m_Ball1->m_Velocity), Direction);
    return Vector2Scale(Direction, DotProduct * m_DampingFactor);
}
void Spring::draw() {
//    int Height = Vector2Distance(m_Ball1->m_CurrentPosition, m_Ball2->m_CurrentPosition);
    DrawLineAsRectangle(m_Ball1->m_CurrentPosition, m_Ball2->m_CurrentPosition, m_Thickness, m_Color);
}
LineSegment Spring::getLineSegment() {
    return {m_Ball1->m_CurrentPosition, m_Ball2->m_CurrentPosition};
}
pair<EulerianRoundBall*, EulerianRoundBall*> Spring::getBalls() {
    return {m_Ball1, m_Ball2};
}
void Spring::setThickness(float Thickness) {
    if (Thickness < 1) Thickness = 1;
    m_Thickness = Thickness;
}
void DrawLineAsRectangle(Vector2 A, Vector2 B, float thickness, Color color) {
    Vector2 Direction = Vector2Subtract(B, A);           // Vector from A to B
    float Distance = Vector2Length(Direction);           // Length of the rectangle (distance between A and B)
    float Angle = atan2(Direction.y, Direction.x) * 180 / PI; // Angle of the rectangle

    // Top-left corner of the rectangle for alignment
    Vector2 TopLeft = {A.x, A.y};

    // Draw the rectangle with proper alignment and rotation
    DrawRectanglePro(
            (Rectangle){TopLeft.x, TopLeft.y, Distance, thickness}, // Rectangle definition
            (Vector2){0, thickness / 2},                           // Rotation pivot at the left-middle
            Angle,                                                 // Rotation angle
            color                                                  // Rectangle color
    );
}
RubberBand::RubberBand(EulerianRoundBall* Ball1, EulerianRoundBall* Ball2, float Length, float k, Color Color) : Spring(Ball1, Ball2, Length, k, Color) {

}
void RubberBand::update() {
    Vector2 Direction = Vector2Subtract(m_Ball2->m_CurrentPosition, m_Ball1->m_CurrentPosition);
    float Distance = Vector2Length(Direction);
    if (true)
    {
//        std::cout << "Applying Force\n";
        float Delta = Distance - m_Length;
        Vector2 Normalized = Vector2Normalize(Direction);
        Vector2 Force = Vector2Scale(Normalized, m_k * Delta);
        if (!(m_Ball1 -> m_Fixed)) m_Ball1->applyForce(Force);
        if (!(m_Ball2 -> m_Fixed)) m_Ball2->applyForce(Vector2Negate(Force));
        if (m_Damping)
        {
//            cout << "Damping Factor: " << m_DampingFactor << endl;
            Vector2 DampingForce = calculateDampingForce();
            if (!(m_Ball1 -> m_Fixed)) m_Ball1->applyForce(DampingForce);
            if (!(m_Ball2 -> m_Fixed)) m_Ball2->applyForce(Vector2Negate(DampingForce));
        }
    }
}

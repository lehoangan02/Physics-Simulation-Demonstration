//
// Created by Le Hoang An on 27/10/24.
//

#include "Spring.hpp"
Spring::Spring(EulerianRoundBall* Ball1, EulerianRoundBall* Ball2, float Length, float k, Color Color) : m_Ball1(Ball1), m_Ball2(Ball2), m_Length(Length), m_k(k),
                                                                                                         m_Color(Color) {

}
void Spring::update() {
    Vector2 Direction = Vector2Subtract(m_Ball2->m_CurrentPosition, m_Ball1->m_CurrentPosition);
    float Distance = Vector2Length(Direction);
    if (Distance != m_Length)
    {
        float DampingFactor = 0.9;
        float Delta = Distance - m_Length;
        Vector2 Normalized = Vector2Normalize(Direction);
        Vector2 Force = Vector2Scale(Normalized, m_k * Delta);
        if (!(m_Ball1 -> m_Fixed)) m_Ball1->applyForce(Force);
        if (!(m_Ball2 -> m_Fixed)) m_Ball2->applyForce(Vector2Negate(Force));

    }
}
void Spring::draw() {
    int Width = 4;
    int Height = Vector2Distance(m_Ball1->m_CurrentPosition, m_Ball2->m_CurrentPosition);
    DrawLineAsRectangle(m_Ball1->m_CurrentPosition, m_Ball2->m_CurrentPosition, Width, m_Color);
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
//
// Created by Le Hoang An on 27/10/24.
//

#include "Springs.h"
Spring::Spring(EulerianRoundBall* Ball1, EulerianRoundBall* Ball2, float Length, float k) : m_Ball1(Ball1), m_Ball2(Ball2), m_Length(Length), m_k(k) {

}
void Spring::update() {
    Vector2 Direction = Vector2Subtract(m_Ball2->m_CurrentPosition, m_Ball1->m_CurrentPosition);
    float Distance = Vector2Length(Direction);
    if (Distance != m_Length)
    {
        float Delta = Distance - m_Length;
        Vector2 Normalized = Vector2Normalize(Direction);
        Vector2 Force = Vector2Scale(Normalized, m_k * Delta);
        m_Ball1->applyForce(Force);
        m_Ball2->applyForce(Vector2Negate(Force));
    }

}
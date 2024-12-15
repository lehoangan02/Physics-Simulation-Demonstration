//
// Created by Le Hoang An on 25/10/24.
//

#include "Chains.hpp"
#include "raymath.h"
void VerletChain::Link::update() {
    Vector2 Direction = Vector2Subtract(m_Ball2->m_CurrentPosition, m_Ball1->m_CurrentPosition);
    float Distance = Vector2Length(Direction);
    Vector2 Normalized = Vector2Normalize(Direction);
    if (Distance > m_Length)
    {
        Vector2 Correction = Vector2Scale(Normalized, (Distance - m_Length) / 2);
        if (m_Ball1->m_Fixed)
        {
            Vector2 Correction = Vector2Scale(Normalized, (Distance - m_Length));
            m_Ball2->m_CurrentPosition = Vector2Subtract(m_Ball2->m_CurrentPosition, Correction);
            return;
        }
        if (m_Ball2->m_Fixed)
        {
            Vector2 Correction = Vector2Scale(Normalized, (Distance - m_Length));
            m_Ball1->m_CurrentPosition = Vector2Add(m_Ball1->m_CurrentPosition, Correction);
            return;
        }
//        Vector2 Correction = Vector2Scale(Normalized, (Distance - m_Length) / 2);
        m_Ball1->m_CurrentPosition = Vector2Add(m_Ball1->m_CurrentPosition, Correction);
        m_Ball2->m_CurrentPosition = Vector2Subtract(m_Ball2->m_CurrentPosition, Correction);
    }
}
VerletChain::VerletChain(std::vector<VerletRoundBall*> BallList, float Length) : m_Length(Length) {
    for (int i = 0; i < BallList.size() - 1; i++)
    {
        m_Links.push_back(Link(BallList[i], BallList[i + 1], m_Length));
    }
}
void VerletChain::update() {
//    size_t halfSize = m_Links.size() / 2;
//    for (size_t i = 0; i < halfSize; ++i) {
//        m_Links[i].update();
//    }
//    for (size_t i = m_Links.size() - 1; i >= halfSize; --i) {
//        m_Links[i].update();
//    }
    for (int j = 0; j < 3; ++j) { // iterationCount: controls stiffness
        for (size_t i = 0; i < m_Links.size(); ++i) {
            m_Links[i].update();
        }
    }
}
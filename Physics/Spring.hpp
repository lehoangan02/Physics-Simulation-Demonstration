//
// Created by Le Hoang An on 27/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_SPRING_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_SPRING_HPP
#include "RoundBall.hpp"
#include "../Math/Geometry.hpp"
class Spring
{
private:
    float m_Length;
    float m_k;
    EulerianRoundBall* m_Ball1;
    EulerianRoundBall* m_Ball2;
    Color m_Color;
public:
    Spring(EulerianRoundBall* Ball1, EulerianRoundBall* Ball2, float Length, float k, Color Color);
    void update();
    void draw();
};
void DrawLineAsRectangle(Vector2 a, Vector2 b, float thickness, Color color);
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SPRING_HPP

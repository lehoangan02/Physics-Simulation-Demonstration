//
// Created by Le Hoang An on 27/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_SPRING_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_SPRING_HPP
#include "RoundBall.hpp"
#include "../Math/Geometry.hpp"
class Spring
{
protected:
    float m_Length;
    float m_k;
    EulerianRoundBall* m_Ball1;
    EulerianRoundBall* m_Ball2;
    Color m_Color;
    float m_DampingFactor = 1.0f;
    bool m_Damping = false;
protected:
    Vector2 calculateDampingForce();
public:
    Spring(EulerianRoundBall* Ball1, EulerianRoundBall* Ball2, float Length, float k, Color Color);
    LineSegment getLineSegment();
    pair<EulerianRoundBall*, EulerianRoundBall*> getBalls();
    void setDamping(bool Damping) {m_Damping = Damping;}
    void setDamping(bool Damping, float DampingFactor) {m_Damping = Damping; m_DampingFactor = DampingFactor;}
    virtual void update();
    void draw();
    void setColor(Color Color) {m_Color = Color;}
private:
    friend class SpringSoftBodyState;
    friend class PlayableSpringSoftBodyState;
};
class RubberBand : public Spring
{
public:
    RubberBand(EulerianRoundBall* Ball1, EulerianRoundBall* Ball2, float Length, float k, Color Color);
    void update() override;
};
void DrawLineAsRectangle(Vector2 a, Vector2 b, float thickness, Color color);
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SPRING_HPP

//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_ROUNDBALL_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_ROUNDBALL_HPP

#include "raylib.h"
#include "raymath.h"
class VerletRoundBall {
private:
    Vector2 m_CurrentPosition;
    Vector2 m_PreviousPosition;
    Vector2 m_Acceleration;
    Vector2 m_Velocity;
    static float m_Radius;
    Color m_Color;
    bool m_Fixed = false;
private:
    VerletRoundBall(Vector2 Position, Color Color);
    void giveSpeed(Vector2 Speed);
    void update(float DeltaTime);
    void draw();
    friend class VerletEngine;
    friend class VerletDropState;
    friend class VerletChainState;
    friend class VerletChainBasketState;
    friend class VerletChain;
};
class EulerianRoundBall {
private:
    Vector2 m_PreviousPosition;
    Vector2 m_CurrentPosition;
    Vector2 m_Acceleration;
    Vector2 m_Velocity;
    const float m_Radius = 100.0f;
    Color m_Color;
    bool m_Fixed = false;
    float m_Mass = 1.0f;
public:
    EulerianRoundBall(Vector2 Position, Color Color, float Mass);
//    void giveSpeed(Vector2 Speed);
    void accelerate(Vector2 Acceleration);
    void applyForce(Vector2 Force);
    void update(float DeltaTime);
    void draw();
    friend class ContinuousEulerianEngine;
    friend class Spring;
    friend class EulerianDropState;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_ROUNDBALL_HPP

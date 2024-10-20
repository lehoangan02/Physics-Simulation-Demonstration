//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_ROUNDBALL_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_ROUNDBALL_HPP

#include "raylib.h"
#include "raymath.h"
class RoundBall {
private:
    Vector2 m_CurrentPosition;
    Vector2 m_PreviousPosition;
    Vector2 m_Acceleration;
    Vector2 m_Velocity;
    static float m_Radius;
    Color m_Color;
public:
    RoundBall(Vector2 Position);
    void update(float DeltaTime);
    void draw();
    friend class Engine;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_ROUNDBALL_HPP

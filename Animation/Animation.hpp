//
// Created by Le Hoang An on 3/1/25.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_ANIMATION_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_ANIMATION_HPP

#endif //PHYSICS_SIMULATION_DEMONSTRATION_ANIMATION_HPP
#include "raylib.h"
class Animation
{
    Texture2D m_Texture;
    Vector2 m_FrameCount;
    Vector2 m_CurrentFrame;
    float m_TimeStep;
    float m_CurrentTime;
    Rectangle m_SourceRect;
public:
    Animation(Texture2D Texture, Vector2 FrameCount, float TimeStep);
    void update(float DeltaTime);
    void draw(Vector2 Position, float Scale, float Rotation);
};
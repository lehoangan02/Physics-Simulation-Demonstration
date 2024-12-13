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
    float m_SpeedScale = 1.0f;
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
    friend class TunnellingComparisonState;
    friend class EnergyComparisonState;
};
class EulerianRoundBall {
protected:
    Vector2 m_PreviousPosition;
    Vector2 m_CurrentPosition;
    Vector2 m_Acceleration;
    Vector2 m_Velocity = {0, 0};
    float m_Radius = 100.0f;
    Color m_Color;
    bool m_Fixed = false;
    float m_Mass = 1.0f;
    bool m_Visible = true;
public:
    EulerianRoundBall(Vector2 Position, Color Color, float Mass);
//    void giveSpeed(Vector2 Speed);
    void accelerate(Vector2 Acceleration);
    void applyForce(Vector2 Force);
    void update(float DeltaTime);
    virtual void draw();
    void setVisibility(bool Visible);
    bool isVisble();
    virtual void drawCustomRadius(float Radius);
    friend class ContinuousEulerianEngine;
    friend class DiscreteEulerianEngine;
    friend class Grid;
    friend class Cell;
    friend class Spring;
    friend class EulerianDropState;
    friend class TunnellingComparisonState;
    friend class EnergyComparisonState;
    friend class FPSInvariantStateForContinuousIntegration;
    friend class ParticleGravityState;
    friend class Optimization1State;
    friend class SpringState;
    friend class SpringSoftBodyState;
    friend class PlayableSpringSoftBodyState;
    friend class KmeansGroupingState;
    friend class KMeansEngine;
    friend class KMeansOptimizedState;
    friend class CanvasState;
    friend class PressureSoftBodyState;
    friend class RubberBand;
    friend class ShapeMatchingSoftBodyState;
};
class ClickableEulerianRoundBall : public EulerianRoundBall {
public:
    bool isInside(Vector2 Position);
    ClickableEulerianRoundBall(Vector2 Position, Color Color, float Mass);
    void draw() override;
};
class VelocityVisualizer : public EulerianRoundBall {
public:
    void turnOffVelocityVisualizer();
    void turnOnVelocityVisualizer();
    VelocityVisualizer(Vector2 Position, Color Color, float Mass);
    void draw() override;
private:
    bool m_VelocityVisualizerOn = true;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_ROUNDBALL_HPP

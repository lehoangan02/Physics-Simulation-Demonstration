//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP
#include "raylib.h"
#include "raymath.h"
#include "RoundBall.hpp"
#include "PlatformTriangle.hpp"
#include "../Math/Geometry.hpp"
#include "Chains.hpp"
#include <vector>
using namespace std;
class VerletEngine
{
public:
    Vector2 m_Gravity = {0, 400.0f};
    int m_Width;
    int m_Height;
    int m_TotalEngery;
private:
    Texture2D m_Background;
    vector<VerletRoundBall*> m_RoundBallList;
    vector<PlatformTriangle*> m_PlatformTriangleList;
    vector<PlatformTriangle> m_DilatedPlatformTriangleList;
    vector<PlatformTriangle> m_SecondaryDilatedPlatformTriangleList;
    vector<vector<LineSegment>> m_TriangleLineSegmentList;
    vector<vector<LineSegment>> m_SecondaryTriangleLineSegmentList;
    vector<VerletChain*> m_ChainList;
public:
    VerletEngine(int Width, int Height);
    void attachRoundBall(VerletRoundBall* NewRoundBall);
    void attachPlatformTriangle(PlatformTriangle* NewPlatformTriangle);
    void attachChain(VerletChain* NewChain);
    void update(float DeltaTime);
    void draw();
    void reset();
private:
    void accelerate(Vector2 Acceleration);
    void applyConstraints();
    void collidePlatformTriangle();
    void collideRoundBall();
    void printMousePosition();
};
class ContinuousEulerianEngine
{
private:
    Vector2 m_Gravity = {0, 9000.0f};
    int m_Width;
    int m_Height;
    int m_TotalEngery;
    Texture2D m_Background;
    vector<EulerianRoundBall*> m_RoundBallList;
    vector<PlatformTriangle*> m_PlatformTriangleList;
    Line m_GroundLine = Line(0, 1, 0);
    Line m_TopLine = Line(0, 1, -m_Height);
    Line m_LeftLine = Line (1, 0, 0);
    Line m_RightLine = Line(1, 0, -m_Width);
public:
    ContinuousEulerianEngine(int Width, int Height);
    void attachRoundBall(EulerianRoundBall* NewRoundBall);
//    void attachPlatformTriangle(PlatformTriangle* NewPlatformTriangle);
    void update(float DeltaTime);
    void draw();
    void reset();
private:
    void applyConstraints();
    void applyGravity();
    void collideRoundBalls();
    void calculateFinalVelocity(const float &Mass1, const float &Mass2, float& Velocity1, float& Velocity2);
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP

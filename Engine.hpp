//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP
#include "raylib.h"
#include "raymath.h"
#include "RoundBall.hpp"
#include "PlatformTriangle.hpp"
#include "Math/Geometry.hpp"
#include <vector>
using namespace std;
class Engine
{
public:
    const Vector2 m_Gravity = {0, 400.0f};
    int m_Width;
    int m_Height;
    int m_TotalEngery;
private:
    vector<RoundBall*> m_RoundBallList;
    vector<PlatformTriangle*> m_PlatformTriangleList;
    vector<PlatformTriangle> m_DilatedPlatformTriangleList;
    vector<vector<LineSegment>> m_TriangleLineSegmentList;
public:
    Engine(int Width, int Height);
    void attachRoundBall(RoundBall* roundBall);
    void attachPlatformTriangle(PlatformTriangle* platformTriangle);
    void update(float DeltaTime);
    void draw();
private:
    void accelerate(Vector2 Acceleration);
    void applyConstraints();
    void collidePlatformTriangle();
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP

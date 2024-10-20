//
// Created by Le Hoang An on 17/10/24.
//

#include "Engine.hpp"
Engine::Engine(int Width, int Height) : m_Width(Width), m_Height(Height), m_TotalEngery(0) {

}
void Engine::attachRoundBall(RoundBall* roundBall) {
    m_RoundBallList.push_back(roundBall);
}
void Engine::applyConstraints() {
    for (auto& ball : m_RoundBallList)
    {
        if (ball -> m_CurrentPosition.x + ball -> m_Radius > m_Width)
        {
            ball -> m_PreviousPosition.x = ball -> m_CurrentPosition.x;
            ball -> m_CurrentPosition.x = m_Width - ball -> m_Radius;
        }
        if (ball -> m_CurrentPosition.x - ball -> m_Radius < 0)
        {
            ball -> m_PreviousPosition.x = ball -> m_CurrentPosition.x;
            ball -> m_CurrentPosition.x = ball -> m_Radius;
        }
        if (ball -> m_CurrentPosition.y + ball -> m_Radius > m_Height)
        {
            ball -> m_PreviousPosition.y = ball -> m_CurrentPosition.y;
            ball -> m_CurrentPosition.y = m_Height - ball -> m_Radius;
        }
        if (ball -> m_CurrentPosition.y - ball -> m_Radius < 0)
        {
            ball -> m_PreviousPosition.y = ball -> m_CurrentPosition.y;
            ball -> m_CurrentPosition.y = ball -> m_Radius;
        }
    }

}
void Engine::accelerate(Vector2 Acceleration) {
    for (auto& ball : m_RoundBallList)
    {
        ball -> m_Acceleration = Acceleration;
    }
}
void Engine::update(float DeltaTime) {
    accelerate(m_Gravity);
    applyConstraints();
    collidePlatformTriangle();
    for (auto& ball : m_RoundBallList)
    {
        ball -> update(DeltaTime);
    }
}
void Engine::collidePlatformTriangle() {
    for (auto& Ball : m_RoundBallList)
    {
        for (auto& TriangleLineSegment: m_TriangleLineSegmentList)
        {
            if (isInsidePolygon(Ball->m_CurrentPosition, TriangleLineSegment))
            {
//                exit(0);
                DrawText("Collide", 100, 100, 20, RED);
                int i = 0;
                Line FirstLine(TriangleLineSegment[0]);
                int min = FirstLine.distanceToPoint(Ball -> m_CurrentPosition);
                for (int j = 0; j < 3; ++j)
                {
                    float temp = Line(TriangleLineSegment[j]).distanceToPoint(Ball -> m_CurrentPosition);
                    if (min < temp)
                    {
                        min = temp;
                        i = j;
                    }
                }
                Ball -> m_CurrentPosition = Line(TriangleLineSegment[i]).projection(Ball -> m_CurrentPosition);
            }
            else {
                DrawText("Not Collide", 100, 100, 20, GREEN);
            }
        }
    }
}
void Engine::draw() {
    for (auto& dilated: m_DilatedPlatformTriangleList)
    {
        dilated.draw();
    }
    for (auto& platform : m_PlatformTriangleList)
    {
        platform -> draw();
    }
    for (auto& ball : m_RoundBallList) {
        ball->draw();
    }
}
void Engine::attachPlatformTriangle(PlatformTriangle *platformTriangle) {
    m_PlatformTriangleList.push_back(platformTriangle);
    PlatformTriangle DilatedTriangle = platformTriangle ->createDilation(RoundBall::m_Radius);
    m_DilatedPlatformTriangleList.push_back(DilatedTriangle);
    LineSegment AB(DilatedTriangle.m_A, DilatedTriangle.m_B);
    LineSegment BC(DilatedTriangle.m_B, DilatedTriangle.m_C);
    LineSegment CA(DilatedTriangle.m_C, DilatedTriangle.m_A);
    vector<LineSegment> LineSegmentList = {AB, BC, CA};
    m_TriangleLineSegmentList.push_back(LineSegmentList);
}
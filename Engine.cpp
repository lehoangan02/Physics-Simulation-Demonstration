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
    collideRoundBall();
    for (auto& ball : m_RoundBallList)
    {
        ball -> update(DeltaTime);
    }
    printMousePosition();
}
void Engine::collidePlatformTriangle() {
    for (auto& Ball : m_RoundBallList)
    {
//        for (auto& TriangleLineSegment: m_TriangleLineSegmentList)
        for (long k = 0; k < m_TriangleLineSegmentList.size(); ++k)
        {
            vector<LineSegment> TriangleLineSegment = m_TriangleLineSegmentList[k];
            if (isInsidePolygon(Ball->m_CurrentPosition, TriangleLineSegment))
            {
                DrawText("Collide", 100, 100, 20, RED);
                int i = 0;
                Line FirstLine(TriangleLineSegment[0]);
                int min = FirstLine.distanceToPoint(Ball -> m_CurrentPosition);
                for (int j = 0; j < 3; ++j)
                {
                    float temp = Line(TriangleLineSegment[j]).distanceToPoint(Ball -> m_CurrentPosition);
                    if (temp < min)
                    {
                        min = temp;
                        i = j;
                    }
                }
                Vector2 Res = Line(TriangleLineSegment[i]).projection(Ball -> m_CurrentPosition);
                Ball -> m_PreviousPosition = Ball -> m_CurrentPosition;
                LineSegment Secondary = m_SecondaryTriangleLineSegmentList[k][i];
                Ball -> m_CurrentPosition = Line(Secondary).projection(Ball -> m_CurrentPosition);
            }
            else {
//                DrawText("Not Collide", 100, 100, 20, GREEN);
            }
        }
    }
}
void Engine::draw() {
//    for (auto& dilated: m_DilatedPlatformTriangleList)
//    {
//        dilated.draw();
//    }
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
    PlatformTriangle SecondaryDilatedTriangle = DilatedTriangle.createDilation(RoundBall::m_Radius + 0.5f);
    m_SecondaryDilatedPlatformTriangleList.push_back(SecondaryDilatedTriangle);
    LineSegment AB2(SecondaryDilatedTriangle.m_A, SecondaryDilatedTriangle.m_B);
    LineSegment BC2(SecondaryDilatedTriangle.m_B, SecondaryDilatedTriangle.m_C);
    LineSegment CA2(SecondaryDilatedTriangle.m_C, SecondaryDilatedTriangle.m_A);
    vector<LineSegment> LineSegmentList2 = {AB2, BC2, CA2};
    m_SecondaryTriangleLineSegmentList.push_back(LineSegmentList2);
}
void Engine::collideRoundBall()
{
    for (auto& Ball1 : m_RoundBallList)
    {
        for (auto& Ball2 : m_RoundBallList)
        {
            if (Ball1 != Ball2)
            {
                Vector2 DirectVector = Vector2Subtract(Ball2 -> m_CurrentPosition, Ball1 -> m_CurrentPosition);
                float Distance = Vector2Length(DirectVector);
                if (Distance < 2 * RoundBall::m_Radius)
                {
                    Vector2 NormalizedDirectVector = Vector2Normalize(DirectVector);
                    if (!(Ball1->m_Fixed) && !(Ball2 -> m_Fixed)) {
                        Ball2 -> m_CurrentPosition = Vector2Add(Ball2 -> m_CurrentPosition, Vector2Scale(NormalizedDirectVector, 0.5 * (2 * RoundBall::m_Radius - Distance)));
                        Ball1 -> m_CurrentPosition = Vector2Subtract(Ball1 -> m_CurrentPosition, Vector2Scale(NormalizedDirectVector, 0.5 * (2* RoundBall::m_Radius - Distance)));
                    }
                }
            }
        }
    }
}
void Engine::printMousePosition() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        printf("Mouse Position: %f %f\n", GetMousePosition().x, GetMousePosition().y);
    }
}
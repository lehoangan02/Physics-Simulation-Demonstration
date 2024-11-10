//
// Created by Le Hoang An on 17/10/24.
//

#include "Engine.hpp"
VerletEngine::VerletEngine(int Width, int Height) : m_Width(Width), m_Height(Height), m_TotalEngery(0) {
    m_Background = LoadTexture("Assets/Textures/GridBackground.png");
}
void VerletEngine::attachRoundBall(VerletRoundBall* NewRoundBall) {
    m_RoundBallList.push_back(NewRoundBall);
}
void VerletEngine::applyConstraints() {
    for (auto& ball : m_RoundBallList)
    {
        if (ball -> m_CurrentPosition.x + ball -> m_Radius > m_Width)
        {
            ball -> m_PreviousPosition.x = ball -> m_CurrentPosition.x;
            ball -> m_CurrentPosition.x = m_Width - ball -> m_Radius;
        }
        if (ball -> m_CurrentPosition.x - ball -> m_Radius < m_Left)
        {
            ball -> m_PreviousPosition.x = ball -> m_CurrentPosition.x;
            ball -> m_CurrentPosition.x = ball -> m_Radius + m_Left;
        }
        if (ball -> m_CurrentPosition.y + ball -> m_Radius > m_Height)
        {
            ball -> m_PreviousPosition.y = ball -> m_CurrentPosition.y;
            ball -> m_CurrentPosition.y = m_Height - ball -> m_Radius;
        }
        if (ball -> m_CurrentPosition.y - ball -> m_Radius < m_Top)
        {
            ball -> m_PreviousPosition.y = ball -> m_CurrentPosition.y;
            ball -> m_CurrentPosition.y = ball -> m_Radius + m_Top;
        }
    }

}
void VerletEngine::accelerate(Vector2 Acceleration) {
    for (auto& ball : m_RoundBallList)
    {
        if (ball -> m_Fixed)
        {
            ball -> m_Acceleration = Vector2{0, 0};
        }
        else
        {
            ball -> m_Acceleration = Acceleration;
        }
    }
}
void VerletEngine::update(float DeltaTime) {

    accelerate(m_Gravity);
    applyConstraints();
    collidePlatformTriangle();
//    printf("VerletChain Update\n");
    static const int substeps = 5;
    for (int i = 0; i < substeps; ++i){
        collideRoundBall();
    }
    for (auto& chain : m_ChainList)
    {
        chain -> update();
    }
//    printf("VerletChain Update Done\n");


    for (auto& ball : m_RoundBallList) {
        ball->update(DeltaTime);
    }
    printMousePosition();
}
void VerletEngine::collidePlatformTriangle() {
    for (auto& Ball : m_RoundBallList)
    {
//        for (auto& TriangleLineSegment: m_TriangleLineSegmentList)
        for (long k = 0; k < m_TriangleLineSegmentList.size(); ++k)
        {
            vector<LineSegment> TriangleLineSegment = m_TriangleLineSegmentList[k];
            if (isInsidePolygon(Ball->m_CurrentPosition, TriangleLineSegment))
            {
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
                Ball -> m_PreviousPosition = Ball -> m_CurrentPosition;
                LineSegment Secondary = m_SecondaryTriangleLineSegmentList[k][i];
                Ball -> m_CurrentPosition = Line(Secondary).projection(Ball -> m_CurrentPosition);
            }
            else {
            }
        }
    }
}
void VerletEngine::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);
    for (auto& platform : m_PlatformTriangleList)
    {
        platform -> draw();
    }
    for (auto& ball : m_RoundBallList) {
        ball->draw();
    }
    m_TotalEngery = 0;
    for (auto& ball : m_RoundBallList) {
        float Mass = 10.0f;
        m_TotalEngery += Mass * Vector2Length(ball -> m_Velocity) * Vector2Length(ball -> m_Velocity) / 2 + Mass * m_Gravity.y * (m_Height - ball -> m_CurrentPosition.y);
    }
    std::string Engery = "Total Energy Approximation: " + to_string(m_TotalEngery / 100000);
    DrawText(Engery.c_str(), 10, 30, 20, RED);
}
void VerletEngine::attachPlatformTriangle(PlatformTriangle *NewPlatformTriangle) {
    m_PlatformTriangleList.push_back(NewPlatformTriangle);
    PlatformTriangle DilatedTriangle = NewPlatformTriangle ->createDilation(VerletRoundBall::m_Radius);
    m_DilatedPlatformTriangleList.push_back(DilatedTriangle);
    LineSegment AB(DilatedTriangle.m_A, DilatedTriangle.m_B);
    LineSegment BC(DilatedTriangle.m_B, DilatedTriangle.m_C);
    LineSegment CA(DilatedTriangle.m_C, DilatedTriangle.m_A);
    vector<LineSegment> LineSegmentList = {AB, BC, CA};
    m_TriangleLineSegmentList.push_back(LineSegmentList);
    PlatformTriangle SecondaryDilatedTriangle = DilatedTriangle.createDilation(VerletRoundBall::m_Radius + 0.01f);
    m_SecondaryDilatedPlatformTriangleList.push_back(SecondaryDilatedTriangle);
    LineSegment AB2(SecondaryDilatedTriangle.m_A, SecondaryDilatedTriangle.m_B);
    LineSegment BC2(SecondaryDilatedTriangle.m_B, SecondaryDilatedTriangle.m_C);
    LineSegment CA2(SecondaryDilatedTriangle.m_C, SecondaryDilatedTriangle.m_A);
    vector<LineSegment> LineSegmentList2 = {AB2, BC2, CA2};
    m_SecondaryTriangleLineSegmentList.push_back(LineSegmentList2);
}
void VerletEngine::attachChain(VerletChain *NewChain) {
    m_ChainList.push_back(NewChain);
}
void VerletEngine::collideRoundBall()
{
    for (auto& Ball1 : m_RoundBallList)
    {
        for (auto& Ball2 : m_RoundBallList)
        {
            if (Ball1 != Ball2)
            {
                Vector2 DirectVector = Vector2Subtract(Ball2 -> m_CurrentPosition, Ball1 -> m_CurrentPosition);
                float Distance = Vector2Length(DirectVector);
                if (Distance < 2 * VerletRoundBall::m_Radius)
                {
                    Vector2 NormalizedDirectVector = Vector2Normalize(DirectVector);
                    if (!(Ball1->m_Fixed) && !(Ball2 -> m_Fixed)) {
                        Ball2 -> m_CurrentPosition = Vector2Add(Ball2 -> m_CurrentPosition, Vector2Scale(NormalizedDirectVector, 0.5 * (2 * VerletRoundBall::m_Radius - Distance)));
                        Ball1 -> m_CurrentPosition = Vector2Subtract(Ball1 -> m_CurrentPosition, Vector2Scale(NormalizedDirectVector, 0.5 * (2 * VerletRoundBall::m_Radius - Distance)));
                    }
                }
            }
        }
    }
}
void VerletEngine::printMousePosition() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        printf("Mouse Position: %f %f\n", GetMousePosition().x, GetMousePosition().y);
    }
}
void VerletEngine::reset() {
    m_RoundBallList.clear();
    m_PlatformTriangleList.clear();
    m_DilatedPlatformTriangleList.clear();
    m_SecondaryDilatedPlatformTriangleList.clear();
    m_TriangleLineSegmentList.clear();
    m_SecondaryTriangleLineSegmentList.clear();
    m_ChainList.clear();
}
ContinuousEulerianEngine::ContinuousEulerianEngine(int Width, int Height) : m_Width(Width), m_Height(Height), m_TotalEngery(0) {
    m_Background = LoadTexture("Assets/Textures/GridBackground.png");
}
void ContinuousEulerianEngine::attachRoundBall(EulerianRoundBall *NewRoundBall) {
    m_RoundBallList.push_back(NewRoundBall);
}
void ContinuousEulerianEngine::update(float DeltaTime) {
    applyGravity();
    for (auto& ball : m_RoundBallList) {
        ball->update(DeltaTime);
    }
    applyConstraints();
    collideRoundBalls();
}
void ContinuousEulerianEngine::applyConstraints() {
    for (auto &ball : m_RoundBallList) {
//        if (ball->m_Fixed) continue;
        if (ball-> m_CurrentPosition.x + ball->m_Radius > m_Width) {
            Line Delta = Line(ball->m_CurrentPosition, ball -> m_PreviousPosition);
            Line RealRight = Line(1, 0, - m_Width + ball -> m_Radius);
            Vector2 Intersection = Delta.intersection(RealRight);
            float NewY = ball -> m_CurrentPosition.y;
            Line FlippedDelta = Delta.flipVertically(Intersection);
            float NewX;
            if (FlippedDelta.isHorizontal())
            {
                float Excess = ball -> m_CurrentPosition.x - (m_Width - ball -> m_Radius);
                NewX = (m_Width - ball -> m_Radius) - Excess;
            }
            else
            {
                NewX = FlippedDelta.findX(NewY);
            }
            if (NewX - ball -> m_Radius < 0)
            {
                NewX = Intersection.x;
            }
            ball -> m_CurrentPosition = Vector2{NewX, NewY};
            ball -> m_PreviousPosition = Intersection;
            ball -> m_Velocity.x = -(ball -> m_Velocity.x);
        }
        if (ball-> m_CurrentPosition.x - ball->m_Radius < 0) {
            Line Delta = Line(ball->m_CurrentPosition, ball -> m_PreviousPosition);
            Line RealLeft = Line(1, 0,   -ball -> m_Radius);
            Vector2 Intersection = Delta.intersection(RealLeft);
            float NewY = ball -> m_CurrentPosition.y;
            Line FlippedDelta = Delta.flipVertically(Intersection);
            float NewX;
            if (FlippedDelta.isVertical())
            {
                float Excess = ball -> m_CurrentPosition.x - ball -> m_Radius;
                NewX = ball -> m_Radius - Excess;
            }
            else
            {
                NewX = FlippedDelta.findX(NewY);
            }
            if (NewX + ball -> m_Radius > m_Width)
            {
                NewX = Intersection.x;
            }
            ball -> m_CurrentPosition = Vector2{NewX, NewY};
            ball -> m_PreviousPosition = Intersection;
            ball -> m_Velocity.x = -(ball -> m_Velocity.x);
        }
        if (ball-> m_CurrentPosition.y + ball->m_Radius > m_Height) {
            Line Delta = Line(ball->m_CurrentPosition, ball -> m_PreviousPosition);
            Line RealTop = Line(0, 1, -m_Height + ball -> m_Radius);
            Vector2 Intersection = Delta.intersection(RealTop);
            float NewX = ball -> m_CurrentPosition.x;
            Line FlippedDelta = Delta.flipHorizontally(Intersection);
            float NewY;
            if (FlippedDelta.isVertical())
            {
                float Excess = ball -> m_CurrentPosition.y - (m_Height - ball -> m_Radius);
                NewY = (m_Height - ball -> m_Radius) - Excess;
            }
            else
            {
                NewY = FlippedDelta.findY(NewX);
            }
            if (NewY - ball -> m_Radius < 0)
            {
                NewY = Intersection.y;

            }
            ball -> m_CurrentPosition = Vector2{NewX, NewY};
            ball -> m_PreviousPosition = Intersection;
            ball -> m_Velocity.y = -(ball -> m_Velocity.y);
        }
        if (ball-> m_CurrentPosition.y - ball->m_Radius < 0) {
            Line Delta = Line(ball->m_CurrentPosition, ball -> m_PreviousPosition);
            Line RealBottom = Line(0, 1, -ball -> m_Radius);
            Vector2 Intersection = Delta.intersection(RealBottom);
            float NewX = ball -> m_CurrentPosition.x;
            Line FlippedDelta = Delta.flipHorizontally(Intersection);
            float NewY;
            if (FlippedDelta.isVertical())
            {
                float Excess = fabs(ball -> m_CurrentPosition.y - ball -> m_Radius);
                NewY = ball -> m_Radius + Excess;
            }
            else
            {
                NewY = FlippedDelta.findY(NewX);
            }
            if (NewY + ball -> m_Radius > m_Height)
            {
                NewY = Intersection.y;
            }
            ball -> m_CurrentPosition = Vector2{NewX, NewY};
            ball -> m_PreviousPosition = Intersection;
            ball -> m_Velocity.y = -(ball -> m_Velocity.y);
        }
    }
}
void ContinuousEulerianEngine::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);
    for (auto& ball : m_RoundBallList) {
        ball->draw();
    }
    m_TotalEngery = 0;
    for (auto& ball : m_RoundBallList) {
        m_TotalEngery += ball -> m_Mass * Vector2Length(ball -> m_Velocity) * Vector2Length(ball -> m_Velocity) / 2 + ball -> m_Mass * m_Gravity.y * (m_Height - ball -> m_CurrentPosition.y);
    }
    std::string Engery = "Total Energy Approximation: " + to_string(m_TotalEngery / 10000);
    DrawText(Engery.c_str(), 10, 30, 20, RED);
}
void ContinuousEulerianEngine::reset() {
    m_RoundBallList.clear();
    m_PlatformTriangleList.clear();
}
void ContinuousEulerianEngine::applyGravity() {
    for (auto& ball : m_RoundBallList) {
//        if (!(ball -> m_Fixed)) ball->accelerate(m_Gravity);
        ball -> applyForce(m_Gravity);
    }
}
void ContinuousEulerianEngine::collideRoundBalls() {
    for (auto &Ball1: m_RoundBallList) {
        for (auto &Ball2: m_RoundBallList) {
            if (Ball1 != Ball2) {
                if (Ball1->m_Radius + Ball2->m_Radius >
                    Vector2Distance(Ball1->m_CurrentPosition, Ball2->m_CurrentPosition)) {
                    // use pythagorean theorem to calculate the distance between the two balls
                    LineSegment DeltaPosition = LineSegment(Ball1->m_PreviousPosition, Ball1->m_CurrentPosition);
                    Vector2 Projection = DeltaPosition.projection(Ball2->m_CurrentPosition);
                    LineSegment Altitude(Projection, Ball2->m_CurrentPosition);
                    float RadicalProjectionLength = sqrt(
                            (Ball1->m_Radius + Ball2->m_Radius) * (Ball1->m_Radius + Ball2->m_Radius) -
                            Altitude.getLength() * Altitude.getLength());
                    float ProjectionLength = Vector2Distance(Projection, Ball1->m_PreviousPosition);
                    float Offset = ProjectionLength - RadicalProjectionLength;
                    Vector2 CollidingPosition = DeltaPosition.getPointWithDistance(Offset, "A");
                    float DistanceToCollidingPosition = Vector2Distance(CollidingPosition, Ball1->m_CurrentPosition);
                    float TimeSinceCollision = Vector2Distance(Ball1->m_CurrentPosition, CollidingPosition) /
                            Vector2Length(Ball1 -> m_Velocity);
                    Ball1->m_CurrentPosition = CollidingPosition;

                    float Velocity1 = Vector2Length(Ball1->m_Velocity);
                    float Velocity2 = Vector2Length(Ball2->m_Velocity);
                    calculateFinalVelocity(Ball1->m_Mass, Ball2->m_Mass, Velocity1, Velocity2);
                    Vector2 Direction = Vector2Normalize(Vector2Subtract(Ball2->m_CurrentPosition, Ball1->m_CurrentPosition));
                    Ball1 -> m_Velocity = Vector2Scale(Direction, -Velocity1);
                    Ball2 -> m_Velocity = Vector2Scale(Direction, Velocity2);

                    Ball1->m_CurrentPosition = Vector2Add(Ball1->m_CurrentPosition, Vector2Scale(Ball1->m_Velocity, TimeSinceCollision));
                    Ball2->m_CurrentPosition = Vector2Add(Ball2->m_CurrentPosition, Vector2Scale(Ball2->m_Velocity, TimeSinceCollision));
                }
            }
        }
    }
}
void ContinuousEulerianEngine::calculateFinalVelocity(const float &Mass1, const float &Mass2, float &Velocity1,
                                                      float &Velocity2) {
    float v1 = fabs((2 * Mass2 * Velocity2 + (Mass1 - Mass2) * Velocity1) / (Mass1 + Mass2));
    float v2 = fabs((2 * Mass1 * Velocity1 + (Mass2 - Mass1) * Velocity2) / (Mass1 + Mass2));
    Velocity1 = v1;
    Velocity2 = v2;
}
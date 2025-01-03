//
// Created by Le Hoang An on 17/10/24.
//

#include "Engine.hpp"
#include "raymath.h"
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
    for (auto& ball : m_RoundBallList) {
        ball->update(DeltaTime);
    }
    printMousePosition();
}
void VerletEngine::collidePlatformTriangle() {
    for (auto& Ball : m_RoundBallList)
    {
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
    collideRoundBalls(DeltaTime);
    applyConstraints();
}
void ContinuousEulerianEngine::applyConstraints() {
    for (auto &ball : m_RoundBallList) {
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
}
void ContinuousEulerianEngine::applyGravity() {
    for (auto& ball : m_RoundBallList) {
        ball -> applyForce(m_Gravity);
    }
}
void ContinuousEulerianEngine::collideRoundBalls(float DeltaTime) {
    for (auto &Ball1: m_RoundBallList) {
        for (auto &Ball2: m_RoundBallList) {
            if (Ball1 != Ball2) {
                if (Ball1->m_Radius + Ball2->m_Radius >
                    Vector2Distance(Ball1->m_CurrentPosition, Ball2->m_CurrentPosition)) {
                    Vector2 DeltaVelocity = Vector2Subtract(Ball1->m_Velocity, Ball2->m_Velocity);
                    Vector2 DeltaPosition = Vector2Subtract(Ball1->m_PreviousPosition, Ball2->m_PreviousPosition);
                    float A = DeltaVelocity.x * DeltaVelocity.x + DeltaVelocity.y * DeltaVelocity.y;
                    float B = 2 * (DeltaVelocity.x * DeltaPosition.x + DeltaVelocity.y * DeltaPosition.y);
                    float C = DeltaPosition.x * DeltaPosition.x + DeltaPosition.y * DeltaPosition.y - (Ball1->m_Radius + Ball2->m_Radius) * (Ball1->m_Radius + Ball2->m_Radius);
                    QuadraticEquationsSolver Solver;
                    float TimeToCollision = Solver(A, B, C);
                    Ball1 -> m_CurrentPosition = Vector2Add(Ball1 -> m_PreviousPosition, Vector2Scale(Ball1 -> m_Velocity, TimeToCollision));
                    Ball2 -> m_CurrentPosition = Vector2Add(Ball2 -> m_PreviousPosition, Vector2Scale(Ball2 -> m_Velocity, TimeToCollision));

                    Ball1 -> m_PreviousPosition = Ball1 -> m_CurrentPosition;
                    Ball2 -> m_PreviousPosition = Ball2 -> m_CurrentPosition;
                    float TimeSinceCollision = DeltaTime - TimeToCollision;
                    calculateFinalVelocity(Ball1->m_Mass, Ball2->m_Mass, Ball1->m_Velocity, Ball2->m_Velocity);
                    Ball1->m_CurrentPosition = Vector2Add(Ball1->m_CurrentPosition, Vector2Scale(Ball1->m_Velocity, TimeSinceCollision));
                    Ball2->m_CurrentPosition = Vector2Add(Ball2->m_CurrentPosition, Vector2Scale(Ball2->m_Velocity, TimeSinceCollision));
                    if (Ball1->m_Radius + Ball2->m_Radius >
                        Vector2Distance(Ball1->m_CurrentPosition, Ball2->m_CurrentPosition)) {
                        std::cout << "Error: Collision not resolved" << std::endl;
                    }
            }
            }
        }
    }
}
void calculateFinalVelocity(const float &Mass1, const float &Mass2, Vector2 &Velocity1,
                                                      Vector2 &Velocity2) {
    Vector2 v1 = Vector2Scale(Vector2Add(Vector2Scale(Velocity2, 2 * Mass2), Vector2Scale(Velocity1, Mass1 - Mass2)), 1 / (Mass1 + Mass2));
    Vector2 v2 = Vector2Scale(Vector2Add(Vector2Scale(Velocity1, 2 * Mass1), Vector2Scale(Velocity2, Mass2 - Mass1)), 1 / (Mass1 + Mass2));
    Velocity1 = v1;
    Velocity2 = v2;
}
void calculateFinalVelocity(const float &Mass1, const float &Mass2, float &Velocity1,
                                                      float &Velocity2)
{
        float v1 = fabs((2 * Mass2 * Velocity2 + (Mass1 - Mass2) * Velocity1) / (Mass1 + Mass2));
    float v2 = fabs((2 * Mass1 * Velocity1 + (Mass2 - Mass1) * Velocity2) / (Mass1 + Mass2));

    Velocity1 = v1;
    Velocity2 = v2;
}
DiscreteEulerianEngine::DiscreteEulerianEngine(int Width, int Height) : m_Width(Width), m_Height(Height), m_TotalEngery(0) {
    m_Background = LoadTexture("Assets/Textures/GridBackground.png");

}
void DiscreteEulerianEngine::attachRoundBall(EulerianRoundBall *NewRoundBall) {
    m_RoundBallList.push_back(NewRoundBall);
}
void DiscreteEulerianEngine::attachSpring(Spring *NewSpring) {
    m_SpringList.push_back(NewSpring);
}
void DiscreteEulerianEngine::attachRectangle(PlatformRectangle *NewRectangle) {
    m_PlatformRectangleList.push_back(NewRectangle);
}
void DiscreteEulerianEngine::update(float DeltaTime) {
    for (auto& ball : m_RoundBallList) {
        ball->update(DeltaTime);
    }
    if (m_MuttualyAccelerate) accelerateMutually();
    collideRoundBalls();
    collidePlatformRectangle();
    for (auto& string: m_SpringList)
    {
        string -> update();
    }
    if (m_GravityOn) applyGravity();
    applyConstraints();

}
void DiscreteEulerianEngine::applyConstraints() {
    for (auto &Ball : m_RoundBallList) {
        if (Ball-> m_CurrentPosition.x + Ball->m_Radius > m_Width) {
            Ball -> m_CurrentPosition.x = m_Width - Ball -> m_Radius;
            Ball -> m_Velocity.x = -(Ball -> m_Velocity.x);
        }
        if (Ball-> m_CurrentPosition.x - Ball->m_Radius < 0) {
            Ball -> m_CurrentPosition.x = Ball -> m_Radius;
            Ball -> m_Velocity.x = -(Ball -> m_Velocity.x);
        }
        if (Ball-> m_CurrentPosition.y + Ball->m_Radius > m_Height) {
            Ball -> m_CurrentPosition.y = m_Height - Ball -> m_Radius;
            Ball -> m_Velocity.y = -(Ball -> m_Velocity.y);
        }
        if (Ball-> m_CurrentPosition.y - Ball->m_Radius < 0) {
            Ball -> m_CurrentPosition.y = Ball -> m_Radius;
            Ball -> m_Velocity.y = -(Ball -> m_Velocity.y);
        }
    }
}
void DiscreteEulerianEngine::collideRoundBalls() {
    for (auto& Ball1 : m_RoundBallList)
    {
        for (auto& Ball2 : m_RoundBallList)
        {
            if (Ball1 != Ball2)
            {
                Vector2 DirectVector = Vector2Subtract(Ball2 -> m_CurrentPosition, Ball1 -> m_CurrentPosition);
                float Distance = Vector2Length(DirectVector);
                if (Distance < 2 * Ball1 -> m_Radius) {
                    Vector2 NormalizedDirectVector = Vector2Normalize(DirectVector);
                    if (!(Ball1->m_Fixed) && !(Ball2->m_Fixed)) {
                        Ball2->m_CurrentPosition = Vector2Add(Ball2->m_CurrentPosition,
                                                              Vector2Scale(NormalizedDirectVector,
                                                                           0.5 * (2 * Ball1->m_Radius - Distance)));
                        Ball1->m_CurrentPosition = Vector2Subtract(Ball1->m_CurrentPosition,
                                                                   Vector2Scale(NormalizedDirectVector, 0.5 * (2 *
                                                                                                               Ball1->m_Radius -
                                                                                                               Distance)));
                        calculateFinalVelocity(Ball1->m_Mass, Ball2->m_Mass, Ball1->m_Velocity, Ball2->m_Velocity);
                    }
                }
            }
        }
    }
}
void DiscreteEulerianEngine::collidePlatformRectangle() {
    for (auto& ball : m_RoundBallList)
    {
        for (auto& platform : m_PlatformRectangleList)
        {
            if (platform->isInside(ball -> m_CurrentPosition))
            {
                vector<float> DistanceToLineSegment;
                for (auto& lineSegment : platform -> m_LineSegmentList)
                {
                    DistanceToLineSegment.push_back(Line(lineSegment).distanceToPoint(ball -> m_CurrentPosition));
                }
                int k = 0;
                for (int i = 0; i < 4; ++i)
                {
                    if (DistanceToLineSegment[i] < DistanceToLineSegment[k])
                    {
                        k = i;
                    }
                }
                Vector2 Last = ball -> m_CurrentPosition;
                ball -> m_PreviousPosition = ball -> m_CurrentPosition;

                ball -> m_CurrentPosition = Line(platform -> m_LineSegmentList[k]).projection(ball -> m_CurrentPosition);
                ball -> m_CurrentPosition.y += 0.01f;
                Vector2 NormalVector = Vector2Subtract(ball -> m_CurrentPosition, ball -> m_PreviousPosition);
                ball -> m_Velocity = (flipVector(ball -> m_Velocity, NormalVector));
            }
        }
    }
}
void DiscreteEulerianEngine::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);
    for (auto& rectangle : m_PlatformRectangleList)
    {
        rectangle -> draw();
    }
    for (auto& spring : m_SpringList)
    {
        spring -> draw();
    }
    for (auto& ball : m_RoundBallList) {
        if (m_ProximityColoring) ball -> m_Color = calculateColor(ball);
        if (ball -> isVisble()) ball->draw();
    }

}
void DiscreteEulerianEngine::reset() {
    m_RoundBallList.clear();
    m_PlatformRectangleList.clear();
    m_SpringList.clear();
    turnOnGravity();
    turnOnProximityColoring();
}
void DiscreteEulerianEngine::applyGravity() {
    for (auto& ball : m_RoundBallList) {
        if (!(ball -> m_Fixed)) {
            ball->accelerate(m_Gravity);
        }
    }
}
Color DiscreteEulerianEngine::ColorSquare::m_RED = Color(238, 66, 102, 255);
Color DiscreteEulerianEngine::ColorSquare::m_YELLOW = Color(255, 210, 63, 255);
Color DiscreteEulerianEngine::ColorSquare::m_GREEN = Color(51, 115, 87, 255);
Color DiscreteEulerianEngine::ColorSquare::chooseColor(vector<EulerianRoundBall*> m_RoundBallList, EulerianRoundBall* Ball) {
    int Count = 0;
    for (auto& ball : m_RoundBallList)
    {
        if (isInsideSquare(ball))
        {
            ++Count;
        }
    }
    int RedThreshold = 17;
    int YellowThreshold = 10;

    if (Count > RedThreshold)
    {
        return m_RED;
    }
    else if (Count > YellowThreshold)
    {
        return m_YELLOW;
    }
    else
    {
        return m_GREEN;
    }
}
void DiscreteEulerianEngine::accelerateMutually() {
    for (auto& Ball1 : m_RoundBallList)
    {
        for (auto& Ball2 : m_RoundBallList)
        {
            if (Ball1 != Ball2)
            {

                Vector2 DirectVector = Vector2Subtract(Ball2 -> m_CurrentPosition, Ball1 -> m_CurrentPosition);
                float Distance = Vector2Length(DirectVector);
                float Acceleration = 6.67f * 10 * 1 / (Distance * Distance);
                Vector2 AccelerationVector = Vector2Scale(Vector2Normalize(DirectVector), Acceleration);
                if (!(Ball1->m_Fixed) && !(Ball2 -> m_Fixed)) {
                    Ball1 ->accelerate(AccelerationVector);
                    Ball2 ->accelerate(Vector2Scale(AccelerationVector, -1));
                }
            }
        }
    }
}
bool DiscreteEulerianEngine::ColorSquare::isInsideSquare(EulerianRoundBall* Ball) {
    return Ball -> m_CurrentPosition.x > m_TopLeft.x && Ball -> m_CurrentPosition.x < m_TopLeft.x + m_Width && Ball -> m_CurrentPosition.y > m_TopLeft.y && Ball -> m_CurrentPosition.y < m_TopLeft.y + m_Height;
}
Color DiscreteEulerianEngine::calculateColor(EulerianRoundBall *Ball) {
    ColorSquare Square;
    Square.m_Center = Ball -> m_CurrentPosition;
    Square.m_Width = 100.0f;
    Square.m_Height = 100.0f;
    Square.m_TopLeft = Vector2{Square.m_Center.x - Square.m_Width / 2, Square.m_Center.y - Square.m_Height / 2};
    return Square.chooseColor(m_RoundBallList, Ball);
}
void DiscreteEulerianEngine::turnOffGravity() {
    m_GravityOn = false;
}
void DiscreteEulerianEngine::turnOffProximityColoring() {
    m_ProximityColoring = false;
}
void DiscreteEulerianEngine::turnOnGravity() {
    m_GravityOn = true;
}
void DiscreteEulerianEngine::turnOnProximityColoring() {
    m_ProximityColoring = true;
}
void DiscreteEulerianEngine::turnOffMutualAcceleration() {
    m_MuttualyAccelerate = false;
}
void DiscreteEulerianEngine::turnOnMutualAcceleration() {
    m_MuttualyAccelerate = true;
}
void DiscreteEulerianEngine::setGravity(const Vector2 &Gravity) {
    m_Gravity = Gravity;
}

int Grid::m_Width = 0.0f;
int Grid::m_Height = 0.0f;
Grid::Grid(int Width, int Height, int NumRow, int NumColumn) : m_NumRow(NumRow), m_NumColumn(NumColumn) {
    m_Width = Width;
    m_Height = Height;
    float CellWidth = m_Width / m_NumRow;
    float CellHeight = m_Height / m_NumColumn;
    Cell::m_Height = CellHeight;
    Cell::m_Width = CellWidth;
    reset();
}
Grid::~Grid() {
    for (auto& Row : m_CellMatrix)
    {
        for (auto& Cell : Row)
        {
            delete Cell;
        }
    }
}
Cell* Grid::findCell(Vector2 Position) {
    int i = Position.x / (m_Width / m_NumRow);
    int j = Position.y / (m_Height/ m_NumColumn);
    if (i >= m_NumRow || i < 0)
    {
        return nullptr;
    }
    if (j >= m_NumColumn || j < 0)
    {
        return nullptr;
    }
    return m_CellMatrix[i][j];
}
void Grid::attachRoundBall(EulerianRoundBall *NewRoundBall) {
    m_RoundBallList.push_back(NewRoundBall);
}
void Grid::organiseBall(EulerianRoundBall *Ball) {
    Vector2 TopLeft = Vector2Subtract(Ball -> m_CurrentPosition, Vector2{Ball -> m_Radius, Ball -> m_Radius});
    Vector2 TopRight = Vector2Add(Ball -> m_CurrentPosition, Vector2{Ball -> m_Radius, -Ball -> m_Radius});
    Vector2 BottomLeft = Vector2Add(Ball -> m_CurrentPosition, Vector2{-Ball -> m_Radius, Ball -> m_Radius});
    Vector2 BottomRight = Vector2Add(Ball -> m_CurrentPosition, Vector2{Ball -> m_Radius, Ball -> m_Radius});
    vector<Cell*> CellList;
    Cell* TopLeftCell = findCell(TopLeft);
    if (TopLeftCell)
    {
        CellList.push_back(TopLeftCell);
        TopLeftCell -> attachRoundBall(Ball);
    }
    Cell* TopRightCell = findCell(TopRight);
    if (TopRightCell && find(CellList.begin(), CellList.end(), TopRightCell) == CellList.end())
    {
        CellList.push_back(TopRightCell);
        TopRightCell -> attachRoundBall(Ball);
    }
    Cell* BottomLeftCell = findCell(BottomLeft);
    if (BottomLeftCell && find(CellList.begin(), CellList.end(), BottomLeftCell) == CellList.end())
    {
        CellList.push_back(BottomLeftCell);
        BottomLeftCell -> attachRoundBall(Ball);
    }
    Cell* BottomRightCell = findCell(BottomRight);
    if (BottomRightCell && find(CellList.begin(), CellList.end(), BottomRightCell) == CellList.end())
    {
        CellList.push_back(BottomRightCell);
        BottomRightCell -> attachRoundBall(Ball);
    }
}
void Grid::update(float DeltaTime) {
    for (auto& CellRow : m_CellMatrix) {
        for (auto &Cell: CellRow) {
            Cell->m_RoundBallList.clear();
        }
    }
    for (auto& Ball : m_RoundBallList)
    {
        organiseBall(Ball);
    }
    collideRoundBalls();
    applyConstraints();
}
void Grid::applyConstraints() {
    int RowSize = m_CellMatrix.size();
    int ColumnSize = m_CellMatrix[0].size();
    for (int i = 0; i < RowSize; ++i)
    {
        m_CellMatrix[i][ColumnSize - 1] -> applyConstraintBottom();
    }
    for (int i = 0; i < RowSize; ++i)
    {
        m_CellMatrix[i][0] -> applyConstraintTop();
    }
    for (int i = 0; i < ColumnSize; ++i)
    {
        m_CellMatrix[RowSize - 1][i] -> applyConstraintRight();
    }
    for (int i = 0; i < ColumnSize; ++i)
    {
        m_CellMatrix[0][i] -> applyConstraintLeft();
    }
}
void Grid::collideRoundBalls() {
    for (auto& CellRow : m_CellMatrix)
    {
        for (auto& Cell : CellRow)
        {
            Cell -> collideRoundBalls();
        }
    }
}
void Grid::draw() {
    float CellWidth = m_Width / m_NumRow;
    for (int i = 0; i < m_NumRow; ++i)
    {
        Rectangle Rec = {(float)i * CellWidth,0, 1, (float)m_Height};
        DrawRectangleRec(Rec, GRAY);
    }
    float CellHeight = m_Height / m_NumColumn;
    for (int i = 0; i < m_NumColumn; ++i)
    {
        Rectangle Rec = {0, (float)i * CellHeight, (float)m_Width, 1};
        DrawRectangleRec(Rec, GRAY);
    }
    for (auto& CellRow : m_CellMatrix)
    {
        for (auto& Cell : CellRow)
        {
            if (Cell -> m_RoundBallList.size() > 0)
            {
                Cell -> drawRectangle();
            }
        }
    }
}
void Grid::reset() {
    m_RoundBallList.clear();
    for (auto& Row : m_CellMatrix)
    {
        for (auto& Cell : Row)
        {
            delete Cell;
        }
    }
    m_CellMatrix.clear();
    for (int i = 0; i < m_NumRow; ++i)
    {
        vector<Cell*> Column;
        for (int j = 0; j < m_NumColumn; ++j)
        {
            Column.push_back(new Cell(Vector2{Cell::m_Width * i, Cell::m_Height * j}));
        }
        m_CellMatrix.push_back(Column);
    }
}

float Cell::m_Width = 0.0f;
float Cell::m_Height = 0.0f;
Cell::Cell(Vector2 TopLeft) : m_TopLeft(TopLeft) {

}
void Cell::attachRoundBall(EulerianRoundBall *NewRoundBall) {
    m_RoundBallList.push_back(NewRoundBall);
}
void Cell::applyConstraintLeft() {
    for (auto& Ball : m_RoundBallList)
    {
        if (Ball -> m_CurrentPosition.x - Ball -> m_Radius < 0.0f)
        {
            Ball -> m_CurrentPosition.x = 0.0f + Ball -> m_Radius;
            Ball -> m_Velocity.x = -(Ball -> m_Velocity.x);
        }
    }
}
void Cell::applyConstraintRight() {
    for (auto& Ball : m_RoundBallList)
    {
        if (Ball -> m_CurrentPosition.x + Ball -> m_Radius > (float)Grid::m_Width)
        {
            Ball -> m_CurrentPosition.x = (float)Grid::m_Width - Ball -> m_Radius;
            Ball -> m_Velocity.x = -(Ball -> m_Velocity.x);
        }
    }
}
void Cell::applyConstraintTop() {
    for (auto& Ball : m_RoundBallList)
    {
        if (Ball -> m_CurrentPosition.y - Ball -> m_Radius < 0.0f)
        {
            Ball -> m_CurrentPosition.y = 0.0f + Ball -> m_Radius;
            Ball -> m_Velocity.y = -(Ball -> m_Velocity.y);
        }
    }
}
void Cell::applyConstraintBottom() {
    for (auto& Ball : m_RoundBallList)
    {
        if (Ball -> m_CurrentPosition.y + Ball -> m_Radius > (float)Grid::m_Height)
        {
            Ball -> m_CurrentPosition.y = (float)Grid::m_Height - Ball -> m_Radius;
            Ball -> m_Velocity.y = -(Ball -> m_Velocity.y);
        }
    }
}
void Cell::collideRoundBalls() {
    for (auto& Ball1 : m_RoundBallList)
    {
        for (auto& Ball2 : m_RoundBallList)
        {
            if (Ball1 != Ball2)
            {
                Vector2 DirectVector = Vector2Subtract(Ball2 -> m_CurrentPosition, Ball1 -> m_CurrentPosition);
                float Distance = Vector2Length(DirectVector);
                if (Distance < 2 * Ball1 -> m_Radius)
                {
                    Vector2 NormalizedDirectVector = Vector2Normalize(DirectVector);
                    if (!(Ball1->m_Fixed) && !(Ball2 -> m_Fixed)) {
                        Ball2 -> m_CurrentPosition = Vector2Add(Ball2 -> m_CurrentPosition, Vector2Scale(NormalizedDirectVector, 0.5 * (2 * Ball1 -> m_Radius - Distance)));
                        Ball1 -> m_CurrentPosition = Vector2Subtract(Ball1 -> m_CurrentPosition, Vector2Scale(NormalizedDirectVector, 0.5 * (2 * Ball1 -> m_Radius - Distance)));
                    }
                    calculateFinalVelocity(Ball1->m_Mass, Ball2->m_Mass, Ball1->m_Velocity, Ball2->m_Velocity);
                }
            }
        }
    }
}
void Cell::drawOutline() {
    DrawRectangleLines(m_TopLeft.x, m_TopLeft.y, Cell::m_Width, Cell::m_Height, RED);
}
void Cell::drawRectangle() {
    DrawRectangle(m_TopLeft.x, m_TopLeft.y, Cell::m_Width, Cell::m_Height, Color(126, 212, 173, 70));
}

UniformGridEngine::UniformGridEngine(int Width, int Height, int NumRow, int NumColumn) : DiscreteEulerianEngine(Width, Height),
                                                                                         m_Grid(Width, Height, NumRow, NumColumn) {
}
void UniformGridEngine::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);
    m_Grid.draw();
    for (auto& ball : m_RoundBallList) {
        ball->draw();
    }
}
void UniformGridEngine::attachRoundBall(EulerianRoundBall *NewRoundBall) {
    DiscreteEulerianEngine::attachRoundBall(NewRoundBall);
    m_Grid.attachRoundBall(NewRoundBall);
}
void UniformGridEngine::update(float DeltaTime) {
    for (auto& ball : m_RoundBallList) {
        ball->update(DeltaTime);
    }
    m_Grid.update(DeltaTime);
}
void UniformGridEngine::reset() {
    DiscreteEulerianEngine::reset();
    m_Grid.reset();
}
KMeansEngine::KMeansEngine(int Width, int Height, int NumRow, int NumColumn) : UniformGridEngine(Width, Height, NumRow, NumColumn) {
    m_KMeansCalculator = KMeansCalculator::getKMeansCalculator();
    reset();
}
void KMeansEngine::update(float DeltaTime) {
    for (auto &ball: m_RoundBallList) {
        ball->update(DeltaTime);
    }
    changeViewMode();
    accelerateMutally();
    m_Grid.update(DeltaTime);
}
void KMeansEngine::draw() {
//    std::cout << "Drawing" << std::endl;
    DrawTexture(m_Background, 0, 0, WHITE);
    if (m_ViewMode == PLANETARY)
    {
        for (auto& ball : m_RoundBallList) {
            ball -> m_Color = calculateColor(ball);
            VelocityVisualizer *NewBall = static_cast<VelocityVisualizer*>(ball);
            NewBall -> turnOffVelocityVisualizer();
        }
    }
    else if (m_ViewMode == NORMAL)
    {
        for (auto& ball : m_RoundBallList) {
            ball -> m_Color = BLACK;
            VelocityVisualizer *NewBall = static_cast<VelocityVisualizer*>(ball);
            NewBall -> turnOnVelocityVisualizer();
        }
        drawCetroids();
    }
    else if (m_ViewMode == CENTROID) {
        for (auto& ball : m_RoundBallList) {
            VelocityVisualizer *NewBall = static_cast<VelocityVisualizer*>(ball);
            NewBall -> turnOffVelocityVisualizer();
        }
        drawCetroids();
    }
    for (auto &ball: m_RoundBallList) {
        ball->draw();
    }
}
void KMeansEngine::reset() {
    UniformGridEngine::reset();
    m_Centroids.clear();
    m_CentroidColorList.clear();
    m_Data.clear();

    m_Centroids.reserve(m_NumCentroids);
    m_CentroidColorList.reserve(m_NumCentroids);
    m_CentroidColorList.push_back(Color1);
    m_CentroidColorList.push_back(Color2);
    m_CentroidColorList.push_back(Color3);
}
void KMeansEngine::accelerateMutally() {
    m_Data.clear();
    for (auto &ball: m_RoundBallList) {
        m_Data.push_back(ball->m_CurrentPosition);
    }
    static int FrameSkipped = 100;
    if (FrameSkipped == 100) {
        m_Centroids = (*m_KMeansCalculator)(m_Data, m_NumCentroids);
        m_Assignment = m_KMeansCalculator->getAssignment();
        if (m_ViewMode == CENTROID) {
            sortColors();
        }
        FrameSkipped = 0;
    }
    ++FrameSkipped;
    if (m_ViewMode == CENTROID) {
        for (int i = 0; i < m_RoundBallList.size(); ++i) {
            int Centroid = m_Assignment[i];
            m_RoundBallList[i]->m_Color = m_CentroidColorList[Centroid];
        }
    }

    std::vector<float> Weight(m_NumCentroids, 0);
    for (int i = 0; i < m_Assignment.size(); ++i) {
        ++Weight[m_Assignment[i]];
    }
    for (int i = 0; i < m_RoundBallList.size(); ++i) {
        for (int j = 0; j < m_NumCentroids; ++j) {
            Vector2 DirectVector = Vector2Subtract(m_Centroids[j], m_RoundBallList[i]->m_CurrentPosition);
            float Distance = Vector2Length(DirectVector);
            if (Distance == 0) continue;
            float Acceleration = 6.67f * 10000000 / (Distance * Distance);
            if (Acceleration > 10e1) {
                Acceleration = 10e1;
            }
            Vector2 AccelerationVector = Vector2Scale(Vector2Normalize(DirectVector), Acceleration);
            m_RoundBallList[i]->accelerate(AccelerationVector);
        }
    }

}
void KMeansEngine::sortColors() {
    std::vector<std::pair<Vector2, Color>> ColorData;
    for (int i = 0; i < m_NumCentroids; ++i) {
        ColorData.push_back({m_Centroids[i], m_CentroidColorList[i]});
    }
    // find the centroid with the smallest x coordinate
    int MinIndex = 0;
    for (int i = 0; i < m_NumCentroids; ++i)
    {
        if (ColorData[i].first.x < ColorData[MinIndex].first.x)
        {
            MinIndex = i;
        }
    }
    // find the centroid with the largest x coordinate
    int MaxIndex = 0;
    for (int i = 0; i < m_NumCentroids; ++i)
    {
        if (ColorData[i].first.x > ColorData[MaxIndex].first.x)
        {
            MaxIndex = i;
        }
    }
    // find the remaining centroid
    int RemainingIndex = 0;
    for (int i = 0; i < m_NumCentroids; ++i)
    {
        if (i != MinIndex && i != MaxIndex)
        {
            RemainingIndex = i;
        }
    }
    m_CentroidColorList[MinIndex] = Color1;
    m_CentroidColorList[RemainingIndex] = Color2;
    m_CentroidColorList[MaxIndex] = Color3;
}
void KMeansEngine::changeViewMode() {
    if (IsKeyPressed(KEY_ONE))
    {
        m_ViewMode = NORMAL;
    }
    else if (IsKeyPressed(KEY_TWO))
    {
        m_ViewMode = PLANETARY;
    }
    else if (IsKeyPressed(KEY_THREE))
    {
        m_ViewMode = CENTROID;
    }
}
void KMeansEngine::drawCetroids() {
    for (int i = 0; i < m_NumCentroids; ++i) {
        DrawTexture(m_XTexture, m_Centroids[i].x - m_XTextureOffset.x, m_Centroids[i].y - m_XTextureOffset.y, WHITE);
    }
}
DiscreteSATEulerianEngine::DiscreteSATEulerianEngine(int Width, int Height) : m_Width(Width), m_Height(Height), m_CollisionOn(true)
{
    m_Background = LoadTexture("Assets/Textures/GridBackground.png");
}
void DiscreteSATEulerianEngine::attachSATPolygon(SATPlatformPolygon *NewSATPolygon) {
    m_SATPolygonList.push_back(NewSATPolygon);
    NewSATPolygon->setMassUsingArea();
    std::cout << "Mass: " << NewSATPolygon->getMass() << std::endl;
}
void DiscreteSATEulerianEngine::attachSATCircle(SATPlatformCircle *NewSATCircle) {
    m_SATCircleList.push_back(NewSATCircle);
    NewSATCircle->setMassUsingArea();
}
void DiscreteSATEulerianEngine::applyConstraints() {
    if (!m_CollisionOn) return;
    if (m_EngineMode == ENGINE_MODE::INSTANT_ENGINE) {

        for (auto &SATPolygon: m_SATPolygonList) {
            for (auto &vertex: SATPolygon->getVertices()) {
                if (vertex.x < 0) {
                    Vector2 Move = Vector2{-vertex.x, 0};
                    SATPolygon->move(Move);
                }
                if (vertex.x > m_Width) {
                    Vector2 Move = Vector2{m_Width - vertex.x, 0};
                    SATPolygon->move(Move);
                }
                if (vertex.y < 0) {
                    Vector2 Move = Vector2{0, -vertex.y};
                    SATPolygon->move(Move);
                }
                if (vertex.y > m_Height) {

                    Vector2 Move = Vector2{0, m_Height - vertex.y};
                    SATPolygon->move(Move);
                }
            }
        }
        for (auto &SATCircle: m_SATCircleList) {
            if (SATCircle->getCenter().x - SATCircle->getRadius() < 0) {
                Vector2 Move = Vector2{-SATCircle->getCenter().x + SATCircle->getRadius(), 0};
                SATCircle->move(Move);
            }
            if (SATCircle->getCenter().x + SATCircle->getRadius() > m_Width) {
                Vector2 Move = Vector2{m_Width - SATCircle->getCenter().x - SATCircle->getRadius(), 0};
                SATCircle->move(Move);
            }
            if (SATCircle->getCenter().y - SATCircle->getRadius() < 0) {
                Vector2 Move = Vector2{0, -SATCircle->getCenter().y + SATCircle->getRadius()};
                SATCircle->move(Move);
            }
            if (SATCircle->getCenter().y + SATCircle->getRadius() > m_Height) {
                Vector2 Move = Vector2{0, m_Height - SATCircle->getCenter().y - SATCircle->getRadius()};
                SATCircle->move(Move);
            }
        }
    }
    else if (m_EngineMode == ENGINE_MODE::ACCELERATE_ENGINE)
    {
        for (auto &SATPolygon: m_SATPolygonList) {
            for (auto &vertex: SATPolygon->getVertices()) {
                if (vertex.x < 0) {
                    Vector2 Move = Vector2{-vertex.x, 0};
                    SATPolygon->move(Move);
                    SATPolygon->setVelocity(Vector2{-SATPolygon->getVelocity().x, SATPolygon->getVelocity().y});
                    break;
                }
                if (vertex.x > m_Width) {
                    Vector2 Move = Vector2{m_Width - vertex.x, 0};
                    SATPolygon->move(Move);
                    SATPolygon->setVelocity(Vector2{-SATPolygon->getVelocity().x, SATPolygon->getVelocity().y});
                    break;
                }
                if (vertex.y < 0) {
                    Vector2 Move = Vector2{0, -vertex.y};
                    SATPolygon->move(Move);
                    SATPolygon->setVelocity(Vector2{SATPolygon->getVelocity().x, -SATPolygon->getVelocity().y});
                    break;
                }
                if (vertex.y > m_Height) {
                    Vector2 Move = Vector2{0, m_Height - vertex.y};
                    SATPolygon->move(Move);
                    SATPolygon->setVelocity(Vector2{SATPolygon->getVelocity().x, -SATPolygon->getVelocity().y});
                    break;
                }
            }
        }
        for (auto &SATCircle: m_SATCircleList) {
            if (SATCircle->getCenter().x - SATCircle->getRadius() < 0) {
                Vector2 Move = Vector2{-SATCircle->getCenter().x + SATCircle->getRadius(), 0};
                SATCircle->move(Move);
                SATCircle->setVelocity(Vector2{-SATCircle->getVelocity().x, SATCircle->getVelocity().y});
            }
            if (SATCircle->getCenter().x + SATCircle->getRadius() > m_Width) {
                Vector2 Move = Vector2{m_Width - SATCircle->getCenter().x - SATCircle->getRadius(), 0};
                SATCircle->move(Move);
                SATCircle->setVelocity(Vector2{-SATCircle->getVelocity().x, SATCircle->getVelocity().y});
            }
            if (SATCircle->getCenter().y - SATCircle->getRadius() < 0) {
                Vector2 Move = Vector2{0, -SATCircle->getCenter().y + SATCircle->getRadius()};
                SATCircle->move(Move);
                SATCircle->setVelocity(Vector2{SATCircle->getVelocity().x, -SATCircle->getVelocity().y});
            }
            if (SATCircle->getCenter().y + SATCircle->getRadius() > m_Height) {
                Vector2 Move = Vector2{0, m_Height - SATCircle->getCenter().y - SATCircle->getRadius()};
                SATCircle->move(Move);
                SATCircle->setVelocity(Vector2{SATCircle->getVelocity().x, -SATCircle->getVelocity().y});
            }
        }
    }
}
void DiscreteSATEulerianEngine::checkCollision() {
    for (auto& SATPolygon : m_SATPolygonList)
    {
        SATPolygon->setActive(false);
    }
    for (auto& SATCircle : m_SATCircleList)
    {
        SATCircle->setActive(false);
    }
    for (auto& SATPolygon1 : m_SATPolygonList)
    {
        for (auto& SATPolygon2 : m_SATPolygonList)
        {
            if (SATPolygon1 != SATPolygon2)
            {
                SATCollider* Collider = SATPolygonCollider::getSATPolygonCollider();
                if (Collider->isColliding(SATPolygon1->getVertices(), SATPolygon2->getVertices()))
                {
                    SATPolygon1->setActive(true);
                    SATPolygon2->setActive(true);
                }
            }
        }
    }

    for (auto& SATPolygon : m_SATPolygonList)
    {
        for (auto& SATCircle : m_SATCircleList)
        {
            SATCirclePolygonCollider* Collider = SATCirclePolygonCollider::getSATCirclePolygonCollider();
            if (Collider->isColliding(*SATCircle, SATPolygon->getVertices()))
            {
//                std::cout << "Checking Collision" << std::endl;
                SATPolygon->setActive(true);
                SATCircle->setActive(true);
            }
        }
    }
}
void DiscreteSATEulerianEngine::turnOnOffCollision(bool CollisionOn) {
    m_CollisionOn = CollisionOn;
}
void DiscreteSATEulerianEngine::turnOnOffPlayerControl(bool PlayerControlOn) {
    m_PlayerControlOn = PlayerControlOn;
}
void DiscreteSATEulerianEngine::update(float DeltaTime) {
    handlePlayerControl();

    // std::cout << "Contact Points: " << m_ContactPointList.size() << std::endl;
    applyGravity();
    for (auto& SATPolygon : m_SATPolygonList)
    {
        SATPolygon->update(DeltaTime);
    }
    for (auto& SATCircle : m_SATCircleList)
    {
        SATCircle->update(DeltaTime);
    }
    calculateContactPoints();
    collideSATPolygons();
    collideSATCircle();
    applyConstraints();
}
void DiscreteSATEulerianEngine::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);

    for (auto& SATPolygon : m_SATPolygonList)
    {
        SATPolygon->draw();
    }
    for (auto& SATCircle : m_SATCircleList)
    {
        SATCircle->draw();
    }
    drawContactPoints();
    if (m_PlayerControlOn)
    {
        Vector2 End = m_SATPolygonList[0]->getCenter();
        if (m_ObjectTypeToControl == CONTROL_OBJECT::POLYGON)
        {
            if (IsKeyDown(KEY_LEFT))
            {
                End = Vector2Subtract(End, Vector2{50, 0});

            }
            if (IsKeyDown(KEY_RIGHT))
            {
                End = Vector2Add(End, Vector2{50, 0});
            }
            if (IsKeyDown(KEY_UP))
            {
                End = Vector2Subtract(End, Vector2{0, 50});
            }
            if (IsKeyDown(KEY_DOWN))
            {
                End = Vector2Add(End, Vector2{0, 50});
            }
            Color ArrowColor = Color{255, 131, 131, 255};
            drawArrow(m_SATPolygonList[0]->getCenter(), End, ArrowColor, 4);
        }
        else if (m_ObjectTypeToControl == CONTROL_OBJECT::CIRCLE)
        {
            Vector2 End = m_SATCircleList[0]->getCenter();
            if (IsKeyDown(KEY_LEFT))
            {
                End = Vector2Subtract(End, Vector2{50, 0});
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                End = Vector2Add(End, Vector2{50, 0});
            }
            if (IsKeyDown(KEY_UP))
            {
                End = Vector2Subtract(End, Vector2{0, 50});
            }
            if (IsKeyDown(KEY_DOWN))
            {
                End = Vector2Add(End, Vector2{0, 50});
            }
            Color ArrowColor = Color{255, 131, 131, 255};
            drawArrow(m_SATCircleList[0]->getCenter(), End, ArrowColor, 4);
        }
    }

}
void DiscreteSATEulerianEngine::reset() {
    m_SATPolygonList.clear();
    m_SATCircleList.clear();
    m_CollisionOn = true;
    m_PlayerControlOn = false;
}
void DiscreteSATEulerianEngine::handlePlayerControl() {
    if (!m_PlayerControlOn)  return;
    if (m_ObjectTypeToControl == CONTROL_OBJECT::NONE) return;
    if (m_ControlType == CONTROL_TYPE::INSTANT_CONTROL)
    {
        if (m_ObjectTypeToControl == CONTROL_OBJECT::POLYGON)
        {
            if (IsKeyDown(KEY_LEFT))
            {
                m_SATPolygonList[0]->move(Vector2{-2, 0});
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                m_SATPolygonList[0]->move(Vector2{2, 0});
            }
            if (IsKeyDown(KEY_UP))
            {
                m_SATPolygonList[0]->move(Vector2{0, -2});
            }
            if (IsKeyDown(KEY_DOWN))
            {
                m_SATPolygonList[0]->move(Vector2{0, 2});
            }
            if (IsKeyDown(KEY_ONE))
            {
                m_SATPolygonList[0]->rotate(-0.05f);
            }
            if (IsKeyDown(KEY_TWO))
            {
                m_SATPolygonList[0]->rotate(0.05f);
            }
        }
        else if (m_ObjectTypeToControl == CONTROL_OBJECT::CIRCLE)
        {
            if (IsKeyDown(KEY_LEFT))
            {
                m_SATCircleList[0]->move(Vector2{-4, 0});
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                m_SATCircleList[0]->move(Vector2{4, 0});
            }
            if (IsKeyDown(KEY_UP))
            {
                m_SATCircleList[0]->move(Vector2{0, -4});
            }
            if (IsKeyDown(KEY_DOWN)) {
                m_SATCircleList[0]->move(Vector2{0, 4});
            }
        }
    }
    else if (m_ControlType == CONTROL_TYPE::ACCELERATE_CONTROL)
    {
        if (m_ObjectTypeToControl == CONTROL_OBJECT::POLYGON)
        {
            if (IsKeyDown(KEY_LEFT))
            {
                m_SATPolygonList[0]->accelerate(Vector2{-100.0, 0});
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                m_SATPolygonList[0]->accelerate(Vector2{100.0f, 0});
            }
            if (IsKeyDown(KEY_UP))
            {
                m_SATPolygonList[0]->accelerate(Vector2{0, -100.0f});
            }
            if (IsKeyDown(KEY_DOWN))
            {
                m_SATPolygonList[0]->accelerate(Vector2{0, 100.0f});
            }
        }
        else if (m_ObjectTypeToControl == CONTROL_OBJECT::CIRCLE)
        {

            if (IsKeyDown(KEY_LEFT))
            {
                m_SATCircleList[0]->accelerate(Vector2{-100.0f, 0});
            }
            if (IsKeyDown(KEY_RIGHT))
            {
                m_SATCircleList[0]->accelerate(Vector2{100.0f, 0});
            }
            if (IsKeyDown(KEY_UP))
            {
                m_SATCircleList[0]->accelerate(Vector2{0, -100.0f});
            }
            if (IsKeyDown(KEY_DOWN))
            {
                m_SATCircleList[0]->accelerate(Vector2{0, 100.0f});
            }
        }
    }
}
void DiscreteSATEulerianEngine::collideSATPolygons() {
    if (m_EngineMode == ENGINE_MODE::INSTANT_ENGINE)
    {
        collideSATPolygonsInstant();
    }
    else if (m_EngineMode == ENGINE_MODE::ACCELERATE_ENGINE)
    {
        collideSATPolygonsAccelerate();
    }
}
void DiscreteSATEulerianEngine::collideSATCircle() {
    if (m_EngineMode == ENGINE_MODE::INSTANT_ENGINE)
    {
        collideSATCircleInstant();
    }
    else if (m_EngineMode == ENGINE_MODE::ACCELERATE_ENGINE)
    {
        collideSATCircleAccelerate();
    }
}
void DiscreteSATEulerianEngine::collideSATPolygonsInstant()
{
    checkCollision();
    if (m_CollisionOn)
    {
        SATCollider* Collider = SATPolygonCollider::getSATPolygonCollider();
        for (auto& SATPolygon1 : m_SATPolygonList)
        {
            for (auto& SATPolygon2 : m_SATPolygonList)
            {
                if (SATPolygon1 != SATPolygon2)
                {
                    SATPolygon1->move(Collider->getCollisionResolution(SATPolygon1->getVertices(), SATPolygon2->getVertices()).FirstResolution);
                    SATPolygon2->move(Collider->getCollisionResolution(SATPolygon1->getVertices(), SATPolygon2->getVertices()).SecondResolution);

                }
            }
        }
    }
    for (auto& SATPolygon : m_SATPolygonList)
    {
        for (auto& SATCircle : m_SATCircleList)
        {

            SATCirclePolygonCollider* Collider = SATCirclePolygonCollider::getSATCirclePolygonCollider();
            if (Collider->isColliding(*SATCircle, SATPolygon->getVertices()))
            {

                SATCircle->move(Collider->getCollisionResolution(*SATCircle, SATPolygon->getVertices()).FirstResolution);
                SATPolygon->move(Collider->getCollisionResolution(*SATCircle, SATPolygon->getVertices()).SecondResolution);

            }
        }
    }
}
void DiscreteSATEulerianEngine::collideSATCircleInstant()
{
    if (m_CollisionOn)
    {
        for (auto& SATCircle1 : m_SATCircleList)
        {
            for (auto& SATCircle2 : m_SATCircleList)
            {
                if (SATCircle1 != SATCircle2)
                {
                    if (Vector2Distance(SATCircle1->getCenter(), SATCircle2->getCenter()) < SATCircle1->getRadius() + SATCircle2->getRadius())
                    {
                        Vector2 NormalizedDirectVector = Vector2Normalize(Vector2Subtract(SATCircle2->getCenter(), SATCircle1->getCenter()));
                        SATCircle2->move(Vector2Scale(NormalizedDirectVector, 0.5 * (SATCircle1->getRadius() + SATCircle2->getRadius() - Vector2Distance(SATCircle1->getCenter(), SATCircle2->getCenter()))));
                        SATCircle1->move(Vector2Scale(NormalizedDirectVector, -0.5 * (SATCircle1->getRadius() + SATCircle2->getRadius() - Vector2Distance(SATCircle1->getCenter(), SATCircle2->getCenter()))));

                    }
                }
            }
        }
    }
}
void DiscreteSATEulerianEngine::collideSATPolygonsAccelerate() {
    for (auto &SATPolygon1: m_SATPolygonList)
    {
        for (auto &SATPolygon2: m_SATPolygonList)
        {
            if (SATPolygon1 != SATPolygon2)
            {
                SATCollider* Collider = SATPolygonCollider::getSATPolygonCollider();
                if (Collider->isColliding(SATPolygon1->getVertices(), SATPolygon2->getVertices()))
                {
                    vector<Vector2> Vertices1 = SATPolygon1->getVertices();
                    vector<Vector2> Vertices2 = SATPolygon2->getVertices();
                    vector<LineSegment> Edges2;
                    for (int i = 0; i < Vertices2.size(); ++i)
                    {
                        LineSegment Edge = {Vertices2[i], Vertices2[(i + 1) % Vertices2.size()]};
                        Edges2.push_back(Edge);
                    }
                    int IndexInside2 = 0;
                    float MaxPenetration = -INT_MAX;
                    for (int i = 0; i < Vertices1.size(); ++i) {
                        float Penetration = 0.0f;
                        for (auto &Edge : Edges2) {
                            Line LineEdge(Edge);
                            Penetration = LineEdge.distanceToPoint(Vertices1[i]);
                            if (isInsidePolygon(Vertices1[i], Edges2) && Penetration > MaxPenetration) {
                                MaxPenetration = Penetration;
                                IndexInside2 = i;
                            }
                        }
                    }
                    int ClosestEdgeIndex = 0;
                    for (int i = 0; i < Edges2.size(); ++i)
                    {
                        Line Line2(Edges2[i]);
                        Line MinLine2(Edges2[ClosestEdgeIndex]);
                        if (Line2.distanceToPoint(Vertices1[IndexInside2]) < MinLine2.distanceToPoint(Vertices1[IndexInside2]))
                        {
                            ClosestEdgeIndex = i;
                        }

                    }
                    Vector2 Projection = Line(Edges2[ClosestEdgeIndex]).projection(Vertices1[IndexInside2]);
                    Vector2 Normal = Vector2Normalize(Vector2Subtract(Projection, Vertices1[IndexInside2]));
                    Normal = Vector2Normalize(Normal);
                    Vector2 RelativeVelocity = Vector2Subtract(SATPolygon2->getVelocity(), SATPolygon1->getVelocity());
                    float e = 1.0f; // coefficient of restitution
                    // e = 0.5f;
                    float Mass1 = SATPolygon1->getMass();
                    float Mass2 = SATPolygon2->getMass();
                    float j = -(1 + e) * Vector2DotProduct(RelativeVelocity, Normal) /
                              (1 / Mass1 + 1 / Mass2);
                    SATPolygon1->addVelocity(Vector2Scale(Normal, -j / Mass1));
                    SATPolygon2->addVelocity(Vector2Scale(Normal, +j / Mass2));
                    SATPolygon1->move(Collider->getCollisionResolution(SATPolygon1->getVertices(), SATPolygon2->getVertices()).FirstResolution);
                    SATPolygon2->move(Collider->getCollisionResolution(SATPolygon1->getVertices(), SATPolygon2->getVertices()).SecondResolution);
                }
            }
        }
    }
    for (auto& SATPolygon : m_SATPolygonList)
    {
        for (auto& SATCircle : m_SATCircleList)
        {
            SATCirclePolygonCollider* Collider = SATCirclePolygonCollider::getSATCirclePolygonCollider();
            if (Collider->isColliding(*SATCircle, SATPolygon->getVertices()))
            {
                // find if there is a vertex inside the circle
                bool Inside = false;
                vector<Vector2> Vertices = SATPolygon->getVertices();
                for (int i = 0; i < Vertices.size(); ++i)
                {
                    if (Vector2Distance(SATCircle->getCenter(), Vertices[i]) < SATCircle->getRadius())
                    {
                        Inside = true;
                        break;
                    }
                }
                if (Inside) {
                    // find the closest vertex to the circle center
                    int ClosestVertexIndex = 0;
                    for (int i = 0; i < Vertices.size(); ++i)
                    {
                        if (Vector2Distance(SATCircle->getCenter(), Vertices[i]) < Vector2Distance(SATCircle->getCenter(), Vertices[ClosestVertexIndex]))
                        {
                            ClosestVertexIndex = i;
                        }
                    }
                    Vector2 Normal = Vector2Normalize(Vector2Subtract(SATCircle->getCenter(), Vertices[ClosestVertexIndex]));
                    Vector2 RelativeVelocity = Vector2Subtract(SATCircle->getVelocity(), SATPolygon->getVelocity());
                    float e = 1.0f; // coefficient of restitution
                    float Mass1 = SATPolygon->getMass();
                    float Mass2 = SATCircle->getMass();
                    // std::cout << "Real mass 1: " << SATPolygon->getMass() << std::endl;
                    // std::cout << "Real mass 2: " << SATCircle->getMass() << std::endl;
                    float j = -(1 + e) * Vector2DotProduct(RelativeVelocity, Normal) /
                              (1 / Mass1 + 1 / Mass2);
                    SATPolygon->addVelocity(Vector2Scale(Normal, -j / Mass1));
                    SATCircle->addVelocity(Vector2Scale(Normal, +j / Mass2));
                    Vector2 FirstResolution = Collider->getCollisionResolution(*SATCircle, SATPolygon->getVertices()).FirstResolution;
                    Vector2 SecondResolution = Collider->getCollisionResolution(*SATCircle, SATPolygon->getVertices()).SecondResolution;
                    SATCircle->move(FirstResolution);
                    SATPolygon->move(SecondResolution);
                }
                else {
                    // find the closest edge to the circle center
                    vector<Vector2> Vertices = SATPolygon->getVertices();
                    vector<LineSegment> Edges;
                    for (int i = 0; i < Vertices.size(); ++i)
                    {
                        LineSegment Edge = {Vertices[i], Vertices[(i + 1) % Vertices.size()]};
                        Edges.push_back(Edge);
                    }
                    vector<bool> HaveProjection(Edges.size(), false);
                    int ClosestEdgeIndex;
                    for (int i = 0; i < Edges.size(); ++i)
                    {
                        if (Edges[i].haveProjection(SATCircle->getCenter()))
                        {
                            HaveProjection[i] = true;
                            ClosestEdgeIndex = i;
                        }
                    }
                    for (int i = 0; i < Edges.size(); ++i)
                    {
                        if (!HaveProjection[i]) continue;
                        Line LineEdge(Edges[i]);
                        Line MinLineEdge(Edges[ClosestEdgeIndex]);
                        if (LineEdge.distanceToPoint(SATCircle->getCenter()) < MinLineEdge.distanceToPoint(SATCircle->getCenter()))
                        {
                            ClosestEdgeIndex = i;
                        }
                    }
                    Line ClosestEdgeLine(Edges[ClosestEdgeIndex]);
                    Vector2 Projection = ClosestEdgeLine.projection(SATCircle->getCenter());
                    Vector2 Normal = Vector2Normalize(Vector2Subtract(Projection, SATCircle->getCenter()));
                    Vector2 RelativeVelocity = Vector2Subtract(SATCircle->getVelocity(), SATPolygon->getVelocity());
                    float e = 1.0f; // coefficient of restitution
                    float Mass1 = SATPolygon->getMass();
                    float Mass2 = SATCircle->getMass();
                    float j = -(1 + e) * Vector2DotProduct(RelativeVelocity, Normal) /
                              (1 / Mass1 + 1 / Mass2);
                    SATPolygon->addVelocity(Vector2Scale(Normal, -j / Mass1));
                    SATCircle->addVelocity(Vector2Scale(Normal, +j / Mass2));
                    Vector2 FirstResolution = Collider->getCollisionResolution(*SATCircle, SATPolygon->getVertices()).FirstResolution;
                    Vector2 SecondResolution = Collider->getCollisionResolution(*SATCircle, SATPolygon->getVertices()).SecondResolution;

                    SATCircle->move(FirstResolution);
                    SATPolygon->move(SecondResolution);
                }
            }
        }
    }
}
void DiscreteSATEulerianEngine::collideSATCircleAccelerate() {
    for (auto& circle1: m_SATCircleList)
    {
        for (auto& circle2: m_SATCircleList)
        {
            if (circle1 != circle2)
            {
                Vector2 DirectVector = Vector2Subtract(circle2->getCenter(), circle1->getCenter());
                float Distance = Vector2Length(DirectVector);
                if (Distance < circle1->getRadius() + circle2->getRadius()) {
                    Vector2 NormalizedDirectVector = Vector2Normalize(DirectVector);
                    circle2->move(Vector2Scale(NormalizedDirectVector,
                                               0.5 * (circle1->getRadius() + circle2->getRadius() - Distance)));
                    circle1->move(Vector2Scale(NormalizedDirectVector,
                                               -0.5 * (circle1->getRadius() + circle2->getRadius() - Distance)));

                    Vector2 NormalVector = Vector2Normalize(DirectVector);
                    float e = 1.0f; // coefficient of restitution
                    Vector2 RelativeVelocity = Vector2Subtract(circle2->getVelocity(), circle1->getVelocity());
                    float j = -(1 + e) * Vector2DotProduct(RelativeVelocity, NormalVector) /
                              (1 / circle1->getMass() + 1 / circle2->getMass());
                    circle1->addVelocity(Vector2Scale(NormalVector, -j / circle1->getMass()));
                    circle2->addVelocity(Vector2Scale(NormalVector, +j / circle2->getMass()));
                }
            }
        }
    }
}
void DiscreteSATEulerianEngine::setControlType(CONTROL_TYPE ControlType) {
    m_ControlType = ControlType;
    if (m_ControlType == CONTROL_TYPE::INSTANT_CONTROL)
    {
        m_EngineMode = ENGINE_MODE::INSTANT_ENGINE;
        this -> turnOnOffGravity(false);
    }
    else if (m_ControlType == CONTROL_TYPE::ACCELERATE_CONTROL)
    {
        m_EngineMode = ENGINE_MODE::ACCELERATE_ENGINE;
    }
}
void DiscreteSATEulerianEngine::setEngineMode(ENGINE_MODE EngineMode) {
    m_EngineMode = EngineMode;
    if (m_EngineMode == ENGINE_MODE::INSTANT_ENGINE)
    {
        this -> turnOnOffGravity(false);
        m_ControlType = CONTROL_TYPE::INSTANT_CONTROL;
    }
    else if (m_EngineMode == ENGINE_MODE::ACCELERATE_ENGINE)
    {
        m_ControlType = CONTROL_TYPE::ACCELERATE_CONTROL;
    }
}
void DiscreteSATEulerianEngine::setObjectTypeToControl(int ObjectTypeToControl) {
    m_ObjectTypeToControl = ObjectTypeToControl;
}
void DiscreteSATEulerianEngine::applyGravity() {
    if (!m_ApplyGravity) return;
    Vector2 Gravity = Vector2{0, 9.8 * 10};
    for (auto& SATPolygon : m_SATPolygonList)
    {
        SATPolygon->accelerate(Gravity);
    }
    for (auto& SATCircle : m_SATCircleList)
    {
        SATCircle->accelerate(Gravity);
    }
}
void DiscreteSATEulerianEngine::calculateContactPoints() {

    if (!m_DisplayContactPoint) return;
    for (auto& SATCircle1 : m_SATCircleList) {
        for (auto& SATCircle2 : m_SATCircleList) {
            if (SATCircle1 != SATCircle2) {
                Vector2 DirectVector = Vector2Subtract(SATCircle2->getCenter(), SATCircle1->getCenter());
                float Distance = Vector2Length(DirectVector);
                if (Distance < SATCircle1->getRadius() + SATCircle2->getRadius()) {
                    Vector2 ContactPoint = Vector2Add(SATCircle1->getCenter(), Vector2Scale(DirectVector, SATCircle1->getRadius() / (SATCircle1->getRadius() + SATCircle2->getRadius())));
                    m_ContactPointList.push_back(ContactPoint);
                }
            }
        }
    }
    for (auto& SATPolygon1 : m_SATPolygonList) {
        for (auto& SATPolygon2 : m_SATPolygonList) {
            if (SATPolygon1 != SATPolygon2) {
                SATCollider* Collider = SATPolygonCollider::getSATPolygonCollider();
                if (Collider -> isColliding(SATPolygon1->getVertices(), SATPolygon2->getVertices())) {
//                    std::cout << "Colliding" << std::endl;
                    vector<Vector2> VerticesWithProjection1;
                    vector<Vector2> VerticesWithProjection2;
                    vector<LineSegment> LineSegments1;
                    for (int i = 0; i < SATPolygon1->getVertices().size(); ++i) {
                        LineSegment Edge = {SATPolygon1->getVertices()[i], SATPolygon1->getVertices()[(i + 1) % SATPolygon1->getVertices().size()]};
                        LineSegments1.push_back(Edge);
                    }
                    vector<LineSegment> LineSegments2;
                    for (int i = 0; i < SATPolygon2->getVertices().size(); ++i) {
                        LineSegment Edge = {SATPolygon2->getVertices()[i], SATPolygon2->getVertices()[(i + 1) % SATPolygon2->getVertices().size()]};
                        LineSegments2.push_back(Edge);
                    }
                    for (auto& Vertex : SATPolygon1->getVertices()) {
                        for (auto& Edge : LineSegments2) {
                            if (Edge.haveProjection(Vertex)) {
                                VerticesWithProjection1.push_back(Vertex);
                                break;
                            }
                        }
                    }
                    for (auto& Vertex : SATPolygon2->getVertices()) {
                        for (auto& Edge : LineSegments1) {
                            if (Edge.haveProjection(Vertex)) {
                                VerticesWithProjection2.push_back(Vertex);
                                break;
                            }
                        }
                    }
                    // std::cout << "Number of vertices with projection 1: " << VerticesWithProjection1.size() << std::endl;
                    // std::cout << "Number of vertices with projection 2: " << VerticesWithProjection2.size() << std::endl;
                    const float InaccuracyThreshold = 0.000000001f;
                    float MinDistance1 = INT_MAX;
                    int MinimumDistanceIndex1 = 0;
                    for (int i = 0; i < VerticesWithProjection1.size(); ++i) {
                        float LocalMinDistance = INT_MAX;
                        for (auto& Edge : LineSegments2) {
                            float Distance = Line(Edge).distanceToPoint(VerticesWithProjection1[i]);
                            if (Distance < LocalMinDistance) {
                                LocalMinDistance = Distance;
                            }
                        }
                        if (LocalMinDistance < MinDistance1) {
                            MinDistance1 = LocalMinDistance;
                            MinimumDistanceIndex1 = i;
                        }
                    }
                    float MinDistance2 = INT_MAX;
                    int MinimumDistanceIndex2 = 0;
                    for (int i = 0; i < VerticesWithProjection2.size(); ++i) {
                        float LocalMinDistance = INT_MAX;
                        for (auto& Edge : LineSegments1) {
                            float Distance = Line(Edge).distanceToPoint(VerticesWithProjection2[i]);
                            if (Distance < LocalMinDistance) {
                                LocalMinDistance = Distance;
                            }
                        }
                        if (LocalMinDistance < MinDistance2) {
                            MinDistance2 = LocalMinDistance;
                            MinimumDistanceIndex2 = i;
                        }
                    }
                    float MinimumDistance;
                    if (MinDistance1 < MinDistance2) {
                        MinimumDistance = MinDistance1;
                    }
                    else {
                        MinimumDistance = MinDistance2;
                    }
                    for (int i = 0; i < VerticesWithProjection1.size(); ++i) {
                        for (auto& Edge : LineSegments2) {
                            if (!Edge.haveProjection(VerticesWithProjection1[i])) continue;
                            float Distance = Line(Edge).distanceToPoint(VerticesWithProjection1[i]);
                            if (Distance <= MinimumDistance + InaccuracyThreshold) {
                                Vector2 Projection = Line(Edge).projection(VerticesWithProjection1[i]);
                                m_ContactPointList.push_back(Projection);
                                break;
                            }
                        }
                    }
                    for (int i = 0; i < VerticesWithProjection2.size(); ++i) {
                        for (auto& Edge : LineSegments1) {
                            if (!(Edge.haveProjection(VerticesWithProjection2[i]))) continue;
                            float Distance = Line(Edge).distanceToPoint(VerticesWithProjection2[i]);
                            if (Distance <= MinimumDistance + InaccuracyThreshold) {
                                Vector2 Projection = Line(Edge).projection(VerticesWithProjection2[i]);
                                m_ContactPointList.push_back(Projection);
                                break;
                            }
                        }
                    }
                    // BeginDrawing();
                    // for (auto& Point : m_ContactPointList) {
                    //     DrawCircle(Point.x, Point.y, 5, RED);
                    // }
                    // EndDrawing();
                    std::cout << "Number of contact points: " << m_ContactPointList.size() << std::endl;
                }
            }
        }
    }
    for (auto& SATPolygon : m_SATPolygonList)
    {
        for (auto& SATCircle : m_SATCircleList)
        {
            SATCirclePolygonCollider* Collider = SATCirclePolygonCollider::getSATCirclePolygonCollider();
            if (Collider->isColliding(*SATCircle, SATPolygon->getVertices()))
            {
                // find if there is a vertex inside the circle
                bool Inside = false;
                vector<Vector2> Vertices = SATPolygon->getVertices();
                for (int i = 0; i < Vertices.size(); ++i)
                {
                    if (Vector2Distance(SATCircle->getCenter(), Vertices[i]) < SATCircle->getRadius())
                    {
                        Inside = true;
                        break;
                    }
                }
                if (Inside) {
                    // find the closest vertex to the circle center
                    int ClosestVertexIndex = 0;
                    for (int i = 0; i < Vertices.size(); ++i)
                    {
                        if (Vector2Distance(SATCircle->getCenter(), Vertices[i]) < Vector2Distance(SATCircle->getCenter(), Vertices[ClosestVertexIndex]))
                        {
                            ClosestVertexIndex = i;
                        }
                    }
                    Vector2 Projection = Vertices[ClosestVertexIndex];
                    m_ContactPointList.push_back(Projection);
                }
                else {
                    // find the closest edge to the circle center
                    vector<Vector2> Vertices = SATPolygon->getVertices();
                    vector<LineSegment> Edges;
                    for (int i = 0; i < Vertices.size(); ++i)
                    {
                        LineSegment Edge = {Vertices[i], Vertices[(i + 1) % Vertices.size()]};
                        Edges.push_back(Edge);
                    }
                    vector<bool> HaveProjection(Edges.size(), false);
                    int ClosestEdgeIndex;
                    for (int i = 0; i < Edges.size(); ++i)
                    {
                        if (Edges[i].haveProjection(SATCircle->getCenter()))
                        {
                            HaveProjection[i] = true;
                            ClosestEdgeIndex = i;
                        }
                    }
                    for (int i = 0; i < Edges.size(); ++i)
                    {
                        if (!HaveProjection[i]) continue;
                        Line LineEdge(Edges[i]);
                        Line MinLineEdge(Edges[ClosestEdgeIndex]);
                        if (LineEdge.distanceToPoint(SATCircle->getCenter()) < MinLineEdge.distanceToPoint(SATCircle->getCenter()))
                        {
                            ClosestEdgeIndex = i;
                        }
                    }
                    Line ClosestEdgeLine(Edges[ClosestEdgeIndex]);
                    Vector2 Projection = ClosestEdgeLine.projection(SATCircle->getCenter());
                    m_ContactPointList.push_back(Projection);
                }
            }
        }
    }
}
void DiscreteSATEulerianEngine::drawContactPoints() {
    if (!m_DisplayContactPoint) return;
    for (auto& Point : m_ContactPointList) {
        drawSquare(Point, 10, MAGENTA);
    }
    static int FrameSkipped = 0;
    const int FrameSkip = 20;
    if (FrameSkipped == FrameSkip) {
        m_ContactPointList.clear();
        FrameSkipped = 0;
    }
    ++FrameSkipped;
}
DiscreteRotatingEulerianEngine::DiscreteRotatingEulerianEngine(int Width, int Height) : DiscreteSATEulerianEngine(Width, Height) {
    m_ApplyGravity = false;
}
void DiscreteRotatingEulerianEngine::attachRotatingPlatformCircle(SATRotatingPlatformCircle *NewRotatingPlatformCircle) {
    m_RotatingCircleList.push_back(NewRotatingPlatformCircle);
}
void DiscreteRotatingEulerianEngine::attachRotatingPlatformPolygon(SATRotatingPlatformPolygon *NewRotatingPlatformPolygon) {
    m_RotatingPolygonList.push_back(NewRotatingPlatformPolygon);
    if (!(NewRotatingPlatformPolygon->isFixed()))
    {
        NewRotatingPlatformPolygon->setMassUsingArea();
    }
//    NewRotatingPlatformPolygon->cal
}
void DiscreteRotatingEulerianEngine::update(float DeltaTime) {
    applyGravity();
    for (auto& RotatingCircle : m_RotatingCircleList)
    {
        RotatingCircle->update(DeltaTime);
    }
    for (auto& RotatingPolygon : m_RotatingPolygonList)
    {
        RotatingPolygon->update(DeltaTime);
    }
    calculateContactPoints();
    collideSATCircle();
    applyConstraints();
}
void DiscreteRotatingEulerianEngine::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);
    for (auto& SATPolygon : m_RotatingPolygonList) {
        SATPolygon->draw();
    }
    for (auto& SATCircle : m_RotatingCircleList) {
        SATCircle->draw();
    }
    collideSATPolygons();
    drawContactPoints();
}
void DiscreteRotatingEulerianEngine::reset() {

    m_RotatingCircleList.clear();
    m_RotatingPolygonList.clear();
}
void DiscreteRotatingEulerianEngine::collideSATCircle() {
}
void DiscreteRotatingEulerianEngine::collideSATPolygons() {
    for (int i = 0; i < m_RotatingPolygonList.size(); ++i)
    {
        SATRotatingPlatformPolygon* Polygon1 = m_RotatingPolygonList[i];
        for (int j = i + 1; j < m_RotatingPolygonList.size(); ++j)
        {
            SATRotatingPlatformPolygon* Polygon2 = m_RotatingPolygonList[j];
            if (Polygon1 != Polygon2)
            {
                SATRotatingCollider* Collider = SATRotatingCollider::getSATRotatingCollider();
                SATPolygonCollider* PolygonCollider = dynamic_cast<SATPolygonCollider*>(SATPolygonCollider::getSATPolygonCollider());
                if (PolygonCollider->isColliding(Polygon1->getVertices(), Polygon2->getVertices()))
                {

                    AngularCollisionResolve Result = Collider->getCollisionResolution(Polygon1, Polygon2);
                    Result.FirstPositionResolution = Vector2Scale(Result.FirstPositionResolution, 1.1f);
                    Result.SecondPositionResolution = Vector2Scale(Result.SecondPositionResolution, 1.1f);
                    Polygon1->move(Result.FirstPositionResolution);
                    Polygon2->move(Result.SecondPositionResolution);
//                    std::cout << "First velocity resolution: " << Result.FirstVelocityResolution.x << " " << Result.FirstVelocityResolution.y << std::endl;
//                    std::cout << "Second velocity resolution: " << Result.SecondVelocityResolution.x << " " << Result.SecondVelocityResolution.y << std::endl;
                    Polygon1->addVelocity(Result.FirstVelocityResolution);
                    Polygon2->addVelocity(Result.SecondVelocityResolution);
                    Polygon1->addRotationalVelocity(Result.FirstAngularResolution);
                    Polygon2->addRotationalVelocity(Result.SecondAngularResolution);

//                    CollisionResolve Result = PolygonCollider->getCollisionResolution(Polygon1->getVertices(), Polygon2->getVertices());
//                    Polygon1->move(Result.FirstResolution);
//                    Polygon2->move(Result.SecondResolution);
                }
            }
        }
    }
}
void DiscreteRotatingEulerianEngine::applyGravity() {
    Vector2 Gravity = Vector2{0, 9.8 * 10};
    for (auto& Polygon : m_RotatingPolygonList) {
        if (Polygon->isFixed()) continue;
        Polygon -> accelerate(Gravity);
    }
    for (auto& Circle : m_RotatingCircleList) {
        Circle -> accelerate(Gravity);
    }
}
void DiscreteRotatingEulerianEngine::calculateContactPoints() {
    m_ContactPointList.clear();
    for (int i = 0; i < m_RotatingPolygonList.size(); ++i) {
        SATRotatingPlatformPolygon *Polygon1 = m_RotatingPolygonList[i];
        for (int j = i + 1; j < m_RotatingPolygonList.size(); ++j) {
            SATRotatingPlatformPolygon *Polygon2 = m_RotatingPolygonList[j];
            if (Polygon1 != Polygon2) {
                SATPolygonCollider *Collider = dynamic_cast<SATPolygonCollider *>(SATPolygonCollider::getSATPolygonCollider());
                if (Collider->isColliding(Polygon1->getVertices(), Polygon2->getVertices())) {
                    vector<Vector2> VerticesWithProjection1;
                    vector<Vector2> VerticesWithProjection2;
                    vector<LineSegment> LineSegments1;
                    for (int i = 0; i < Polygon1->getVertices().size(); ++i) {
                        LineSegment Edge = {Polygon1->getVertices()[i],
                                            Polygon1->getVertices()[(i + 1) % Polygon1->getVertices().size()]};
                        LineSegments1.push_back(Edge);
                    }
                    vector<LineSegment> LineSegments2;
                    for (int i = 0; i < Polygon2->getVertices().size(); ++i) {
                        LineSegment Edge = {Polygon2->getVertices()[i],
                                            Polygon2->getVertices()[(i + 1) % Polygon2->getVertices().size()]};
                        LineSegments2.push_back(Edge);
                    }
                    for (auto &Vertex: Polygon1->getVertices()) {
                        for (auto &Edge: LineSegments2) {
                            if (Edge.haveProjection(Vertex)) {
                                VerticesWithProjection1.push_back(Vertex);
                                break;
                            }
                        }
                    }
                    for (auto &Vertex: Polygon2->getVertices()) {
                        for (auto &Edge: LineSegments1) {
                            if (Edge.haveProjection(Vertex)) {
                                VerticesWithProjection2.push_back(Vertex);
                                break;
                            }
                        }
                    }
//                    std::cout << "Number of vertices with projection 1: " << VerticesWithProjection1.size() << std::endl;
//                    for (int i = 0; i < VerticesWithProjection1.size(); ++i) {
//                        std::cout << "Vertex index: " << i << std::endl;
//                        std::cout << "Vertex: " << VerticesWithProjection1[i].x << " " << VerticesWithProjection1[i].y << std::endl;
//                    }
//                    std::cout << "Number of vertices with projection 2: " << VerticesWithProjection2.size() << std::endl;
//                    for (int i = 0; i < VerticesWithProjection2.size(); ++i) {
//                        std::cout << "Vertex index: " << i << std::endl;
//                        std::cout << "Vertex: " << VerticesWithProjection2[i].x << " " << VerticesWithProjection2[i].y << std::endl;
//                    }
                    const float InaccuracyThreshold = 0.000000001f;
                    float MinDistance1 = INT_MAX;
                    int MinimumDistanceIndex1 = 0;
                    for (int i = 0; i < VerticesWithProjection1.size(); ++i) {
                        float LocalMinDistance = INT_MAX;
                        for (auto &Edge: LineSegments2) {
                            float Distance = Line(Edge).distanceToPoint(VerticesWithProjection1[i]);
                            if (Distance < LocalMinDistance) {
                                LocalMinDistance = Distance;
                            }
                        }
                        if (LocalMinDistance < MinDistance1) {
                            MinDistance1 = LocalMinDistance;
                            MinimumDistanceIndex1 = i;
                        }
                    }
                    float MinDistance2 = INT_MAX;
                    int MinimumDistanceIndex2 = 0;
                    for (int i = 0; i < VerticesWithProjection2.size(); ++i) {
                        float LocalMinDistance = INT_MAX;
                        for (auto &Edge: LineSegments1) {
                            float Distance = Line(Edge).distanceToPoint(VerticesWithProjection2[i]);
                            if (Distance < LocalMinDistance) {
                                LocalMinDistance = Distance;
                            }
                        }
                        if (LocalMinDistance < MinDistance2) {
                            MinDistance2 = LocalMinDistance;
                            MinimumDistanceIndex2 = i;
                        }
                    }
                    float MinimumDistance;
                    if (MinDistance1 < MinDistance2) {
                        MinimumDistance = MinDistance1;
                    } else {
                        MinimumDistance = MinDistance2;
                    }
                    for (int i = 0; i < VerticesWithProjection1.size(); ++i) {
                        for (auto &Edge: LineSegments2) {
                            if (!Edge.haveProjection(VerticesWithProjection1[i])) continue;
                            float Distance = Line(Edge).distanceToPoint(VerticesWithProjection1[i]);
                            if (Distance <= MinimumDistance + InaccuracyThreshold) {
                                Vector2 Projection = Line(Edge).projection(VerticesWithProjection1[i]);
                                m_ContactPointList.push_back(Projection);
                                break;
                            }
                        }
                    }
                    for (int i = 0; i < VerticesWithProjection2.size(); ++i) {
                        for (auto &Edge: LineSegments1) {
                            if (!(Edge.haveProjection(VerticesWithProjection2[i]))) continue;
                            float Distance = Line(Edge).distanceToPoint(VerticesWithProjection2[i]);
                            if (Distance <= MinimumDistance + InaccuracyThreshold) {
                                Vector2 Projection = Line(Edge).projection(VerticesWithProjection2[i]);
                                m_ContactPointList.push_back(Projection);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
//    std::cout << "Number of contact points: " << m_ContactPointList.size() << std::endl;
//    for (auto& Point : m_ContactPointList)
//    {
//        std::cout << "Contact point: " << Point.x << " " << Point.y << std::endl;
//    }
}
void DiscreteRotatingEulerianEngine::drawContactPoints() {
    for (auto& Point : m_ContactPointList) {
        drawSquare(Point, 10, MAGENTA);
    }
}








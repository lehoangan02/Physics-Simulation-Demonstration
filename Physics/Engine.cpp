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
        ball->draw();
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


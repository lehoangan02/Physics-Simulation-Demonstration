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
#include <cmath>
#include <algorithm>
using namespace std;
void calculateFinalVelocity(const float &Mass1, const float &Mass2, Vector2 &Velocity1,
                            Vector2 &Velocity2);
void calculateFinalVelocity(const float &Mass1, const float &Mass2, float &Velocity1,
                            float &Velocity2);
class VerletEngine
{
public:
    Vector2 m_Gravity = {0, 400.0f};
    int m_Width;
    int m_Height;
    int m_Top = 0;
    int m_Left = 0;
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
    Vector2 m_Gravity = {0, 400.0f};
    int m_Width;
    int m_Height;
    int m_TotalEngery;
    Texture2D m_Background;
    vector<EulerianRoundBall*> m_RoundBallList;
public:
    ContinuousEulerianEngine(int Width, int Height);
    void attachRoundBall(EulerianRoundBall* NewRoundBall);
    void update(float DeltaTime);
    void draw();
    void reset();
private:
    void applyConstraints();
    void applyGravity();
    void collideRoundBalls(float DeltaTime);
    friend class FPSInvariantStateForContinuousIntegration;
};

class DisceteEulerianEngine
{
    class ColorSquare {
    protected:
        static Color m_RED;
        static Color m_YELLOW;
        static Color m_GREEN;
    public:
        Vector2 m_TopLeft;
        float m_Width;
        float m_Height;
        Vector2 m_Center;
    public:
        Color chooseColor(vector<EulerianRoundBall*> m_RoundBallList, EulerianRoundBall* Ball);
        bool isInsideSquare(EulerianRoundBall* Ball);
    };
protected:
    int m_Width;
    int m_Height;
    int m_TotalEngery;
    Texture2D m_Background;
    vector<EulerianRoundBall*> m_RoundBallList;
public:
    DisceteEulerianEngine(int Width, int Height);
    virtual void attachRoundBall(EulerianRoundBall* NewRoundBall);
    virtual void update(float DeltaTime);
    virtual void draw();
    void reset();
protected:
    void applyConstraints();
    void collideRoundBalls();
    void accelerateMutually();
    Color calculateColor(EulerianRoundBall* Ball);
};
class Cell
{
    friend class Grid;
private:
    Vector2 m_TopLeft;
    static float m_Width;
    static float m_Height;
    vector<EulerianRoundBall*> m_RoundBallList;
    void applyConstraintLeft();
    void applyConstraintRight();
    void applyConstraintTop();
    void applyConstraintBottom();
    void collideRoundBalls();
public:
    void attachRoundBall(EulerianRoundBall* NewRoundBall);
    explicit Cell(Vector2 TopLeft);
    void drawOutline();
    void drawRectangle();
};
class Grid
{
    friend class Cell;
private:
    static int m_Width;
    static int m_Height;
    int m_NumColumn;
    int m_NumRow;
    vector<vector<Cell*>> m_CellMatrix;
public:
    Grid(int Width, int Height, int DivideWidth, int DivideHeight);
    ~Grid();
    void attachRoundBall(EulerianRoundBall* NewRoundBall);
    vector<EulerianRoundBall*> m_RoundBallList;
    void update(float DeltaTime);
    void draw();
    void reset();
private:
    void organiseBall(EulerianRoundBall* Ball);
    Cell* findCell(Vector2 Position);
    void applyConstraints();
    void collideRoundBalls();
};
class UniformGridEngine: public DisceteEulerianEngine
{
private:
    Grid m_Grid;
public:
    UniformGridEngine(int Width, int Height, int NumRow, int NumColumn);
    void attachRoundBall(EulerianRoundBall* NewRoundBall);
    void draw();
    void update(float DeltaTime);
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP

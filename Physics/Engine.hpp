//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP
#include "RoundBall.hpp"
#include "PlatformRectangle.h"
#include "PlatformTriangle.hpp"
#include "../Math/Geometry.hpp"
#include "../Physics/Spring.hpp"
#include "Chains.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include "../Machine Learning/KMeansCalculator.h"
#include "Collider.h"
#include "SATPlatform.hpp"
#include <climits>

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

class DiscreteEulerianEngine
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
    Vector2 m_Gravity = {0, 400.0f};
    Texture2D m_Background;
    vector<EulerianRoundBall*> m_RoundBallList;
    vector<PlatformRectangle*> m_PlatformRectangleList;
    vector<Spring*> m_SpringList;
    bool m_GravityOn = true;
    bool m_ProximityColoring = true;
    bool m_MuttualyAccelerate = true;
public:
    DiscreteEulerianEngine(int Width, int Height);
    virtual void attachRoundBall(EulerianRoundBall* NewRoundBall);
    virtual void attachSpring(Spring* NewSpring);
    virtual void attachRectangle(PlatformRectangle* NewRectangle);
    virtual void update(float DeltaTime);
    virtual void draw();
    virtual void reset();
    void turnOffGravity();
    void turnOffProximityColoring();
    void turnOnGravity();
    void turnOnProximityColoring();
    void turnOffMutualAcceleration();
    void turnOnMutualAcceleration();
    void setGravity(const Vector2& Gravity);
protected:
    void applyConstraints();
    void collideRoundBalls();
    void collidePlatformRectangle();
    void accelerateMutually();
    void applyGravity();
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
private:
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
class UniformGridEngine: public DiscreteEulerianEngine
{
protected:
    Grid m_Grid;
public:
    UniformGridEngine(int Width, int Height, int NumRow, int NumColumn);
    virtual void attachRoundBall(EulerianRoundBall* NewRoundBall) override;
    virtual void draw() override;
    virtual void update(float DeltaTime) override;
    virtual void reset() override;
};
class KMeansEngine : public UniformGridEngine
{
private:
    enum VIEW_MODE
    {
        NORMAL,
        PLANETARY,
        CENTROID
    };
private:
    vector<Vector2> m_Centroids;
    vector<Color> m_CentroidColorList;
    vector<int> m_Assignment;
    KMeansCalculator* m_KMeansCalculator = nullptr;
    int m_NumCentroids = 3;
    vector<Vector2> m_Data;
    Color Color1 = RED;
    Color Color2 = BLUE;
    Color Color3 = GREEN;
    VIEW_MODE m_ViewMode = CENTROID;
    Texture2D m_XTexture = LoadTexture("Assets/Textures/X.png");
    Vector2 m_XTextureOffset = {64, 64};
public:
    KMeansEngine(int Width, int Height, int NumRow, int NumColumn);
//    void attachRoundBall(EulerianRoundBall* NewRoundBall) override;
    void update(float DeltaTime) override;
    void draw() override;
    void reset() override;
private:
    void accelerateMutally();
    void sortColors();
    void changeViewMode();
    void drawCetroids();
};
class DiscreteSATEulerianEngine
{
public:
enum CONTROL_OBJECT
{
    NONE,
    POLYGON,
    CIRCLE,
};
enum class CONTROL_TYPE
{
    INSTANT_CONTROL,
    ACCELERATE_CONTROL,
};
enum class ENGINE_MODE
{
    INSTANT_ENGINE,
    ACCELERATE_ENGINE,
};
public:
    DiscreteSATEulerianEngine(int Width, int Height);
    void attachSATPolygon(SATPlatformPolygon* NewSATPolygon);
    void attachSATCircle(SATPlatformCircle* NewSATCircle);
    void update(float DeltaTime);
    void draw();
    void reset();
    void turnOnOffCollision(bool CollisionOn);
    void turnOnOffPlayerControl(bool PlayerControlOn);
    void setObjectTypeToControl(int ObjectTypeToControl);
    void setControlType(CONTROL_TYPE ControlType);
    void setEngineMode(ENGINE_MODE EngineMode);
    void handlePlayerControl();
    void turnOnOffGravity(bool ApplyGravity) { m_ApplyGravity = ApplyGravity; }
    void turnOnOffDisplayContactPoint(bool DisplayContactPoint) { m_DisplayContactPoint = DisplayContactPoint; }
private:
    void applyConstraints();
    void checkCollision();
    void collideSATPolygons();
    void collideSATCircle();
    void collideSATPolygonsInstant();
    void collideSATCircleInstant();
    void collideSATPolygonsAccelerate();
    void collideSATCircleAccelerate();
    void applyGravity();
private:
    bool m_CollisionOn = true;
    bool m_PlayerControlOn = false;
    bool m_DisplayContactPoint = false;
    bool m_ApplyGravity = false;
    CONTROL_TYPE m_ControlType = CONTROL_TYPE::INSTANT_CONTROL;
    ENGINE_MODE m_EngineMode = ENGINE_MODE::INSTANT_ENGINE;
    int m_ObjectTypeToControl = CONTROL_OBJECT::NONE;
    int m_Width;
    int m_Height;
    Texture2D m_Background;
    vector<SATPlatformPolygon*> m_SATPolygonList;
    vector<SATPlatformCircle*> m_SATCircleList;
    vector<Vector2> m_ContactPointList;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_ENGINE_HPP
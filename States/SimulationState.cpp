//
// Created by Le Hoang An on 17/10/24.
//
#include "SimulationState.hpp"
#include "raymath.h"
#define MY_ORANGE Color(235, 131, 23, 255)
#define MY_YELLOW Color(243, 198, 35, 255)

SimulationState *StateFactory::getState(int StateNumber) {
    switch (StateNumber) {
        case StateNumber::HOME_STATE:
            return HomeState::getHomeState();
        case StateNumber::VERLET_DROP_STATE:
            return VerletDropState::getVerletDropState();
        case StateNumber::VERLET_CHAIN_STATE:
            return VerletChainState::getVerletChainState();
        case StateNumber::VERLET_CHAIN_BASKET_STATE:
            return VerletChainBasketState::getVerletChainBasketState();
        case StateNumber::EULERIAN_DROP_STATE:
            return EulerianDropState::getEulerianDropState();
        case StateNumber::TUNNELLING_COMPARISON_STATE:
            return TunnellingComparisonState::getComparisonState();
        case StateNumber::ENERGY_COMPARISON_STATE:
            return EnergyComparisonState::getEnergyComparisonState();
        case StateNumber::FPS_INVARIANT_STATE_FOR_CONTINUOUS_INTEGRATION:
            return FPSInvariantStateForContinuousIntegration::getFPSInvariantStateForContinuousIntegration();
        case StateNumber::PARTICLE_GRAVITY_STATE:
            return ParticleGravityState::getParticleGravityState();
        case StateNumber::OPTIMIZATION_1_STATE:
            return Optimization1State::getOptimization1State();
        case StateNumber::SPRING_STATE:
            return SpringState::getSpringState();
        case StateNumber::SPRING_SOFT_BODY_STATE:
            return SpringSoftBodyState::getSpringSoftBodyState();
        case StateNumber::PLAYABLE_SPRING_SOFT_BODY_STATE:
            return PlayableSpringSoftBodyState::getPlayableSpringSoftBodyState();
        case StateNumber::KMEANS_GROUPING_STATE:
            return KmeansGroupingState::getKmeansGroupingState();
        case StateNumber::KMEANS_OPTIMIZED_STATE:
            return KMeansOptimizedState::getKMeansOptimizedState();
        case StateNumber::CANVAS_STATE:
            return CanvasState::getCanvasState();
        case StateNumber::PRESSURE_SOFT_BODY_STATE:
            return PressureSoftBodyState::getPressureSoftBodyState();
        case StateNumber::SHAPE_MATCHING_SOFT_BODY_STATE:
            return ShapeMatchingSoftBodyState::getShapeMatchingSoftBodyState();
        case StateNumber::SAT_TRIANGLE_STATE:
            return SATTriangleState::getSATTriangleState();
        case StateNumber::SAT_POLYGON_STATE:
            return SATPolygonState::getSATPolygonState();
        case StateNumber::SAT_CIRCLE_POLYGON_STATE:
            return SATCirclePolygonState::getSATCirclePolygonState();
        case StateNumber::SAT_RESPONSE_STATE:
            return SATResponseState::getSATResponseState();
        case StateNumber::SAT_GRAVITY_AND_CONTACT_POINTS_STATE:
            return SATGravityAndContactPointsState::getSATGravityAndContactPointsState();
        case StateNumber::SAT_FIXED_AND_ROTATING_STATE:
            return SATRotatingState::getSATRotatingState();
        case StateNumber::MOON_LANDER_STATE:
            return MoonLanderState::getMoonLanderState();
        default:
            return nullptr;
    }
}
void SimulationState::exitState() {
    m_IsActive = false;
}
SimulationState* VerletDropState::getVerletDropState()
{

    VerletRoundBall::m_Radius = 10.0f;
    static VerletDropState MyVerletDropState;
    return &MyVerletDropState;
}
VerletDropState::VerletDropState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::VERLET_DROP_STATE;
    reset();
}
VerletDropState::~VerletDropState() {
    for (auto& platform : m_PlatformTriangleList)
    {
        delete platform;
        platform = nullptr;
    }
    for (auto& ball : m_RoundBall)
    {
        delete ball;
        ball = nullptr;
    }
}
SimulationState* VerletDropState::update() {
    if (!m_IsActive || IsKeyPressed(KEY_BACKSPACE))
    {
        return HomeState::getHomeState();
    }
    m_TotalTime += m_FrameTime;
    if (m_TotalTime > m_DropInterval && m_RoundBall.size() < m_NumBall)
    {
//        printf("Create new ball\n");
        VerletRoundBall* RoundBall2 = new VerletRoundBall(Vector2(StartX, StartY), Color(16, 55, 92, 255));
        RoundBall2 -> giveSpeed(Vector2(StartSppedX, -StartSppedY));
        m_RoundBall.push_back(RoundBall2);
        m_Engine.attachRoundBall(RoundBall2);
        m_TotalTime -= m_DropInterval;
    }
    m_Engine.update(0.05f);
    return nullptr;
}
void VerletDropState::draw() {
    ClearBackground(Color(244, 246, 255, 255));
    m_Engine.draw();
}
void VerletDropState::onNotify() {
    exitState();
}
void VerletDropState::reset() {
    m_IsActive = true;
    VerletRoundBall::m_Radius = 10.0f;
    for (auto& platform : m_PlatformTriangleList) {
        delete platform;
    }
    m_PlatformTriangleList.clear();

    for (auto& ball : m_RoundBall) {
        delete ball;
    }
    m_RoundBall.clear();
    m_TotalTime = 0.0f;
    m_Engine.reset();
    VerletRoundBall* RoundBall1 = new VerletRoundBall(Vector2(StartX, StartY), Color(16, 55, 92, 255));
    RoundBall1->giveSpeed(Vector2{StartSppedX, -StartSppedY});
    m_RoundBall.push_back(RoundBall1);
    m_Engine.attachRoundBall(RoundBall1);

    PlatformTriangle* PlatformTriangle1 = new PlatformTriangle(Vector2{300, 400}, Vector2{300, 500}, Vector2{500, 500}, Color(243, 198, 35, 255));
    PlatformTriangle* PlatformTriangle2 = new PlatformTriangle(Vector2{600, 700}, Vector2{800, 700}, Vector2{850, 500}, Color(235, 131, 23, 255));
    PlatformTriangle* PlatformTriangle3 = new PlatformTriangle(Vector2{400, 800}, Vector2{400, 900}, Vector2{600, 900}, Color(243, 198, 35, 255));
    PlatformTriangle* PlatformTriangle4 = new PlatformTriangle(Vector2{1100, 750}, Vector2{1270, 600}, Vector2{1135, 530}, Color(243, 198, 35, 255));
    PlatformTriangle* PlatformTriangle5 = new PlatformTriangle(Vector2{1360, 400}, Vector2{1610, 351}, Vector2{1431, 261}, Color(235, 131, 23, 255));
    PlatformTriangle* PlatformTriangle6 = new PlatformTriangle(Vector2{1510, 700}, Vector2{1430, 830}, Vector2{1640, 850}, MY_ORANGE);

    m_PlatformTriangleList.push_back(PlatformTriangle1);
    m_PlatformTriangleList.push_back(PlatformTriangle2);
    m_PlatformTriangleList.push_back(PlatformTriangle3);
    m_PlatformTriangleList.push_back(PlatformTriangle4);
    m_PlatformTriangleList.push_back(PlatformTriangle5);
    m_PlatformTriangleList.push_back(PlatformTriangle6);

    m_Engine.attachPlatformTriangle(PlatformTriangle1);
    m_Engine.attachPlatformTriangle(PlatformTriangle2);
    m_Engine.attachPlatformTriangle(PlatformTriangle3);
    m_Engine.attachPlatformTriangle(PlatformTriangle4);
    m_Engine.attachPlatformTriangle(PlatformTriangle5);
    m_Engine.attachPlatformTriangle(PlatformTriangle6);

    m_Engine.m_Gravity = {0, 500.0f};
//    printf("Reset2\n");
}
SimulationState* HomeState::getHomeState()
{
    static HomeState HomeState;
    return &HomeState;
}
HomeState::HomeState() {
    m_Background = LoadTexture("Assets/Textures/HomeBackground.png");
}

SimulationState* HomeState::update() {
    return nullptr;
}
void HomeState::onNotify() {
    return;
}
void HomeState::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);
//    DrawText("Press A to start the simulation", 100, 10, 20, WHITE);
//    DrawText("VERLET_STATE", 10, 80, 20, BLACK);
}
void HomeState::reset() {
    return;
}
SimulationState* VerletChainState::getVerletChainState()
{
    VerletRoundBall::m_Radius = 10.0f;
    static VerletChainState MyVerletChainState;
    return &MyVerletChainState;
}
void VerletChainState::reset() {
    m_IsActive = true;
    VerletRoundBall::m_Radius = 10.0f;
    for (auto& ball : m_RoundBall)
    {
        delete ball;
    }
    delete m_Chain;
    m_RoundBall.clear();
    m_Engine.reset();
    VerletRoundBall *RoundBall1 = new VerletRoundBall(Vector2{900, 270}, MY_ORANGE);
    RoundBall1 -> m_Fixed = true;
    VerletRoundBall *RoundBall2 = new VerletRoundBall(Vector2{915, 285}, MY_YELLOW);
    VerletRoundBall *RoundBall3 = new VerletRoundBall(Vector2{947, 300}, MY_ORANGE);
    VerletRoundBall *RoundBall4 = new VerletRoundBall(Vector2{981, 295}, MY_YELLOW);
    VerletRoundBall *RoundBall5 = new VerletRoundBall(Vector2{1005, 295}, MY_ORANGE);
    VerletRoundBall *RoundBall6 = new VerletRoundBall(Vector2{1035, 290}, MY_YELLOW);
    VerletRoundBall *RoundBall7 = new VerletRoundBall(Vector2{1055, 270}, MY_ORANGE);
    VerletRoundBall *RoundBall8 = new VerletRoundBall(Vector2{1075, 250}, MY_YELLOW);
    VerletRoundBall *RoundBall9 = new VerletRoundBall(Vector2{1095, 230}, MY_ORANGE);
    VerletRoundBall *RoundBall10 = new VerletRoundBall(Vector2{1115, 210}, MY_YELLOW);
    VerletRoundBall *RoundBall11 = new VerletRoundBall(Vector2{1135, 190}, MY_ORANGE);
    VerletRoundBall *RoundBall12 = new VerletRoundBall(Vector2{1155, 170}, MY_YELLOW);
    VerletRoundBall *RoundBall13 = new VerletRoundBall(Vector2{1175, 150}, MY_ORANGE);
    VerletRoundBall *RoundBall14 = new VerletRoundBall(Vector2{1195, 130}, MY_YELLOW);
    VerletRoundBall *RoundBall15 = new VerletRoundBall(Vector2{1215, 110}, MY_ORANGE);
    VerletRoundBall *RoundBall16 = new VerletRoundBall(Vector2{1235, 90}, MY_YELLOW);
    VerletRoundBall *RoundBall17 = new VerletRoundBall(Vector2{1255, 70}, MY_ORANGE);
    VerletRoundBall *RoundBall18 = new VerletRoundBall(Vector2{1275, 50}, MY_YELLOW);
    m_RoundBall.push_back(RoundBall1);
    m_RoundBall.push_back(RoundBall2);
    m_RoundBall.push_back(RoundBall3);
    m_RoundBall.push_back(RoundBall4);
    m_RoundBall.push_back(RoundBall5);
    m_RoundBall.push_back(RoundBall6);
    m_RoundBall.push_back(RoundBall7);
    m_RoundBall.push_back(RoundBall8);
    m_RoundBall.push_back(RoundBall9);
    m_RoundBall.push_back(RoundBall10);
    m_RoundBall.push_back(RoundBall11);
    m_RoundBall.push_back(RoundBall12);
    m_RoundBall.push_back(RoundBall13);
    m_RoundBall.push_back(RoundBall14);
    m_RoundBall.push_back(RoundBall15);
    m_RoundBall.push_back(RoundBall16);
    m_RoundBall.push_back(RoundBall17);
    m_RoundBall.push_back(RoundBall18);

    m_Chain = new VerletChain(m_RoundBall, 30.0f);

    m_Engine.attachRoundBall(RoundBall1);
    m_Engine.attachRoundBall(RoundBall2);
    m_Engine.attachRoundBall(RoundBall3);
    m_Engine.attachRoundBall(RoundBall4);
    m_Engine.attachRoundBall(RoundBall5);
    m_Engine.attachRoundBall(RoundBall6);
    m_Engine.attachRoundBall(RoundBall7);
    m_Engine.attachRoundBall(RoundBall8);
    m_Engine.attachRoundBall(RoundBall9);
    m_Engine.attachRoundBall(RoundBall10);
    m_Engine.attachRoundBall(RoundBall11);
    m_Engine.attachRoundBall(RoundBall12);
    m_Engine.attachRoundBall(RoundBall13);
    m_Engine.attachRoundBall(RoundBall14);
    m_Engine.attachRoundBall(RoundBall15);
    m_Engine.attachRoundBall(RoundBall16);
    m_Engine.attachRoundBall(RoundBall17);
    m_Engine.attachRoundBall(RoundBall18);

    m_Engine.attachChain(m_Chain);
    return;
}
void VerletChainState::onNotify() {
    exitState();
}
void VerletChainState::draw() {
    m_Engine.draw();
    return;
}
SimulationState* VerletChainState::update() {
    if (!m_IsActive)
    {
        return HomeState::getHomeState();
    }
    m_Engine.update(0.03f);

//    printf("ENGINE SUCCESSFULLY UPDATED\n");
    return nullptr;
}
VerletChainState::VerletChainState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::VERLET_CHAIN_STATE;
    reset();
}
VerletChainState::~VerletChainState() {
    for (auto& ball : m_RoundBall)
    {
        delete ball;
        ball = nullptr;
    }
    m_RoundBall.clear();
}
SimulationState* VerletChainBasketState::getVerletChainBasketState()
{
    static VerletChainBasketState MyVerletChainBasketState;
    printf("Get Verlet VerletChain Basket State\n");
    return &MyVerletChainBasketState;
}
VerletChainBasketState::VerletChainBasketState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::VERLET_CHAIN_BASKET_STATE;
    reset();
}
VerletChainBasketState::~VerletChainBasketState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBall)
    {
        delete ball;
        ball = nullptr;
    }
    for (auto& ball : m_Basket)
    {
        delete ball;
        ball = nullptr;
    }
    delete m_Chain;
    m_RoundBall.clear();
    m_Basket.clear();
    m_Engine.reset();
}
void VerletChainBasketState::reset() {
    m_IsActive = true;
    VerletRoundBall::m_Radius = 10.0f;
    for (auto& ball : m_RoundBall)
    {
        delete ball;
        ball = nullptr;
    }
    for (auto& ball : m_Basket)
    {
        delete ball;
        ball = nullptr;
    }
    delete m_Chain;
    m_RoundBall.clear();
    m_Basket.clear();
    m_Engine.reset();
    m_Engine.m_Gravity = {0, 100.0f};
    m_TotalTime = 0.0f;
    VerletRoundBall* RoundBall1 = new VerletRoundBall(Vector2{680, 590}, MY_ORANGE);
    RoundBall1 -> m_Fixed = true;
    VerletRoundBall* RoundBall2 = new VerletRoundBall(Vector2{694, 607}, MY_YELLOW);
    VerletRoundBall* RoundBall3 = new VerletRoundBall(Vector2{711, 620}, MY_ORANGE);
    VerletRoundBall* RoundBall4 = new VerletRoundBall(Vector2{734, 630}, MY_YELLOW);
    VerletRoundBall* RoundBall5 = new VerletRoundBall(Vector2{755, 639}, MY_ORANGE);
    VerletRoundBall* RoundBall6 = new VerletRoundBall(Vector2{776, 649}, MY_YELLOW);
    VerletRoundBall* RoundBall7 = new VerletRoundBall(Vector2{803, 651}, MY_ORANGE);
    VerletRoundBall* RoundBall8 = new VerletRoundBall(Vector2{826, 653}, MY_YELLOW);
    VerletRoundBall* RoundBall9 = new VerletRoundBall(Vector2{847, 658}, MY_ORANGE);
    VerletRoundBall* RoundBall10 = new VerletRoundBall(Vector2{872, 660}, MY_YELLOW);
    VerletRoundBall* RoundBall11 = new VerletRoundBall(Vector2{894, 661}, MY_ORANGE);
    VerletRoundBall* RoundBall12 = new VerletRoundBall(Vector2{920, 656}, MY_YELLOW);
    VerletRoundBall* RoundBall13 = new VerletRoundBall(Vector2{945, 664}, MY_ORANGE);
    VerletRoundBall* RoundBall14 = new VerletRoundBall(Vector2{969, 647}, MY_YELLOW);
    VerletRoundBall* RoundBall15 = new VerletRoundBall(Vector2{990, 656}, MY_ORANGE);
    VerletRoundBall* RoundBall16 = new VerletRoundBall(Vector2{1015, 651}, MY_YELLOW);
    VerletRoundBall* RoundBall17 = new VerletRoundBall(Vector2{1039, 646}, MY_ORANGE);
    VerletRoundBall* RoundBall18 = new VerletRoundBall(Vector2{1062, 638}, MY_YELLOW);
    VerletRoundBall* RoundBall19 = new VerletRoundBall(Vector2{1086, 628}, MY_ORANGE);
    VerletRoundBall* RoundBall20 = new VerletRoundBall(Vector2{1108, 619}, MY_YELLOW);
    VerletRoundBall* RoundBall21 = new VerletRoundBall(Vector2{1129, 606}, MY_ORANGE);
    VerletRoundBall* RoundBall22 = new VerletRoundBall(Vector2{1148, 592}, MY_YELLOW);
    RoundBall22 -> m_Fixed = true;

    m_Basket.push_back(RoundBall1);
    m_Basket.push_back(RoundBall2);
    m_Basket.push_back(RoundBall3);
    m_Basket.push_back(RoundBall4);
    m_Basket.push_back(RoundBall5);
    m_Basket.push_back(RoundBall6);
    m_Basket.push_back(RoundBall7);
    m_Basket.push_back(RoundBall8);
    m_Basket.push_back(RoundBall9);
    m_Basket.push_back(RoundBall10);
    m_Basket.push_back(RoundBall11);
    m_Basket.push_back(RoundBall12);
    m_Basket.push_back(RoundBall13);
    m_Basket.push_back(RoundBall14);
    m_Basket.push_back(RoundBall15);
    m_Basket.push_back(RoundBall16);
    m_Basket.push_back(RoundBall17);
    m_Basket.push_back(RoundBall18);
    m_Basket.push_back(RoundBall19);
    m_Basket.push_back(RoundBall20);
    m_Basket.push_back(RoundBall21);
    m_Basket.push_back(RoundBall22);

    m_Chain = new VerletChain(m_Basket, 10.0f);

    m_Engine.attachRoundBall(RoundBall1);
    m_Engine.attachRoundBall(RoundBall2);
    m_Engine.attachRoundBall(RoundBall3);
    m_Engine.attachRoundBall(RoundBall4);
    m_Engine.attachRoundBall(RoundBall5);
    m_Engine.attachRoundBall(RoundBall6);
    m_Engine.attachRoundBall(RoundBall7);
    m_Engine.attachRoundBall(RoundBall8);
    m_Engine.attachRoundBall(RoundBall9);
    m_Engine.attachRoundBall(RoundBall10);
    m_Engine.attachRoundBall(RoundBall11);
    m_Engine.attachRoundBall(RoundBall12);
    m_Engine.attachRoundBall(RoundBall13);
    m_Engine.attachRoundBall(RoundBall14);
    m_Engine.attachRoundBall(RoundBall15);
    m_Engine.attachRoundBall(RoundBall16);
    m_Engine.attachRoundBall(RoundBall17);
    m_Engine.attachRoundBall(RoundBall18);
    m_Engine.attachRoundBall(RoundBall19);
    m_Engine.attachRoundBall(RoundBall20);
    m_Engine.attachRoundBall(RoundBall21);
    m_Engine.attachRoundBall(RoundBall22);

    m_Engine.attachChain(m_Chain);
    return;
}
void VerletChainBasketState::onNotify() {
    exitState();
}
void VerletChainBasketState::draw() {
//    std::cout << m_RoundBall.size() << std::endl;
    m_Engine.draw();
}
SimulationState* VerletChainBasketState::update() {
    if (m_IsActive == false)
    {
        return HomeState::getHomeState();
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        VerletRoundBall* NewBall = new VerletRoundBall(GetMousePosition(), MY_ORANGE);
        m_Basket.push_back(NewBall);
    }
    m_TotalTime += m_FrameTime;
    if (m_TotalTime > m_DropInterval + 2.0f && m_RoundBall.size() < m_NumBall)
    {
        printf("Create new ball\n");
        VerletRoundBall* RoundBall2 = new VerletRoundBall(Vector2(StartX, StartY), Color(16, 55, 92, 255));
        RoundBall2 -> giveSpeed(Vector2(0, -0));
        m_RoundBall.push_back(RoundBall2);
        cout << m_RoundBall.size() << endl;
        m_Engine.attachRoundBall(RoundBall2);
        m_TotalTime -= m_DropInterval;
    }
    m_Engine.update(0.1f);
    return nullptr;
}
EulerianDropState::EulerianDropState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::EULERIAN_DROP_STATE;
    reset();
}

void EulerianDropState::reset() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
    EulerianRoundBall* RoundBall1 = new EulerianRoundBall(Vector2{500, 300}, MY_ORANGE, 1.0f);
    RoundBall1 -> m_Velocity = Vector2{500, 0};
    EulerianRoundBall* RoundBall2 = new EulerianRoundBall(Vector2{600, 600}, GREEN, 1.0f);
    RoundBall2 -> m_Mass = 2.0f;
    m_RoundBallList.push_back(RoundBall1);
    m_RoundBallList.push_back(RoundBall2);
    m_Engine.attachRoundBall(RoundBall1);
    m_Engine.attachRoundBall(RoundBall2);

    return;
}
void EulerianDropState::onNotify() {
    exitState();
}
void EulerianDropState::draw() {
    m_Engine.draw();
//    m_RoundBallList[1]->draw();
}
SimulationState* EulerianDropState::update() {
    if (!m_IsActive)
    {
        return HomeState::getHomeState();
    }
//    m_Engine.update(m_FrameTime * 10);
    m_Engine.update(m_FrameTime);
    return nullptr;
}
SimulationState* EulerianDropState::getEulerianDropState()
{
    static EulerianDropState MyEulerianDropState;
    return &MyEulerianDropState;
}
EulerianDropState::~EulerianDropState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
}

SimulationState *TunnellingComparisonState::getComparisonState() {
    VerletRoundBall::m_Radius = 100.0f;
    static TunnellingComparisonState MyComparisonState;
    return &MyComparisonState;
}
TunnellingComparisonState::TunnellingComparisonState() : m_EulerianEngine(1800 / 2 - 10, 1000), m_VerletEngine(1800 / 2 - 10, 1000) {
    m_StateNumber = StateNumber::TUNNELLING_COMPARISON_STATE;
    reset();
}
TunnellingComparisonState::~TunnellingComparisonState() {
    m_IsActive = true;
    for (auto& ball : m_EulerianRoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_EulerianRoundBallList.clear();
    for (auto& ball : m_VerletRoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_VerletRoundBallList.clear();
    m_EulerianEngine.reset();
    m_VerletEngine.reset();
}

void TunnellingComparisonState::reset() {
    m_EulerianCamera.target = Vector2{0, 0};
    m_EulerianCamera.offset = Vector2{0, 0};
    m_EulerianCamera.rotation = 0.0f;
    m_EulerianCamera.zoom = 1.0f;
    m_VerletCamera.target = Vector2{1800 / 2, 0};
    m_VerletCamera.offset = Vector2{1800 / 2 + 10, 0};
    m_VerletCamera.rotation = 0.0f;
    m_VerletCamera.zoom = 1.0f;
    m_IsActive = true;
    for (auto& ball : m_EulerianRoundBallList)
    {
        delete ball;
    }
    m_EulerianRoundBallList.clear();
    for (auto& ball : m_VerletRoundBallList)
    {
        delete ball;
    }
    m_VerletRoundBallList.clear();
    m_EulerianEngine.reset();
    m_VerletEngine.reset();
    EulerianRoundBall* EulerianRoundBall1 = new EulerianRoundBall(Vector2{100, 100}, MY_ORANGE, 1.0f);
    EulerianRoundBall1 -> m_Velocity = Vector2{200, 0};
    m_EulerianRoundBallList.push_back(EulerianRoundBall1);
    m_EulerianEngine.attachRoundBall(EulerianRoundBall1);
    VerletRoundBall::m_Radius = 100.0f;
    VerletRoundBall* VerletRoundBall1 = new VerletRoundBall(Vector2{100, 100}, MY_ORANGE);
    VerletRoundBall1 -> giveSpeed(Vector2{5, -5});
    VerletRoundBall1 -> m_SpeedScale = 1.2f;
    m_VerletRoundBallList.push_back(VerletRoundBall1);
    m_VerletEngine.attachRoundBall(VerletRoundBall1);
}

void TunnellingComparisonState::onNotify() {
    exitState();
}
SimulationState* TunnellingComparisonState::update() {
    if (!m_IsActive)
    {
        return HomeState::getHomeState();
    }
    for (auto& ball : m_EulerianRoundBallList)
    {
        ball -> m_Velocity = Vector2Scale(ball -> m_Velocity, 1.01f);
    }
    for (auto& ball : m_VerletRoundBallList)
    {
        ball -> m_Velocity = Vector2Scale(ball -> m_Velocity, 1);
    }
    m_EulerianEngine.update(m_FrameTime);
    m_VerletEngine.update(m_FrameTime);
    return nullptr;
}
void TunnellingComparisonState::draw() {
    BeginTextureMode(screenCamera1);
    ClearBackground(RAYWHITE);
    BeginMode2D(m_EulerianCamera);
    m_EulerianEngine.draw();
    DrawText("Continuous Eulerian Integration Method does not have tunnelling", 10, 50, 20, RED);
    EndMode2D();
    EndTextureMode();

    BeginTextureMode(screenCamera2);
    ClearBackground(RAYWHITE);
    BeginMode2D(m_VerletCamera);
    m_VerletEngine.draw();
    DrawText("Verlet Integration Method has tunnelling", 10, 50, 20, RED);

    EndMode2D();
    EndTextureMode();
    DrawTextureRec(screenCamera1.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
    DrawTextureRec(screenCamera2.texture, splitScreenRect, (Vector2){ 1800/2.0f, 0 }, WHITE);
    Color MidBarColor = Color(68, 3, 129, 255);
    DrawRectangle(1800 / 2 - 10, 0, 20, 1000, MidBarColor);
}

SimulationState *EnergyComparisonState::getEnergyComparisonState() {
    VerletRoundBall::m_Radius = 100.0f;
    static EnergyComparisonState MyEnergyComparisonState;
    return &MyEnergyComparisonState;
}
EnergyComparisonState::EnergyComparisonState() : m_EulerianEngine(1800 / 2 - 10, 1000), m_VerletEngine(1800 / 2 - 10, 1000) {
    m_StateNumber = StateNumber::ENERGY_COMPARISON_STATE;
    reset();
}
EnergyComparisonState::~EnergyComparisonState() {
    m_IsActive = true;
    for (auto& ball : m_EulerianRoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_EulerianRoundBallList.clear();
    for (auto& ball : m_VerletRoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_VerletRoundBallList.clear();
    m_EulerianEngine.reset();
    m_VerletEngine.reset();
}
void EnergyComparisonState::reset() {
    m_EulerianCamera.target = Vector2{0, 0};
    m_EulerianCamera.offset = Vector2{0, 0};
    m_EulerianCamera.rotation = 0.0f;
    m_EulerianCamera.zoom = 1.0f;
    m_VerletCamera.target = Vector2{1800 / 2, 0};
    m_VerletCamera.offset = Vector2{1800 / 2 + 10, 0};
    m_VerletCamera.rotation = 0.0f;
    m_VerletCamera.zoom = 1.0f;
    m_IsActive = true;
    for (auto& ball : m_EulerianRoundBallList)
    {
        delete ball;
    }
    m_EulerianRoundBallList.clear();
    for (auto& ball : m_VerletRoundBallList)
    {
        delete ball;
    }
    m_VerletRoundBallList.clear();
    m_EulerianEngine.reset();
    m_VerletEngine.reset();
    EulerianRoundBall* EulerianRoundBall1 = new EulerianRoundBall(Vector2{100, 100}, MY_ORANGE, 1.0f);
    EulerianRoundBall1 -> m_Velocity = Vector2{300, 0};
    EulerianRoundBall* EulerianRoundBall2 = new EulerianRoundBall(Vector2{400, 200}, GREEN, 1.0f);
    m_EulerianRoundBallList.push_back(EulerianRoundBall1);
    m_EulerianRoundBallList.push_back(EulerianRoundBall2);
    m_EulerianEngine.attachRoundBall(EulerianRoundBall1);
    m_EulerianEngine.attachRoundBall(EulerianRoundBall2);
    VerletRoundBall::m_Radius = 100.0f;
    VerletRoundBall* VerletRoundBall1 = new VerletRoundBall(Vector2{100, 100}, MY_ORANGE);
    VerletRoundBall1 -> giveSpeed(Vector2{5, -5});
    VerletRoundBall* VerletRoundBall2 = new VerletRoundBall(Vector2{400, 400}, GREEN);
    VerletRoundBall2 -> giveSpeed(Vector2{5, 10});
    m_VerletRoundBallList.push_back(VerletRoundBall1);
    m_VerletRoundBallList.push_back(VerletRoundBall2);
    m_VerletEngine.attachRoundBall(VerletRoundBall1);
    m_VerletEngine.attachRoundBall(VerletRoundBall2);
}
void EnergyComparisonState::onNotify() {
    exitState();
}
SimulationState* EnergyComparisonState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_EulerianEngine.update(m_FrameTime);
    m_VerletEngine.update(m_FrameTime);
    return nullptr;
}
void EnergyComparisonState::draw() {
    BeginTextureMode(screenCamera1);
    ClearBackground(RAYWHITE);
    BeginMode2D(m_EulerianCamera);
    m_EulerianEngine.draw();
    DrawText("Continuous Eulerian Integration Method conserves energy", 10, 50, 20, RED);
    EndMode2D();
    EndTextureMode();

    BeginTextureMode(screenCamera2);
    ClearBackground(RAYWHITE);
    BeginMode2D(m_VerletCamera);
    m_VerletEngine.draw();
    DrawText("Verlet Integration Method does not conserve energy", 10, 50, 20, RED);

    EndMode2D();
    EndTextureMode();
    DrawTextureRec(screenCamera1.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
    DrawTextureRec(screenCamera2.texture, splitScreenRect, (Vector2){ 1800/2.0f, 0 }, WHITE);
    Color MidBarColor = Color(68, 3, 129, 255);
    DrawRectangle(1800 / 2 - 10, 0, 20, 1000, MidBarColor);
}
FPSInvariantStateForContinuousIntegration::FPSInvariantStateForContinuousIntegration() : m_Engine1(1800 / 2, 1000), m_Engine2(1800 / 2, 1000) {
    m_StateNumber = StateNumber::FPS_INVARIANT_STATE_FOR_CONTINUOUS_INTEGRATION;
    reset();
}
FPSInvariantStateForContinuousIntegration::~FPSInvariantStateForContinuousIntegration() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList1)
    {
        delete ball;
        ball = nullptr;
    }
    m_RoundBallList1.clear();
    for (auto& ball : m_RoundBallList2)
    {
        delete ball;
        ball = nullptr;
    }
    m_RoundBallList2.clear();
    m_Engine1.reset();
    m_Engine2.reset();
}
SimulationState *FPSInvariantStateForContinuousIntegration::getFPSInvariantStateForContinuousIntegration() {
    static FPSInvariantStateForContinuousIntegration MyFPSInvariantStateForContinuousIntegration;
    return &MyFPSInvariantStateForContinuousIntegration;
}
void FPSInvariantStateForContinuousIntegration::reset() {
    m_Camera1.target = Vector2{0, 0};
    m_Camera1.offset = Vector2{0, 0};
    m_Camera1.rotation = 0.0f;
    m_Camera1.zoom = 1.0f;
    m_Camera2.target = Vector2{1800 / 2, 0};
    m_Camera2.offset = Vector2{1800 / 2 + 10, 0};
    m_Camera2.rotation = 0.0f;
    m_Camera2.zoom = 1.0f;
    m_IsActive = true;
    for (auto& ball : m_RoundBallList1)
    {
        delete ball;
    }
    m_RoundBallList1.clear();
    for (auto& ball : m_RoundBallList2)
    {
        delete ball;
    }
    m_RoundBallList2.clear();
    m_Engine1.reset();
    m_Engine2.reset();
    m_Engine1.m_Gravity = {0, 0.0f};
    m_Engine2.m_Gravity = {0, 0.0f};
    EulerianRoundBall* EulerianRoundBall1 = new EulerianRoundBall(Vector2{100, 100}, MY_ORANGE, 1.0f);
    EulerianRoundBall1 -> m_Velocity = Vector2{90, 500};
    EulerianRoundBall* EulerianRoundBall2 = new EulerianRoundBall(Vector2{300, 300}, GREEN, 1.0f);
    EulerianRoundBall2 -> m_Velocity = Vector2{300, 70};
    m_RoundBallList1.push_back(EulerianRoundBall1);
    m_RoundBallList1.push_back(EulerianRoundBall2);
    m_Engine1.attachRoundBall(EulerianRoundBall1);
    m_Engine1.attachRoundBall(EulerianRoundBall2);
    EulerianRoundBall* EulerianRoundBall3 = new EulerianRoundBall(Vector2{100, 100}, MY_ORANGE, 1.0f);
    EulerianRoundBall3 -> m_Velocity = Vector2{90, 500};
    EulerianRoundBall* EulerianRoundBall4 = new EulerianRoundBall(Vector2{300, 300}, GREEN, 1.0f);
    EulerianRoundBall4 -> m_Velocity = Vector2{300, 70};
    m_RoundBallList2.push_back(EulerianRoundBall3);
    m_RoundBallList2.push_back(EulerianRoundBall4);
    m_Engine2.attachRoundBall(EulerianRoundBall3);
    m_Engine2.attachRoundBall(EulerianRoundBall4);
}
void FPSInvariantStateForContinuousIntegration::onNotify() {
    exitState();
}
SimulationState* FPSInvariantStateForContinuousIntegration::update() {
//    SetTargetFPS(24);
    static int FrameSkipped = 0;
    static float SkippingFrameTime = 0.0f;
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_Engine1.update(m_FrameTime);
    SkippingFrameTime += GetFrameTime();
    if (FrameSkipped == 2)
    {
        m_FrameCount = 1 / SkippingFrameTime;
        m_Engine2.update(m_FrameTime * 3);
        SkippingFrameTime = 0;
        FrameSkipped = 0;
    }
    else {
        ++FrameSkipped;
    }
    return nullptr;
}
void FPSInvariantStateForContinuousIntegration::draw() {
    BeginTextureMode(screenCamera1);
    ClearBackground(RAYWHITE);
    BeginMode2D(m_Camera1);
    m_Engine1.draw();
    DrawText("Continuous Eulerian Integration Method is FPS-invariant", 10, 50, 20, RED);
    DrawText("Inaccuracy is due to the floating point precision", 10, 70, 20, RED);
    EndMode2D();
    EndTextureMode();

    BeginTextureMode(screenCamera2);
    ClearBackground(RAYWHITE);
    BeginMode2D(m_Camera2);
    m_Engine2.draw();
    std::string SkippingFPS = "FPS: " + std::to_string(m_FrameCount);
    DrawText(SkippingFPS.c_str(), 10, 50, 20, RED);

    EndMode2D();
    EndTextureMode();
    DrawTextureRec(screenCamera1.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
    DrawTextureRec(screenCamera2.texture, splitScreenRect, (Vector2){ 1800/2.0f, 0 }, WHITE);
    Color MidBarColor = Color(68, 3, 129, 255);
    DrawRectangle(1800 / 2 - 10, 0, 20, 1000, MidBarColor);
}
ParticleGravityState::ParticleGravityState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::PARTICLE_GRAVITY_STATE;
    reset();
}
ParticleGravityState::~ParticleGravityState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
}
void ParticleGravityState::reset() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
    m_Engine.turnOffGravity();
    for (int i = 0; i < 15; ++i)
    {
        float StartX = 100 + 100 * i;
        for (int j = 0; j < 9; ++j)
        {
            EulerianRoundBall* RoundBall = new EulerianRoundBall(Vector2{StartX, float(100 + 80 * j)}, MY_ORANGE, 1.0f);
            RoundBall -> m_Velocity = Vector2{00, 0};
            RoundBall -> m_Radius = 10.0f;
            m_RoundBallList.push_back(RoundBall);
            m_Engine.attachRoundBall(RoundBall);
        }
    }
}
void ParticleGravityState::onNotify() {
    exitState();
}
SimulationState* ParticleGravityState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    for (int i = 0; i < 6; ++i)
    {
        m_Engine.update(m_FrameTime);
    }
    return nullptr;
}
void ParticleGravityState::draw() {
    m_Engine.draw();
}
SimulationState* ParticleGravityState::getParticleGravityState() {
    static ParticleGravityState MyParticleGravityState;
    return &MyParticleGravityState;
}

SimulationState *Optimization1State::getOptimization1State() {
    static Optimization1State MyOptimization1State;
    return &MyOptimization1State;
}
Optimization1State::Optimization1State() : m_Engine(1800, 1000, 75, 50) {
    m_StateNumber = StateNumber::OPTIMIZATION_1_STATE;
    reset();
}
Optimization1State::~Optimization1State() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
}
void Optimization1State::reset() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
    for (int i = 0; i < 30; ++i)
    {
        float StartX = 100 + 50 * i;
        for (int j = 0; j < 20; ++j)
        {
            EulerianRoundBall* RoundBall = new EulerianRoundBall(Vector2{StartX, float(100 + 50 * j)}, MY_ORANGE, 1.0f);
            float VelX = static_cast<int>(std::rand() % 50);
            float VelY = static_cast<int>(std::rand() % 50);
            RoundBall -> m_Velocity = Vector2{VelX, VelY};
            RoundBall -> m_Radius = 5.0f;
            RoundBall -> m_Color = Color(176, 48, 82, 255);
            m_RoundBallList.push_back(RoundBall);
            m_Engine.attachRoundBall(RoundBall);
        }
    }
}
void Optimization1State::onNotify() {
    exitState();
}
SimulationState* Optimization1State::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    for (int i = 0; i < 6; ++i)
    {
        m_Engine.update(m_FrameTime);
    }
    return nullptr;
}
void Optimization1State::draw() {
    m_Engine.draw();
}

SimulationState *SpringState::getSpringState() {
    static SpringState MySpringState;
    return &MySpringState;
}
SpringState::SpringState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SPRING_STATE;
    reset();
}
SpringState::~SpringState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    for (auto& spring : m_SpringList)
    {
        delete spring;
        spring = nullptr;
    }
    for (auto& rectangle : m_PlatformRectangleList)
    {
        delete rectangle;
        rectangle = nullptr;
    }
    m_RoundBallList.clear();
    m_SpringList.clear();
    m_PlatformRectangleList.clear();
    m_Engine.reset();
}
void SpringState::onNotify() {
    exitState();
}
void SpringState::reset() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    for (auto& spring : m_SpringList)
    {
        delete spring;
        spring = nullptr;
    }
    for (auto& rectangle : m_PlatformRectangleList)
    {
        delete rectangle;
        rectangle = nullptr;
    }
    m_RoundBallList.clear();
    m_SpringList.clear();
    m_PlatformRectangleList.clear();
    m_Engine.reset();
    m_Engine.turnOffProximityColoring();
    m_Engine.turnOffMutualAcceleration();
    Vector2 StartingPosition = Vector2{400, 200};
    Vector2 PositionDifference = Vector2{50, 30};
    Color BallColor = Color(33, 155, 157, 255);
    EulerianRoundBall* RoundBall1 = new EulerianRoundBall(StartingPosition, BallColor, 1.0f);
    RoundBall1 -> m_Radius = 10.0f;
    StartingPosition = Vector2Add(StartingPosition, PositionDifference);
    EulerianRoundBall *RoundBall2 = new EulerianRoundBall(StartingPosition, BallColor, 1.0f);
    RoundBall2 -> m_Radius = 10.0f;
    StartingPosition = Vector2Add(StartingPosition, PositionDifference);
    EulerianRoundBall *RoundBall3 = new EulerianRoundBall(StartingPosition, BallColor, 1.0f);
    RoundBall3 -> m_Radius = 10.0f;
    StartingPosition = Vector2Add(StartingPosition, PositionDifference);
    EulerianRoundBall *RoundBall4 = new EulerianRoundBall(StartingPosition, BallColor, 1.0f);
    RoundBall4 -> m_Radius = 10.0f;
    float SpringStrength = 20.0f;

    Color SpringColor = Color(76, 31, 122, 255);
    Spring *Spring1 = new Spring(RoundBall1, RoundBall2, 20.0f, SpringStrength, SpringColor);
    Spring *Spring2 = new Spring(RoundBall2, RoundBall3, 20.0f, SpringStrength, SpringColor);
    Spring *Spring3 = new Spring(RoundBall3, RoundBall4, 20.0f, SpringStrength, SpringColor);
    EulerianRoundBall *RoundBall5 = new EulerianRoundBall(Vector2{500, 200}, BallColor, 1.0f);
    RoundBall5 -> m_Radius = 10.0f;
    RoundBall5 -> m_Velocity = Vector2{250, 150};
    EulerianRoundBall *RoundBall6 = new EulerianRoundBall(Vector2{600, 200}, BallColor, 1.0f);
    RoundBall6 -> m_Radius = 10.0f;
    RoundBall6 -> m_Velocity = Vector2{150, -50};
    EulerianRoundBall *RoundBall7 = new EulerianRoundBall(Vector2{700, 200}, BallColor, 1.0f);
    RoundBall7 -> m_Radius = 10.0f;
    RoundBall7 -> m_Velocity = Vector2{50, 350};


    m_RoundBallList.push_back(RoundBall1);
    m_RoundBallList.push_back(RoundBall2);
    m_RoundBallList.push_back(RoundBall3);
    m_RoundBallList.push_back(RoundBall4);
    m_RoundBallList.push_back(RoundBall5);
    m_RoundBallList.push_back(RoundBall6);
    m_RoundBallList.push_back(RoundBall7);
    m_SpringList.push_back(Spring1);
    m_Engine.attachRoundBall(RoundBall1);
    m_Engine.attachRoundBall(RoundBall2);
    m_Engine.attachRoundBall(RoundBall3);
    m_Engine.attachRoundBall(RoundBall4);
    m_Engine.attachRoundBall(RoundBall5);
    m_Engine.attachRoundBall(RoundBall6);
    m_Engine.attachRoundBall(RoundBall7);

    m_Engine.attachSpring(Spring1);
    m_Engine.attachSpring(Spring2);
    m_Engine.attachSpring(Spring3);

    Color RectangleColor = Color(182, 162, 142, 255);
    PlatformRectangle *Rectangle1 = new PlatformRectangle(Vector2{230, 400}, 380, 40, 50, RectangleColor);
    PlatformRectangle *Rectangle2 = new PlatformRectangle(Vector2{1350, 430}, 380, 40, 130, RectangleColor);
    PlatformRectangle *Rectangle3 = new PlatformRectangle(Vector2{500, 100}, 380, 40, 0, RectangleColor);
    PlatformRectangle *Rectangle4 = new PlatformRectangle(Vector2{400, 650}, 800, 40, 0, RectangleColor);
    m_PlatformRectangleList.push_back(Rectangle1);
    m_PlatformRectangleList.push_back(Rectangle2);
    m_PlatformRectangleList.push_back(Rectangle3);
    m_PlatformRectangleList.push_back(Rectangle4);
    m_Engine.attachRectangle(Rectangle1);
    m_Engine.attachRectangle(Rectangle2);
    m_Engine.attachRectangle(Rectangle3);
    m_Engine.attachRectangle(Rectangle4);
}
SimulationState* SpringState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_Engine.update(m_FrameTime);
    return nullptr;
}
void SpringState::draw() {
    m_Engine.draw();
}

SimulationState *SpringSoftBodyState::getSpringSoftBodyState() {
    static SpringSoftBodyState MySpringSoftBodyState;
    return &MySpringSoftBodyState;
}
SpringSoftBodyState::SpringSoftBodyState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SPRING_SOFT_BODY_STATE;
    reset();
}
SpringSoftBodyState::~SpringSoftBodyState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallMatrix)
    {
        for (auto& roundball : ball)
        {
            delete roundball;
            roundball = nullptr;
        }
    }
    for (auto& spring : m_SpringList)
    {
        delete spring;
        spring = nullptr;
    }
    for (auto& rectangle : m_PlatformRectangleList)
    {
        delete rectangle;
        rectangle = nullptr;
    }
    m_RoundBallMatrix.clear();
    m_SpringList.clear();
    m_PlatformRectangleList.clear();
    m_Engine.reset();
    m_Engine.turnOffProximityColoring();
    m_Engine.turnOffMutualAcceleration();
}
void SpringSoftBodyState::reset() {
    m_IsActive = true;
    m_TotalTime = 0.0f;
    for (auto& ball : m_RoundBallMatrix)
    {
        for (auto& roundball : ball)
        {
            delete roundball;
            roundball = nullptr;
        }
    }
    for (auto& spring : m_SpringList)
    {
        delete spring;
        spring = nullptr;
    }
    for (auto& rectangle : m_PlatformRectangleList)
    {
        delete rectangle;
        rectangle = nullptr;
    }
    m_RoundBallMatrix.clear();
    m_SpringList.clear();
    m_PlatformRectangleList.clear();
    m_Engine.reset();
    m_Engine.turnOffProximityColoring();
    m_Engine.turnOffMutualAcceleration();


    Vector2 StartingPosition = Vector2{300, 200};
    Vector2 PositionDifference = Vector2{40, 40};
    float AllignedSpringStrength = 3000.0f;

    float AllignedSpringLength = PositionDifference.x;
    float SpringDiagonalStrength = 20000.0f;

    float SpringDiagonalLength = PositionDifference.x * sqrt(2);
    int NumBalls = 10;
    float BallRadius = 13.0f;

    for (int i = 0; i < NumBalls; ++i)
    {
        vector<ClickableEulerianRoundBall*> Row;
        for (int j = 0; j < NumBalls; ++j)
        {
            ClickableEulerianRoundBall *Roundball = new ClickableEulerianRoundBall(Vector2{StartingPosition.x + i * PositionDifference.x, StartingPosition.y + j * PositionDifference.y}, m_BallColor, 1.0f);
            Roundball -> m_Radius = BallRadius;
            Row.push_back(Roundball);
            m_Engine.attachRoundBall(Roundball);
        }
        m_RoundBallMatrix.push_back(Row);
        for (int j = 0; j < NumBalls - 1; ++j)
        {
            Spring *Spring1 = new Spring(m_RoundBallMatrix[i][j], m_RoundBallMatrix[i][j + 1], AllignedSpringLength, AllignedSpringStrength, m_SpringColor);
            m_SpringList.push_back(Spring1);
            m_Engine.attachSpring(Spring1);
        }
        if (i > 0)
        {
            for (int j = 0; j < NumBalls; ++j)
            {
                Spring *Spring1 = new Spring(m_RoundBallMatrix[i][j], m_RoundBallMatrix[i - 1][j], PositionDifference.y, AllignedSpringStrength, m_SpringColor);
                m_SpringList.push_back(Spring1);
                m_Engine.attachSpring(Spring1);

            }
            for (int j = 1; j < NumBalls; ++j)
            {
                Spring *Spring1 = new Spring(m_RoundBallMatrix[i][j], m_RoundBallMatrix[i - 1][j - 1], SpringDiagonalLength, SpringDiagonalStrength, m_SpringColor);
                m_SpringList.push_back(Spring1);
                m_Engine.attachSpring(Spring1);
            }
            for (int j = 0; j < NumBalls - 1; ++j)
            {
                Spring *Spring1 = new Spring(m_RoundBallMatrix[i][j], m_RoundBallMatrix[i - 1][j + 1], SpringDiagonalLength, SpringDiagonalStrength, m_SpringColor);
                m_SpringList.push_back(Spring1);
                m_Engine.attachSpring(Spring1);
            }
        }
    }
    for (auto& spring : m_SpringList)
    {
        spring ->setDamping(true);


    }
    PlatformRectangle *Rectangle1 = new PlatformRectangle(Vector2{230, 500}, 380, 100, 40, Color(182, 162, 142, 255));
    PlatformRectangle *Rectangle2 = new PlatformRectangle(Vector2{2050, 1000}, 380, 100, 130, Color(182, 162, 142, 255));
//    PlatformRectangle *Rectangle2 = new PlatformRectangle(Vector2{1750, 700}, 380, 100, 130, Color(182, 162, 142, 255));

    m_PlatformRectangleList.push_back(Rectangle1);
    m_PlatformRectangleList.push_back(Rectangle2);
    m_Engine.attachRectangle(Rectangle1);
    m_Engine.attachRectangle(Rectangle2);
}
void SpringSoftBodyState::onNotify() {
    exitState();
}
SimulationState* SpringSoftBodyState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_TotalTime += GetFrameTime();
    float Speed = 200.0f;
    float PushAmount = Speed * GetFrameTime();
    if (m_TotalTime > m_PushRectangleTime && m_TotalTime < m_PushRectangleTime + 1.0f)
    {
//        std::cout << "Pushing rectangle\n";
        m_PlatformRectangleList[1]->move(Vector2{-PushAmount, -PushAmount});
    }
    int SubStep = 10;
    for (int i = 0; i < SubStep; ++i) {
        m_Engine.update(m_FrameTime / SubStep);
    }
    return nullptr;
}
void SpringSoftBodyState::draw() {
    m_Engine.draw();
}

SimulationState *PlayableSpringSoftBodyState::getPlayableSpringSoftBodyState() {
    static PlayableSpringSoftBodyState MyPlayableSpringSoftBodyState;
    return &MyPlayableSpringSoftBodyState;
}
PlayableSpringSoftBodyState::PlayableSpringSoftBodyState() {
    m_StateNumber = StateNumber::PLAYABLE_SPRING_SOFT_BODY_STATE;
    reset();
}
PlayableSpringSoftBodyState::~PlayableSpringSoftBodyState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallMatrix)
    {
        for (auto& roundball : ball)
        {
            delete roundball;
            roundball = nullptr;
        }
    }
    for (auto& spring : m_SpringList)
    {
        delete spring;
        spring = nullptr;
    }
    for (auto& rectangle : m_PlatformRectangleList)
    {
        delete rectangle;
        rectangle = nullptr;
    }
    m_RoundBallMatrix.clear();
    m_SpringList.clear();
    m_PlatformRectangleList.clear();
    m_Engine.reset();
    m_Engine.turnOffProximityColoring();
    m_Engine.turnOffMutualAcceleration();
}
void PlayableSpringSoftBodyState::reset() {
    m_IsActive = true;
    m_TotalTime = 0.0f;
    m_BallColor = Color(96, 76, 195, 255);
    m_SpringColor = Color(143, 209, 79, 200);
    for (auto &ball: m_RoundBallMatrix) {
        for (auto &roundball: ball) {
            delete roundball;
            roundball = nullptr;
        }
    }
    for (auto &spring: m_SpringList) {
        delete spring;
        spring = nullptr;
    }
    for (auto &rectangle: m_PlatformRectangleList) {
        delete rectangle;
        rectangle = nullptr;
    }
    m_RoundBallMatrix.clear();
    m_SpringList.clear();
    m_PlatformRectangleList.clear();
    m_Engine.reset();
    m_Engine.turnOffProximityColoring();
    m_Engine.turnOffMutualAcceleration();


    Vector2 StartingPosition = Vector2{300, 200};
    Vector2 PositionDifference = Vector2{40, 40};
    float AllignedSpringStrength = 5000.0f;
    float AllignedSpringLength = PositionDifference.x;
    float SpringDiagonalStrength = 5000.0f;
    float SpringDiagonalLength = PositionDifference.x * sqrt(2);
    int NumBalls = 10;
    float BallRadius = 17.0f;

    for (int i = 0; i < NumBalls; ++i) {
        vector<ClickableEulerianRoundBall *> Row;
        for (int j = 0; j < NumBalls; ++j) {
            ClickableEulerianRoundBall *Roundball = new ClickableEulerianRoundBall(Vector2{StartingPosition.x + i * PositionDifference.x,
                                                                         StartingPosition.y + j * PositionDifference.y},
                                                                                   m_BallColor, 1.0f);
            Roundball->m_Radius = BallRadius;
            Row.push_back(Roundball);
            m_Engine.attachRoundBall(Roundball);
        }
        m_RoundBallMatrix.push_back(Row);
        for (int j = 0; j < NumBalls - 1; ++j) {
            Spring *Spring1 = new Spring(m_RoundBallMatrix[i][j], m_RoundBallMatrix[i][j + 1], AllignedSpringLength,
                                         AllignedSpringStrength, m_SpringColor);
            m_SpringList.push_back(Spring1);
            m_Engine.attachSpring(Spring1);
        }
        if (i > 0) {
            for (int j = 0; j < NumBalls; ++j) {
                Spring *Spring1 = new Spring(m_RoundBallMatrix[i][j], m_RoundBallMatrix[i - 1][j], PositionDifference.y,
                                             AllignedSpringStrength, m_SpringColor);
                m_SpringList.push_back(Spring1);
                m_Engine.attachSpring(Spring1);

            }
            for (int j = 1; j < NumBalls; ++j) {
                Spring *Spring1 = new Spring(m_RoundBallMatrix[i][j], m_RoundBallMatrix[i - 1][j - 1],
                                             SpringDiagonalLength, SpringDiagonalStrength, m_SpringColor);
                m_SpringList.push_back(Spring1);
                m_Engine.attachSpring(Spring1);
            }
            for (int j = 0; j < NumBalls - 1; ++j) {
                Spring *Spring1 = new Spring(m_RoundBallMatrix[i][j], m_RoundBallMatrix[i - 1][j + 1],
                                             SpringDiagonalLength, SpringDiagonalStrength, m_SpringColor);
                m_SpringList.push_back(Spring1);
                m_Engine.attachSpring(Spring1);
            }
        }
    }
    for (auto &spring: m_SpringList) {
        spring->m_Damping = true;
        spring->m_DampingFactor = 1.5f;
    }
}
void PlayableSpringSoftBodyState::onNotify() {
    exitState();
}
SimulationState *PlayableSpringSoftBodyState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 MousePosition = GetMousePosition();
        for (auto& row : m_RoundBallMatrix)
        {
            for (auto& ball : row)
            {
                if (ball ->isInside(MousePosition))
                {
                    if (m_SelectedBall)
                    {
                        m_SelectedBall -> m_Color = m_BallColor;
                    }
                    ball -> m_Color = m_SelectedColor;
                    m_SelectedBall = ball;
                }
            }
        }
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && m_SelectedBall)
    {
        Vector2 MousePosition = GetMousePosition();
        Vector2 Direction = Vector2Subtract(MousePosition, m_SelectedBall -> m_CurrentPosition);
        Direction = Vector2Normalize(Direction);
        Vector2 Force = Vector2Scale(Direction, m_Force);
        m_SelectedBall ->applyForce(Force);
//        std::cout << "Applying force\n";
    }
    int SubStep = 10;
    for (int i = 0; i < SubStep; ++i) {
        m_Engine.update(m_FrameTime / SubStep);
    }
    return nullptr;
}
void PlayableSpringSoftBodyState::draw() {
    m_Engine.draw();
    DrawText("Click on a ball to select it", 100, 10, 20, RED);
    DrawText("Hold left mouse button to apply force", 100, 30, 20, RED);
    DrawText("Enjoy the soft body simulation", 100, 50, 20, RED);
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && m_SelectedBall)
    {
        Color LineColor = Color(249, 84, 84, 255);
        DrawLineAsRectangle(m_SelectedBall -> m_CurrentPosition, GetMousePosition(), 5, LineColor);
        DrawCircle(GetMousePosition().x, GetMousePosition().y, 7, LineColor);
    }
}
KmeansGroupingState::KmeansGroupingState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::KMEANS_GROUPING_STATE;
    reset();
}
KmeansGroupingState::~KmeansGroupingState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
}
void KmeansGroupingState::reset() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
    m_Engine.turnOffGravity();
    m_Engine.turnOffProximityColoring();
    for (int i = 0; i < 15; ++i)
    {
        float StartX = 100 + 100 * i;
        for (int j = 0; j < 9; ++j)
        {
            EulerianRoundBall* RoundBall = new EulerianRoundBall(Vector2{StartX, float(100 + 80 * j)}, MY_ORANGE, 1.0f);
            RoundBall -> m_Velocity = Vector2{00, 0};
            RoundBall -> m_Radius = 10.0f;
            RoundBall -> m_Color = BLACK;
            m_RoundBallList.push_back(RoundBall);
            m_Engine.attachRoundBall(RoundBall);
        }
    }
}
void KmeansGroupingState::onNotify() {
    exitState();
}
SimulationState* KmeansGroupingState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    static int FrameSkipped = 0;
    static std::vector<Vector2> Data;
    for (int i = 0; i < 6; ++i)
    {
        m_Engine.update(m_FrameTime);
    }

    ++FrameSkipped;
    if (FrameSkipped == 10)
    {
        FrameSkipped = 0;
        Data.clear();
        for (auto& ball : m_RoundBallList)
        {
            Data.push_back(ball -> m_CurrentPosition);

        }
        KMeansCalculator* MyKMeansCalculator = KMeansCalculator::getKMeansCalculator();
        m_Centroids = (*MyKMeansCalculator)(Data, 2);
    }
    return nullptr;
}
void KmeansGroupingState::draw() {
    m_Engine.draw();
    for (auto& centroid : m_Centroids)
    {
        DrawCircle(centroid.x, centroid.y, 200, ColorAlpha(RED, 0.1));
        DrawRing(centroid, 200, 210, 0, 360, 100, ColorAlpha(RED, 0.5));
    }
}
SimulationState *KmeansGroupingState::getKmeansGroupingState() {
    static KmeansGroupingState MyKmeansGroupingState;
    return &MyKmeansGroupingState;
}
SimulationState *KMeansOptimizedState::getKMeansOptimizedState() {
    static KMeansOptimizedState MyKMeansOptimizedState;
    return &MyKMeansOptimizedState;
}
KMeansOptimizedState::KMeansOptimizedState() : m_Engine(1800, 1000, 75, 50) {
    m_StateNumber = StateNumber::KMEANS_OPTIMIZED_STATE;
    reset();
}
KMeansOptimizedState::~KMeansOptimizedState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
}
void KMeansOptimizedState::reset() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
    }
    m_RoundBallList.clear();
    m_Engine.reset();
    m_Engine.turnOffGravity();
    m_Engine.turnOffProximityColoring();
    Vector2 StartingPosition = Vector2{100, 100};
    Vector2 PositionDifference = Vector2{50, 50};
    for (int i = 0; i < 25; ++i)
    {
        float StartX = StartingPosition.x + PositionDifference.x * i;
        for (int j = 0; j < 20; ++j)
        {
            EulerianRoundBall* RoundBall = new VelocityVisualizer(Vector2{StartX, StartingPosition.y + PositionDifference.y * j}, MY_ORANGE, 1.0f);
            RoundBall -> m_Velocity = Vector2{00, 0};
            RoundBall -> m_Radius = 7.0f;
            RoundBall -> m_Color = BLACK;
            m_RoundBallList.push_back(RoundBall);
            m_Engine.attachRoundBall(RoundBall);
        }
    }
}
void KMeansOptimizedState::onNotify() {
    exitState();
}
void KMeansOptimizedState::draw() {
    m_Engine.draw();
    DrawText("KMeans Optimized State", 100, 10, 20, RED);
    DrawText("Press '1', '2', '3' to change viewing mode", 100, 30, 20, RED);
}
SimulationState *KMeansOptimizedState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_Engine.update(m_FrameTime);
    return nullptr;
}
SimulationState* CanvasState::getCanvasState() {
    static CanvasState MyCanvasState;
    return &MyCanvasState;
}
CanvasState::CanvasState() {
    m_StateNumber = StateNumber::CANVAS_STATE;
    reset();
}
CanvasState::~CanvasState() {
    m_IsActive = true;
}
void CanvasState::reset() {
    m_IsActive = true;
    m_Points.clear();
}
void CanvasState::onNotify() {
    exitState();
}
SimulationState* CanvasState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        m_Points.push_back(GetMousePosition());
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        for (auto &point: m_Points) {
            std::cout << point.x << " " << point.y << std::endl;
        }
    }
    return nullptr;
}
void CanvasState::draw() {
    for (auto &point: m_Points) {
        DrawCircleV(point, 5, RED);
    }
}
SimulationState* PressureSoftBodyState::getPressureSoftBodyState() {
    static PressureSoftBodyState MyPressureSoftBodyState;
    return &MyPressureSoftBodyState;
}
PressureSoftBodyState::PressureSoftBodyState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::PRESSURE_SOFT_BODY_STATE;
    reset();
}
PressureSoftBodyState::~PressureSoftBodyState() {
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    for (auto& spring : m_SpringList)
    {
        delete spring;
        spring = nullptr;
    }
    m_RoundBallList.clear();
    m_SpringList.clear();
    m_Engine.reset();
}
void PressureSoftBodyState::reset()
{
    m_SmileySize = {float(m_Smiley.width * 0.03), float(m_Smiley.height * 0.03)};
    cout << "Smiley size: " << m_SmileySize.x << " " << m_SmileySize.y << endl;
    m_IsActive = true;
    for (auto& ball : m_RoundBallList)
    {
        delete ball;
        ball = nullptr;
    }
    for (auto& spring : m_SpringList)
    {
        delete spring;
        spring = nullptr;
    }
    m_RoundBallList.clear();
    m_SpringList.clear();
    m_Engine.reset();
    m_Engine.turnOffProximityColoring();
    m_Engine.turnOffMutualAcceleration();

    Color SpringColor = Color(252, 199, 55, 255);
    Color BallColor = Color(126, 24, 145, 255);
    Vector2 Offset = {0, 300};
    ifstream fin("PressureSoftBodyCoordinate.txt");
    int NumberOfBall;
    fin >> NumberOfBall;
    for (int i = 0; i < NumberOfBall; ++i) {
        float x, y;
        fin >> x >> y;
        EulerianRoundBall* RoundBall = new EulerianRoundBall(Vector2{x, y + Offset.y}, RED, 1.0f);
        RoundBall -> m_Color = BallColor;
        m_RoundBallList.push_back(RoundBall);
    }
    fin.close();
    for (auto &ball: m_RoundBallList) {
        m_Engine.attachRoundBall(ball);
        ball -> m_Radius = 5.0f;
    }
    for (int i = 0; i < m_RoundBallList.size(); ++i) {
        int NextBallIndex = (i + 1) % m_RoundBallList.size();
        float Distance = Vector2Distance(m_RoundBallList[i] -> m_CurrentPosition, m_RoundBallList[NextBallIndex] -> m_CurrentPosition);
        Spring* Spring1 = new Spring(m_RoundBallList[i], m_RoundBallList[NextBallIndex], Distance, 5000.0f, RED);
        Spring1 -> setDamping(true, 20);
        Spring1 ->setColor(SpringColor);
        m_SpringList.push_back(Spring1);
        m_Engine.attachSpring(Spring1);

    }

}
void PressureSoftBodyState::onNotify() {
    exitState();
}
void PressureSoftBodyState::draw() {
    m_Engine.draw();
    Vector2& DrawPosition = Center;
//    DrawCircle(Center.x, Center.y, 5, RED);
    Vector2 Direction = Vector2Normalize(Vector2Subtract(m_RoundBallList[0]->m_CurrentPosition, Center));
    Vector2 XVector = {1, 0};
    float Angle = angle360InRadian(XVector, Direction);
    Angle = radianToDegree(Angle);
    Rectangle SourceRec = {0, 0, float(m_Smiley.width), float(m_Smiley.height)};
    Rectangle DestRec = {DrawPosition.x, DrawPosition.y, m_SmileySize.x, m_SmileySize.y};
    DrawTexturePro(m_Smiley, SourceRec, DestRec, Vector2{float(m_SmileySize.x /2 ), float(m_SmileySize.y / 2)}, Angle, WHITE);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 MousePosition = GetMousePosition();
        Vector2 ClosestPoint = m_RoundBallList[0] -> m_CurrentPosition;
        for (auto& ball : m_RoundBallList)
        {
            if (Vector2Distance(ball -> m_CurrentPosition, MousePosition) < Vector2Distance(ClosestPoint, MousePosition))
            {
                ClosestPoint = ball -> m_CurrentPosition;
            }
        }
        DrawLineEx(MousePosition, ClosestPoint, 5, ColorAlpha(RED, 0.2));
        DrawCircle(MousePosition.x, MousePosition.y, 7, RED);
    }
    DrawText("Pressure Soft Body State", 100, 10, 20, RED);
    DrawText("Click and hold to pull smiley around", 100, 30, 20, RED);
}
SimulationState* PressureSoftBodyState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    int SubStep = 10;
    for (int i = 0; i < SubStep; ++i) {
        static float NRT = 0.0821 * 300 * 100000;

        vector<Vector2> Polygon;
        for (auto& ball : m_RoundBallList)
        {
            Polygon.push_back(ball -> m_CurrentPosition);
        }
        float Area = calculateAreaPolygon(Polygon);
        float Pressure = NRT / Area;
        Center = {0, 0};
        for (auto &ball: m_RoundBallList) {
            Center = Vector2Add(Center, ball -> m_CurrentPosition);
        }
        Center = Vector2Scale(Center, 1.0f / m_RoundBallList.size());
        for (auto& spring : m_SpringList)
        {
            LineSegment SpringLine = spring -> getLineSegment();
            Vector2 Projection = Line(SpringLine).projection(Center);
            Vector2 Normal = Vector2Normalize(Vector2Subtract(Projection, Center));
            Normal = Vector2Normalize(Normal);
            float Length = SpringLine.getLength();
            float Force = Pressure * Length;
            Vector2 ForceVector = Vector2Scale(Normal, Force);
            spring -> getBalls().first ->applyForce(ForceVector);
            spring -> getBalls().second ->applyForce(ForceVector);
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 MousePosition = GetMousePosition();
            for (auto& ball : m_RoundBallList)
            {
                Vector2 Direction = Vector2Subtract(MousePosition, ball -> m_CurrentPosition);
                Direction = Vector2Normalize(Direction);
                Vector2 Force = Vector2Scale(Direction, 800);
                ball -> applyForce(Force);
            }
        }
        m_Engine.update(m_FrameTime / SubStep);
    }
    return nullptr;
}
SimulationState* ShapeMatchingSoftBodyState::getShapeMatchingSoftBodyState() {
    static ShapeMatchingSoftBodyState MyShapeMatchingSoftBodyState;
    return &MyShapeMatchingSoftBodyState;
}
ShapeMatchingSoftBodyState::ShapeMatchingSoftBodyState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SHAPE_MATCHING_SOFT_BODY_STATE;
    reset();
}
ShapeMatchingSoftBodyState::~ShapeMatchingSoftBodyState() {
    m_IsActive = true;
    delete m_ShapeA;
    delete m_ShapeH;
    delete m_ShapeL;
    delete m_ShapeN;
}
void ShapeMatchingSoftBodyState::reset() {
    m_IsActive = true;
    m_Engine.reset();
    m_Engine.turnOffProximityColoring();
    m_Engine.turnOffMutualAcceleration();
    m_Engine.setGravity({0, 300});
    readShapeFiles();
    attachShapeToEngine();
    m_ShapeA -> rotate(0);
    m_ShapeH -> rotate(-5);
    m_ShapeL -> rotate(5);
    m_ShapeN -> rotate(25);
}
void ShapeMatchingSoftBodyState::readShapeFiles() {
    Color ColorA = Color(239, 221, 178, 255);
    Color ColorH = Color(29, 85, 111, 255);
    Color ColorL = Color(40, 143, 180, 255);
    Color ColorN = Color(250, 54, 10, 255);
    ifstream fin;
    fin.open("TextData/A_processed.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    vector<Vector2> A;
    for (int i = 0; i < 13; ++i)
    {
        float x, y;
        fin >> x >> y;
        A.push_back(Vector2{x, y});
    }
    Vector2 Position_A = {900, 100};
    m_ShapeA = new ShapeA();
    m_ShapeA -> init(A, Position_A);
    m_ShapeA ->setColor(ColorA);
    fin.close();
    fin.open("TextData/H_processed.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    vector<Vector2> H;
    for (int i = 0; i < 20; ++i)
    {
        float x, y;
        fin >> x >> y;
        H.push_back(Vector2{x, y});
    }
    Vector2 Position_H = {700, 100};
    m_ShapeH = new ShapeH();
    m_ShapeH -> init(H, Position_H);
    m_ShapeH -> setColor(ColorH);
    fin.close();
    fin.open("TextData/L_processed.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    vector<Vector2> L;
    for (int i = 0; i < 10; ++i)
    {
        float x, y;
        fin >> x >> y;
        L.push_back(Vector2{x, y});
    }
    Vector2 Position_L = {400, 100};
    m_ShapeL = new ShapeL();
    m_ShapeL -> init(L, Position_L);
    m_ShapeL ->setColor(ColorL);
    fin.close();
    fin.open("TextData/N_processed.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    vector<Vector2> N;
    for (int i = 0; i < 22; ++i)
    {
        float x, y;
        fin >> x >> y;
        N.push_back(Vector2{x, y});
    }
    Vector2 Position_N = {1200, 100};
    m_ShapeN = new ShapeN();
    m_ShapeN -> init(N, Position_N);
    m_ShapeN ->setColor(ColorN);
    fin.close();
}
SimulationState* ShapeMatchingSoftBodyState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_Engine.update(m_FrameTime);
    m_ShapeA -> update();
    m_ShapeH -> update();
    m_ShapeL -> update();
    m_ShapeN -> update();

    return nullptr;
}
void ShapeMatchingSoftBodyState::draw() {
    m_Engine.draw();
    m_ShapeA -> draw();
    m_ShapeH -> draw();
    m_ShapeL -> draw();
    m_ShapeN -> draw();

}
void ShapeMatchingSoftBodyState::onNotify() {
    exitState();
}
void ShapeMatchingSoftBodyState::Shape::draw() {
//    for (int i = 0; i < m_FrameVertices.size(); ++i)
//    {
//        m_FrameVertices[i]->draw();
//        DrawLineV( m_FrameVertices[i] -> m_CurrentPosition, m_FrameVertices[(i + 1) % m_FrameVertices.size()] -> m_CurrentPosition, GREEN);
//    }
//    for (int i = 0; i < m_FrameVertices.size(); ++i)
//    {
//        DrawCircleV(m_RotatedFrame[i], 5,  PURPLE);
//    }
//    DrawCircle(m_Center.x, m_Center.y, 5, BLUE);
//    DrawCircle(m_OriginalCenter.x, m_OriginalCenter.y, 5, PURPLE);
//    for (auto& position : m_UnRotatedFrame)
//    {
//        DrawCircleV(position, 5, BLACK);
//    }
//    for (auto& band : m_RubberBands)
//    {
//        band -> draw();
//    }
//    for (int i = 0; i < m_FrameVertices.size(); ++i)
//    {
//        DrawLineEx(m_Center, m_UnRotatedFrame[i], 5, BLUE);
//        DrawLineEx(m_Center, m_ShapeVertices[i] -> m_CurrentPosition, 5, BLUE);
//    }
    for (auto& ball : m_ShapeVertices)
    {
        ball -> drawCustomRadius(m_CircleDrawingRadius);
    }
    for (auto& spring : m_Springs)
    {
        spring -> draw();
    }
}
void ShapeMatchingSoftBodyState::Shape::init(vector<Vector2> FrameVertices, Vector2 Position) {
    for (int i = 0; i < FrameVertices.size(); ++i)
    {
        Vector2 RealPosition = Vector2Add(FrameVertices[i], Position);
        EulerianRoundBall* NewBall = new EulerianRoundBall(RealPosition, GREEN, 1.0f);
        NewBall -> m_Radius = 7.0f;
        NewBall ->m_Fixed = true;
        NewBall -> m_Radius = 5.0f;
        m_FrameVertices.push_back(NewBall);
        m_OriginalFrame.push_back(NewBall -> m_CurrentPosition);
        m_RotatedFrame.push_back(NewBall -> m_CurrentPosition);
        m_UnRotatedFrame.push_back(NewBall -> m_CurrentPosition);
        EulerianRoundBall* NewBall2 = new EulerianRoundBall(RealPosition, BLACK, 1.0f);
        NewBall2 -> m_Radius = m_CircleRadius;
        NewBall2 -> m_Mass = 1.5f;
        NewBall2 ->setVisibility(false);
        m_ShapeVertices.push_back(NewBall2);
        Spring *RubberBand1 = new Spring(NewBall, NewBall2, 5, 500, RED);
        RubberBand1->setDamping(true, 25);
        RubberBand1->setColor(YELLOW);
        m_RubberBands.push_back(RubberBand1);
    }
    for (int i = 0; i < m_FrameVertices.size(); ++i)
    {
        int NextIndex = (i + 1) % m_FrameVertices.size();
        Spring *ShapeSpring = new Spring(m_ShapeVertices[i], m_ShapeVertices[NextIndex], Vector2Distance(m_ShapeVertices[i] -> m_CurrentPosition, m_ShapeVertices[NextIndex] -> m_CurrentPosition), 1000, RED);
        ShapeSpring -> setDamping(true, 30);
        ShapeSpring ->setThickness(12);
        ShapeSpring ->setColor(BLACK);

        m_Springs.push_back(ShapeSpring);
    }
    calculateOriginalCenter();
    calculateShapeCenter();

}
void ShapeMatchingSoftBodyState::Shape::attachToEngine(DiscreteEulerianEngine *Engine) {
    for (auto& ball : m_ShapeVertices)
    {
        Engine -> attachRoundBall(ball);
    }

}
ShapeMatchingSoftBodyState::Shape::~Shape()
{
    for (auto& ball : m_FrameVertices)
    {
        delete ball;
        ball = nullptr;
    }
    m_FrameVertices.clear();
    for (auto& ball : m_ShapeVertices)
    {
        delete ball;
        ball = nullptr;
    }
    m_ShapeVertices.clear();
    for (auto& spring : m_Springs)
    {
        delete spring;
        spring = nullptr;
    }
    m_Springs.clear();
    for (auto& band : m_RubberBands)
    {
        delete band;
        band = nullptr;
    }
    m_RubberBands.clear();
}
void ShapeMatchingSoftBodyState::attachShapeToEngine() {
    m_ShapeA -> attachToEngine(&m_Engine);
    m_ShapeH -> attachToEngine(&m_Engine);
    m_ShapeL -> attachToEngine(&m_Engine);
    m_ShapeN -> attachToEngine(&m_Engine);
}
void ShapeMatchingSoftBodyState::Shape::calculateShapeCenter() {
    static bool FirstTime = true;
    m_PreviousCenter = m_Center;
    m_Center = {0, 0};
    for (auto& ball : m_ShapeVertices)
    {
        m_Center = Vector2Add(m_Center, ball -> m_CurrentPosition);
    }
    m_Center = Vector2Scale(m_Center, 1.0f / m_ShapeVertices.size());
    if (FirstTime)
    {
        m_PreviousCenter = m_Center;
        FirstTime = false;
    }
}
void ShapeMatchingSoftBodyState::Shape::calculateOriginalCenter() {
    m_OriginalCenter = {0, 0};
    for (auto& ball : m_OriginalFrame)
    {
        m_OriginalCenter = Vector2Add(m_OriginalCenter, ball);
    }
    m_OriginalCenter = Vector2Scale(m_OriginalCenter, 1.0f / m_OriginalFrame.size());
}
void ShapeMatchingSoftBodyState::Shape::update() {
    for (auto& spring : m_Springs)
    {
        spring -> update();
    }
    for (auto& band : m_RubberBands)
    {
        band -> update();
    }
    calculateShapeCenter();
    selfRotate();
}
void ShapeMatchingSoftBodyState::Shape::rotate(float Degree)
{
    calculateShapeCenter();
    for (int i = 0; i < m_FrameVertices.size(); ++i)
    {
        Vector2 Direction = Vector2Subtract(m_FrameVertices[i] -> m_CurrentPosition, m_Center);
        float Radian = degreeToRadian(Degree);
        Direction = Vector2Rotate(Direction, Radian);
        m_FrameVertices[i] -> m_CurrentPosition = Vector2Add(m_Center, Direction);
    }
}
void ShapeMatchingSoftBodyState::Shape::selfRotate() {
    // calculate unrotated frame
    Vector2 Displacement = Vector2Subtract(m_Center, m_OriginalCenter);
    for (int i = 0; i < m_FrameVertices.size(); ++i)
    {
        m_FrameVertices[i] -> m_CurrentPosition = Vector2Add(m_RotatedFrame[i], Displacement);
        m_UnRotatedFrame[i] = Vector2Add(m_OriginalFrame[i], Displacement);
    }
    // calculate average angle between shape and unrotated frame
    Vector2 Sum = {0, 0};
    Vector2 Sum2 = {0, 0};
    for (int i = 0; i < m_FrameVertices.size(); ++i)
    {
        Vector2 ShapeVertexDirection = Vector2Subtract(m_ShapeVertices[i] -> m_CurrentPosition, m_Center);
        Vector2 UnRotatedDirection = Vector2Subtract(m_UnRotatedFrame[i], m_Center);
        if (m_ShapeVertices[i] -> m_CurrentPosition.x > m_Center.x)
        {
            Sum = Vector2Add(Sum, ShapeVertexDirection);
            Sum2 = Vector2Add(Sum2, UnRotatedDirection);
        }
    }
    Angle = angle360InRadian(Sum2, Sum);
    rotateFromZero(Angle);
//    std::cout << "Angle:" << radianToDegree(Angle) <<std::endl;
//    static float TotalDegree = 0;
//    if (IsKeyDown(KEY_LEFT))
//    {
//        TotalDegree -= 0.1;
//    }
//    if (IsKeyDown(KEY_RIGHT))
//    {
//        TotalDegree += 0.1;
//    }
//    rotateFromZero(TotalDegree);
}
void ShapeMatchingSoftBodyState::Shape::rotateFromZero(float Radian) {
    calculateShapeCenter();
    for (int i = 0; i < m_FrameVertices.size(); ++i)
    {
        Vector2 Direction = Vector2Subtract(m_UnRotatedFrame[i], m_Center);
        Direction = Vector2Rotate(Direction, Radian);
        m_FrameVertices[i] -> m_CurrentPosition = Vector2Add(m_Center, Direction);
    }
}
void ShapeMatchingSoftBodyState::ShapeA::draw() {
    DrawTriangle(m_ShapeVertices[0] -> m_CurrentPosition, m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[0] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_ShapeVertices[12] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[12] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_ShapeVertices[11] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[5] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[5] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[4] -> m_CurrentPosition, m_ShapeVertices[5] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[6] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[11] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[10] -> m_CurrentPosition, m_ShapeVertices[11] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[8] -> m_CurrentPosition, m_ShapeVertices[10] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[8] -> m_CurrentPosition, m_ShapeVertices[9] -> m_CurrentPosition, m_ShapeVertices[10] -> m_CurrentPosition, m_Color);
    Shape::draw();
}
void ShapeMatchingSoftBodyState::ShapeH::draw() {
    DrawTriangle(m_ShapeVertices[0] -> m_CurrentPosition, m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[19] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[18] -> m_CurrentPosition, m_ShapeVertices[19] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[18] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[17] -> m_CurrentPosition, m_ShapeVertices[18] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[17] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_ShapeVertices[17] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[4] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[4] -> m_CurrentPosition, m_ShapeVertices[5] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[6] -> m_CurrentPosition, m_ShapeVertices[16] -> m_CurrentPosition, m_ShapeVertices[17] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[6] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[16] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[8] -> m_CurrentPosition, m_ShapeVertices[9] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[9] -> m_CurrentPosition, m_ShapeVertices[10] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[16] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[10] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[16] -> m_CurrentPosition, m_ShapeVertices[10] -> m_CurrentPosition, m_ShapeVertices[11] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[15] -> m_CurrentPosition, m_ShapeVertices[16] -> m_CurrentPosition, m_ShapeVertices[11] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[15] -> m_CurrentPosition, m_ShapeVertices[11] -> m_CurrentPosition, m_ShapeVertices[12] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[14] -> m_CurrentPosition, m_ShapeVertices[15] -> m_CurrentPosition, m_ShapeVertices[12] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[14] -> m_CurrentPosition, m_ShapeVertices[12] -> m_CurrentPosition, m_ShapeVertices[13] -> m_CurrentPosition, m_Color);
    Shape::draw();

}
void ShapeMatchingSoftBodyState::ShapeL::draw() {
    DrawTriangle(m_ShapeVertices[0] -> m_CurrentPosition, m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[9] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[8] -> m_CurrentPosition, m_ShapeVertices[9] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[8] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[4] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[4] -> m_CurrentPosition, m_ShapeVertices[5] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[4] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_Color);
    Shape::draw();
}
void ShapeMatchingSoftBodyState::ShapeN::draw() {
    DrawTriangle(m_ShapeVertices[0] -> m_CurrentPosition, m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[21] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[8] -> m_CurrentPosition, m_ShapeVertices[21] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[1] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_ShapeVertices[8] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[2] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_ShapeVertices[7] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[4] -> m_CurrentPosition, m_ShapeVertices[5] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[3] -> m_CurrentPosition, m_ShapeVertices[5] -> m_CurrentPosition, m_ShapeVertices[6] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[8] -> m_CurrentPosition, m_ShapeVertices[20] -> m_CurrentPosition, m_ShapeVertices[21] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[8] -> m_CurrentPosition, m_ShapeVertices[9] -> m_CurrentPosition, m_ShapeVertices[20] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[9] -> m_CurrentPosition, m_ShapeVertices[19] -> m_CurrentPosition, m_ShapeVertices[20] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[9] -> m_CurrentPosition, m_ShapeVertices[10] -> m_CurrentPosition, m_ShapeVertices[19] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[10] -> m_CurrentPosition, m_ShapeVertices[11] -> m_CurrentPosition, m_ShapeVertices[12] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[10] -> m_CurrentPosition, m_ShapeVertices[12] -> m_CurrentPosition, m_ShapeVertices[13] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[10] -> m_CurrentPosition, m_ShapeVertices[13] -> m_CurrentPosition, m_ShapeVertices[19] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[13] -> m_CurrentPosition, m_ShapeVertices[14] -> m_CurrentPosition, m_ShapeVertices[19] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[18] -> m_CurrentPosition, m_ShapeVertices[19] -> m_CurrentPosition, m_ShapeVertices[14] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[18] -> m_CurrentPosition, m_ShapeVertices[14] -> m_CurrentPosition, m_ShapeVertices[15] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[17] -> m_CurrentPosition, m_ShapeVertices[18] -> m_CurrentPosition, m_ShapeVertices[15] -> m_CurrentPosition, m_Color);
    DrawTriangle(m_ShapeVertices[17] -> m_CurrentPosition, m_ShapeVertices[15] -> m_CurrentPosition, m_ShapeVertices[16] -> m_CurrentPosition, m_Color);
    Shape::draw();
}
SimulationState* SATTriangleState::getSATTriangleState() {
    static SATTriangleState MySATTriangleState;
    return &MySATTriangleState;
}
SATTriangleState::SATTriangleState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SAT_TRIANGLE_STATE;
    reset();
}
SATTriangleState::~SATTriangleState() {
    m_IsActive = true;
    for (auto& triangle : m_TriangleList)
    {
        delete triangle;
        triangle = nullptr;
    }
    m_TriangleList.clear();
}
void SATTriangleState::reset() {
    m_IsActive = true;
    for (auto& triangle : m_TriangleList)
    {
        delete triangle;
        triangle = nullptr;
    }
    m_TriangleList.clear();
    readCoordinate();
}
void SATTriangleState::onNotify() {
    exitState();
}
void SATTriangleState::draw() {
    DrawText("SAT Triangle State", 100, 10, 20, RED);
    m_Engine.draw();
    for (auto& triangle : m_TriangleList)
    {
        triangle -> draw();
    }
    DrawText("Use arrow keys to move the triangle", 100, 10, 20, RED);

}
SimulationState* SATTriangleState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_TriangleList[0] ->rotate(0.05);
    if (IsKeyDown(KEY_LEFT))
    {
        m_TriangleList[0] -> move(Vector2{-4, 0});
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        m_TriangleList[0] -> move(Vector2{4, 0});
    }
    if (IsKeyDown(KEY_UP))
    {
        m_TriangleList[0] -> move(Vector2{0, -4});
    }
    if (IsKeyDown(KEY_DOWN))
    {
        m_TriangleList[0] -> move(Vector2{0, 4});
    }
    SATCollider* Collider = TriangleSATCollider::getTriangleSATCollider();
    TriangleSATCollider* TriangleCollider = dynamic_cast<TriangleSATCollider*>(Collider);
    for (int i = 1; i < m_TriangleList.size(); ++i)
    {
        if (TriangleCollider -> isColliding(m_TriangleList[0]->getVertices(), m_TriangleList[i]->getVertices()))
        {
            m_TriangleList[i]->setActive(true);
        }
        else
        {
            m_TriangleList[i]->setActive(false);
        }
    }
    return nullptr;
}
void SATTriangleState::readCoordinate() {
    ifstream fin;
    fin.open("TriangleCoordinates.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    int NumberOfTriangles;
    fin >> NumberOfTriangles;
    Vector2 Temp[3];
    for (int i = 0; i < NumberOfTriangles; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            float x, y;
            fin >> x >> y;
            Temp[j] = Vector2{x, y};
        }
        SATPlatformTriangle* NewTriangle = new SATPlatformTriangle(Temp[0], Temp[1], Temp[2], RED);
        m_TriangleList.push_back(NewTriangle);
    }
}
SimulationState* SATPolygonState::getSATPolygonState() {
    static SATPolygonState MySATPolygonState;
    return &MySATPolygonState;
}
SATPolygonState::SATPolygonState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SAT_POLYGON_STATE;
    reset();
}
SATPolygonState::~SATPolygonState() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
}
void SATPolygonState::reset() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    m_Engine.reset();
    readCoordinate();
    for (auto& polygon : m_PolygonList)
    {
        m_Engine.attachSATPolygon(polygon);
    }
    m_Engine.turnOnOffPlayerControl(true);
    int ObjectType = DiscreteSATEulerianEngine::CONTROL_OBJECT::POLYGON;
    m_Engine.setObjectTypeToControl(ObjectType);
}
void SATPolygonState::onNotify() {
    exitState();
}
void SATPolygonState::draw() {
    m_Engine.draw();
    DrawText("SAT Polygon State", 100, 10, 20, RED);
    DrawText("Use arrow keys to move the polygon", 100, 30, 20, RED);
    DrawText("Use 1 and 2 to rotate the polygon", 100, 50, 20, RED);
}
SimulationState* SATPolygonState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_Engine.update(m_FrameTime);
    return nullptr;
}
void SATPolygonState::readCoordinate() {
    ifstream fin;
    fin.open("PolygonCoordinates.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    int NumberOfPolygons;
    fin >> NumberOfPolygons;
    for (int i = 0; i < NumberOfPolygons; ++i)
    {
        int NumberOfVertices;
        fin >> NumberOfVertices;
        vector<Vector2> Vertices;
        for (int j = 0; j < NumberOfVertices; ++j)
        {
            float x, y;
            fin >> x >> y;
            Vertices.push_back(Vector2{x, y});
        }
        SATPlatformPolygon* NewPolygon = new SATPlatformPolygon(Vertices, RED);
        m_PolygonList.push_back(NewPolygon);
    }
    fin.close();
}
SimulationState* SATCirclePolygonState::getSATCirclePolygonState() {
    static SATCirclePolygonState MySATCirclePolygonState;
    return &MySATCirclePolygonState;
}
SATCirclePolygonState::SATCirclePolygonState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SAT_CIRCLE_POLYGON_STATE;
    m_ColorList.push_back(Color{22, 114, 136, 255});
    m_ColorList.push_back(Color{140, 218, 236, 255});
    m_ColorList.push_back(Color{180, 82, 72, 255});
    m_ColorList.push_back(Color{212, 140, 132, 255});
    m_ColorList.push_back(Color{168, 154, 73, 255});
    m_ColorList.push_back(Color{214, 207, 162, 255});
    m_ColorList.push_back(Color{60, 180, 100, 255});
    m_ColorList.push_back(Color{155, 221, 177, 255});
    m_ColorList.push_back(Color{100, 60, 106, 255});
    m_ColorList.push_back(Color{131, 99, 148, 255});
    reset();
}
SATCirclePolygonState::~SATCirclePolygonState() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
}
void SATCirclePolygonState::reset() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
    m_Engine.reset();
    readCoordinate();
    for (auto& polygon : m_PolygonList)
    {
        m_Engine.attachSATPolygon(polygon);
    }
    for (auto& circle : m_CircleList)
    {
        m_Engine.attachSATCircle(circle);
    }
    m_Engine.turnOnOffPlayerControl(true);
    int ObjectType = DiscreteSATEulerianEngine::CONTROL_OBJECT::CIRCLE;
    m_Engine.setObjectTypeToControl(ObjectType);

}
void SATCirclePolygonState::onNotify() {
    exitState();
}
void SATCirclePolygonState::readCoordinate() {
    ifstream fin;
    fin.open("PolygonCoordinates.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    int NumberOfPolygons;
    fin >> NumberOfPolygons;
    for (int i = 0; i < NumberOfPolygons; ++i)
    {
        int NumberOfVertices;
        fin >> NumberOfVertices;
        vector<Vector2> Vertices;
        for (int j = 0; j < NumberOfVertices; ++j)
        {
            float x, y;
            fin >> x >> y;
            Vertices.push_back(Vector2{x, y});
        }
        SATPlatformPolygon* NewPolygon = new SATPlatformPolygon(Vertices, RED);
        NewPolygon->setCustomColor(m_ColorList[i + 6]);
        m_PolygonList.push_back(NewPolygon);
    }
    fin.close();
    fin.open("CircleCoordinate.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    int NumberOfCircles;
    fin >> NumberOfCircles;
    for (int i = 0; i < NumberOfCircles; ++i) {
        float x, y;
        fin >> x >> y;
        SATPlatformCircle *NewCircle = new SATPlatformCircle(Vector2{x, y}, m_ColorList[i], 1.0f);
        switch (i) {
            case 0:
                NewCircle->setRadius(80);
                break;
            case 1:
                NewCircle->setRadius(50);
                break;
            case 2:
                NewCircle->setRadius(60);
                break;
            case 3:
                NewCircle->setRadius(70);
                break;
            case 4:
                NewCircle->setRadius(40);
                break;
            case 5:
                NewCircle->setRadius(90);
                break;
            case 6:
                NewCircle->setRadius(100);
                break;
        }

        m_CircleList.push_back(NewCircle);
    }
}
void SATCirclePolygonState::draw() {
    m_Engine.draw();
}
SimulationState* SATCirclePolygonState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_Engine.update(m_FrameTime);
    return nullptr;
}
SATResponseState::SATResponseState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SAT_RESPONSE_STATE;
    m_ColorList.push_back(Color{22, 114, 136, 255});
    m_ColorList.push_back(Color{140, 218, 236, 255});
    m_ColorList.push_back(Color{180, 82, 72, 255});
    m_ColorList.push_back(Color{212, 140, 132, 255});
    m_ColorList.push_back(Color{168, 154, 73, 255});
    m_ColorList.push_back(Color{214, 207, 162, 255});
    m_ColorList.push_back(Color{60, 180, 100, 255});
    m_ColorList.push_back(Color{155, 221, 177, 255});
    m_ColorList.push_back(Color{100, 60, 106, 255});
    m_ColorList.push_back(Color{131, 99, 148, 255});
    m_Engine.setControlType(DiscreteSATEulerianEngine::CONTROL_TYPE::ACCELERATE_CONTROL);
    m_Engine.setObjectTypeToControl(DiscreteSATEulerianEngine::CONTROL_OBJECT::CIRCLE);
    reset();
}
SATResponseState::~SATResponseState() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
}
void SATResponseState::reset() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
    m_Engine.reset();
    readCoordinate();
    for (auto& polygon : m_PolygonList)
    {
        m_Engine.attachSATPolygon(polygon);
    }
    for (auto& circle : m_CircleList)
    {
        m_Engine.attachSATCircle(circle);
    }
    m_Engine.turnOnOffPlayerControl(true);
    int ObjectType = DiscreteSATEulerianEngine::CONTROL_OBJECT::CIRCLE;
    m_Engine.setObjectTypeToControl(ObjectType);
}
void SATResponseState::onNotify() {
    exitState();
}
void SATResponseState::readCoordinate() {
    ifstream fin;
    fin.open("PolygonCoordinates.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    int NumberOfPolygons;
    fin >> NumberOfPolygons;
    for (int i = 0; i < NumberOfPolygons; ++i)
    {
        int NumberOfVertices;
        fin >> NumberOfVertices;
        vector<Vector2> Vertices;
        for (int j = 0; j < NumberOfVertices; ++j)
        {
            float x, y;
            fin >> x >> y;
            Vertices.push_back(Vector2{x, y});
        }
        SATPlatformPolygon* NewPolygon = new SATPlatformPolygon(Vertices, RED);
        NewPolygon->setCustomColor(m_ColorList[i + 6]);
        m_PolygonList.push_back(NewPolygon);
    }
    fin.close();
    fin.open("CircleCoordinate.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    int NumberOfCircles;
    fin >> NumberOfCircles;
    for (int i = 0; i < NumberOfCircles; ++i) {
        float x, y;
        fin >> x >> y;
        SATPlatformCircle *NewCircle = new SATPlatformCircle(Vector2{x, y}, m_ColorList[i], 1.0f);
        switch (i) {
            case 0:
                NewCircle->setRadius(80);
                break;
            case 1:
                NewCircle->setRadius(50);
                break;
            case 2:
                NewCircle->setRadius(60);
                break;
            case 3:
                NewCircle->setRadius(70);
                break;
            case 4:
                NewCircle->setRadius(40);
                break;
            case 5:
                NewCircle->setRadius(90);
                break;
            case 6:
                NewCircle->setRadius(100);
                break;
        }

        m_CircleList.push_back(NewCircle);
    }
}
void SATResponseState::draw() {
    m_Engine.draw();
    DrawText("SAT Response State", 100, 10, 20, RED);
    DrawText("Use arrow keys to move", 100, 30, 20, RED);
    DrawText("Use 1 to select circle and 2 to select polygon", 100, 50, 20, RED);
}
SimulationState* SATResponseState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    int SubStep = 8;
    for (int i = 0; i < SubStep; ++i)
    {
        m_Engine.update(m_FrameTime / (float)SubStep);
    }
    if (IsKeyPressed(KEY_ONE))
    {
        m_Engine.setObjectTypeToControl(DiscreteSATEulerianEngine::CONTROL_OBJECT::CIRCLE);
    }
    if (IsKeyPressed(KEY_TWO))
    {
        m_Engine.setObjectTypeToControl(DiscreteSATEulerianEngine::CONTROL_OBJECT::POLYGON);
    }
    return nullptr;
}
SimulationState* SATResponseState::getSATResponseState() {
    static SATResponseState MySATResponseState;
    return &MySATResponseState;
}
SimulationState* SATGravityAndContactPointsState::getSATGravityAndContactPointsState() {
    static SATGravityAndContactPointsState MySATGravityAndContactPointsState;
    return &MySATGravityAndContactPointsState;
}
SATGravityAndContactPointsState::SATGravityAndContactPointsState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SAT_GRAVITY_AND_CONTACT_POINTS_STATE;
    m_ColorList.push_back(Color{22, 114, 136, 255});
    m_ColorList.push_back(Color{140, 218, 236, 255});
    m_ColorList.push_back(Color{180, 82, 72, 255});
    m_ColorList.push_back(Color{212, 140, 132, 255});
    m_ColorList.push_back(Color{168, 154, 73, 255});
    m_ColorList.push_back(Color{214, 207, 162, 255});
    m_ColorList.push_back(Color{60, 180, 100, 255});
    m_ColorList.push_back(Color{155, 221, 177, 255});
    m_ColorList.push_back(Color{100, 60, 106, 255});
    m_ColorList.push_back(Color{131, 99, 148, 255});
    reset();
}
SATGravityAndContactPointsState::~SATGravityAndContactPointsState() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
}
void SATGravityAndContactPointsState::reset() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
    m_Engine.reset();
    readCoordinate();
    for (auto& polygon : m_PolygonList)
    {
        m_Engine.attachSATPolygon(polygon);
    }
    for (auto& circle : m_CircleList)
    {
        m_Engine.attachSATCircle(circle);
    }
    m_Engine.turnOnOffPlayerControl(false);
    m_Engine.setEngineMode(DiscreteSATEulerianEngine::ENGINE_MODE::ACCELERATE_ENGINE);
    m_Engine.turnOnOffGravity(true);
    m_Engine.turnOnOffDisplayContactPoint(true);
}
void SATGravityAndContactPointsState::onNotify() {
    exitState();
}
void SATGravityAndContactPointsState::readCoordinate() {
    ifstream fin;
    fin.open("PolygonCoordinates.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    int NumberOfPolygons;
    fin >> NumberOfPolygons;
    for (int i = 0; i < NumberOfPolygons; ++i)
    {
        int NumberOfVertices;
        fin >> NumberOfVertices;
        vector<Vector2> Vertices;
        for (int j = 0; j < NumberOfVertices; ++j)
        {
            float x, y;
            fin >> x >> y;
            Vertices.push_back(Vector2{x, y});
        }
        SATPlatformPolygon* NewPolygon = new SATPlatformPolygon(Vertices, RED);
        NewPolygon->setCustomColor(m_ColorList[i + 6]);
        m_PolygonList.push_back(NewPolygon);
    }
    fin.close();
    fin.open("CircleCoordinate.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    int NumberOfCircles;
    fin >> NumberOfCircles;
    for (int i = 0; i < NumberOfCircles; ++i) {
        float x, y;
        fin >> x >> y;
        SATPlatformCircle *NewCircle = new SATPlatformCircle(Vector2{x, y}, m_ColorList[i], 1.0f);
        switch (i) {
            case 0:
                NewCircle->setRadius(80);
                break;
            case 1:
                NewCircle->setRadius(50);
                break;
            case 2:
                NewCircle->setRadius(60);
                break;
            case 3:
                NewCircle->setRadius(70);
                break;
            case 4:
                NewCircle->setRadius(40);
                break;
            case 5:
                NewCircle->setRadius(90);
                break;
            case 6:
                NewCircle->setRadius(100);
                break;
        }

        m_CircleList.push_back(NewCircle);
    }
}
void SATGravityAndContactPointsState::draw() {
    m_Engine.draw();
    DrawText("SAT Gravity And Contact Points State", 100, 10, 20, RED);
}
SimulationState* SATGravityAndContactPointsState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    int SubStep = 8;
    for (int i = 0; i < SubStep; ++i) {
        m_Engine.update(m_FrameTime / (float)SubStep);
    }
    return nullptr;
}
SATRotatingState::SATRotatingState() : m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::SAT_FIXED_AND_ROTATING_STATE;
    // m_ColorList.push_back(Color{22, 114, 136, 255});
    // m_ColorList.push_back(Color{140, 218, 236, 255});
    // m_ColorList.push_back(Color{180, 82, 72, 255});
    // m_ColorList.push_back(Color{212, 140, 132, 255});
    // m_ColorList.push_back(Color{168, 154, 73, 255});
    // m_ColorList.push_back(Color{214, 207, 162, 255});
    // m_ColorList.push_back(Color{60, 180, 100, 255});
    // m_ColorList.push_back(Color{155, 221, 177, 255});
    // m_ColorList.push_back(Color{100, 60, 106, 255});
    // m_ColorList.push_back(Color{131, 99, 148, 255});
    reset();
}
SATRotatingState::~SATRotatingState() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
}
void SATRotatingState::reset() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
    m_Engine.reset();
    // readCoordinate();
    Color CREAM = Color{255, 250, 236, 255};
    Color GRASS = Color{87, 142, 126, 255};
    std::vector<Vector2> StaticPlatform;
    StaticPlatform.push_back(Vector2{241.2956805081648,293.1547070443129});
    StaticPlatform.push_back(Vector2{669.7839149336057,429.44417768163385});
    StaticPlatform.push_back(Vector2{625.8212141183002,567.6610183871973});
    StaticPlatform.push_back(Vector2{197.3329796928593,431.3715477498762});
    SATRotatingPlatformPolygon* NewPolygon = new SATRotatingPlatformPolygon(StaticPlatform, GRASS);
    NewPolygon->setFixed(true);
    m_PolygonList.push_back(NewPolygon);
    std::vector<Vector2> RotatingPlatform;
    RotatingPlatform.push_back(Vector2{200,100});
    RotatingPlatform.push_back(Vector2{300,100});
    RotatingPlatform.push_back(Vector2{300,200});
    RotatingPlatform.push_back(Vector2{200,200});
    SATRotatingPlatformPolygon* NewPolygon2 = new SATRotatingPlatformPolygon(RotatingPlatform, CREAM);
    m_PolygonList.push_back(NewPolygon2);
    std::vector<Vector2> RotatingPlatform2;
    RotatingPlatform2.push_back(Vector2{400,100});
    RotatingPlatform2.push_back(Vector2{500,100});
    RotatingPlatform2.push_back(Vector2{500,200});
    RotatingPlatform2.push_back(Vector2{400,200});
    SATRotatingPlatformPolygon* NewPolygon3 = new SATRotatingPlatformPolygon(RotatingPlatform2, CREAM);
    m_PolygonList.push_back(NewPolygon3);
    setupBoundaries();
    for (auto& polygon : m_PolygonList)
    {
        m_Engine.attachRotatingPlatformPolygon(polygon);
    }
    for (auto& circle : m_CircleList)
    {
        m_Engine.attachRotatingPlatformCircle(circle);
    }
}
void SATRotatingState::onNotify() {
    exitState();
}
void SATRotatingState::setupBoundaries() {
    Color SOIL = Color{61, 61, 61, 255};
    std::vector<Vector2> LeftBoundary;
    LeftBoundary.push_back(Vector2{0, 0});
    LeftBoundary.push_back(Vector2{0, 1000});
    LeftBoundary.push_back(Vector2{10, 1000});
    LeftBoundary.push_back(Vector2{10, 0});
    SATRotatingPlatformPolygon* LeftBoundaryPolygon = new SATRotatingPlatformPolygon(LeftBoundary, SOIL);
    LeftBoundaryPolygon->setFixed(true);
    m_PolygonList.push_back(LeftBoundaryPolygon);
    std::vector<Vector2> RightBoundary;
    RightBoundary.push_back(Vector2{1790, 0});
    RightBoundary.push_back(Vector2{1800, 0});
    RightBoundary.push_back(Vector2{1800, 1000});
    RightBoundary.push_back(Vector2{1790, 1000});
    SATRotatingPlatformPolygon* RightBoundaryPolygon = new SATRotatingPlatformPolygon(RightBoundary, SOIL);
    RightBoundaryPolygon->setFixed(true);
    m_PolygonList.push_back(RightBoundaryPolygon);
    std::vector<Vector2> BottomBoundary;
    BottomBoundary.push_back(Vector2{0, 990});
    BottomBoundary.push_back(Vector2{1800, 990});
    BottomBoundary.push_back(Vector2{1800, 1000});
    BottomBoundary.push_back(Vector2{0, 1000});
    SATRotatingPlatformPolygon* BottomBoundaryPolygon = new SATRotatingPlatformPolygon(BottomBoundary, SOIL);
    BottomBoundaryPolygon->setFixed(true);
    m_PolygonList.push_back(BottomBoundaryPolygon);
}
void SATRotatingState::draw() {
    // std::cout << "State Drawing\n";
    m_Engine.draw();
    DrawText("SAT Fixed And Rotating State", 100, 10, 20, RED);
}
SimulationState* SATRotatingState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    int SubStep = 8;
    for (int i = 0; i < SubStep; ++i) {
        m_Engine.update(m_FrameTime / (float) SubStep);
    }
//    if (IsKeyDown(KEY_LEFT))
//    {
//        m_PolygonList[1] ->accelerate({-300.0f, 0.0f});
////        m_PolygonList[1] ->move({-1.0f, 0.0f});
//    }
//    if (IsKeyDown(KEY_RIGHT))
//    {
////        std::cout << "Right\n";
//        m_PolygonList[1] ->accelerate({300.0f, 0.0f});
////        m_PolygonList[1] ->move({1.0f, 0.0f});
//    }
//    if (IsKeyDown(KEY_UP))
//    {
//        m_PolygonList[1] ->accelerate({0.0f, -300.0f});
////        m_PolygonList[1] ->move({0.0f, -1.0f});
//    }
//    if (IsKeyDown(KEY_DOWN))
//    {
//        m_PolygonList[1] ->accelerate({0.0f, 300.0f});
////        m_PolygonList[1] ->move({0.0f, 1.0f});
//    }
//    if (IsKeyDown(KEY_ONE))
//    {
//        m_PolygonList[1]->rotate(0.05);
//    }
//    if (IsKeyDown(KEY_TWO))
//    {
//        m_PolygonList[1]->rotate(-0.05);
//    }
    return nullptr;
}
SimulationState* SATRotatingState::getSATRotatingState() {
    static SATRotatingState MySATFixedAndRotatingState;
    return &MySATFixedAndRotatingState;
}
MoonLanderState::MoonLanderState() : /*FlameAnimation(LoadTexture("./Assets/Textures/Fire.png"), Vector2{31, 1}, 0.016f),*/ m_Engine(1800, 1000) {
    m_StateNumber = StateNumber::MOON_LANDER_STATE;
    reset();
}
MoonLanderState::~MoonLanderState() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
}
SimulationState* MoonLanderState::getMoonLanderState() {
    static MoonLanderState MyMoonLanderState;
    return &MyMoonLanderState;
}
SimulationState* MoonLanderState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }
    m_LandingCraft.update();
    m_Engine.update(m_FrameTime);
    return nullptr;
}
void MoonLanderState::reset() {
    m_IsActive = true;
    for (auto& polygon : m_PolygonList)
    {
        delete polygon;
        polygon = nullptr;
    }
    m_PolygonList.clear();
    for (auto& circle : m_CircleList)
    {
        delete circle;
        circle = nullptr;
    }
    m_CircleList.clear();
    m_Engine.reset();
    Color Beige{245, 236, 213, 255};
    std::vector<Vector2> Body;
    Body.push_back(Vector2{300, 200});
    Body.push_back(Vector2{400, 200});
    Body.push_back(Vector2{400, 300});
    Body.push_back(Vector2{300, 300});
    SATRotatingPlatformPolygon* NewPolygon = new SATRotatingPlatformPolygon(Body, RED);
    m_LandingCraft.GiveBody(NewPolygon);
    m_PolygonList.push_back(NewPolygon);
    std::vector<Vector2> Platform1;
//    104.031 676.992
//    312.949 743.441
//    338.727 835.547
//    86.0156 748.113
    Platform1.push_back(Vector2{104.031, 676.992});
    Platform1.push_back(Vector2{312.949, 743.441});
    Platform1.push_back(Vector2{338.727, 835.547});
    Platform1.push_back(Vector2{86.0156, 748.113});
    SATRotatingPlatformPolygon* NewPolygon1 = new SATRotatingPlatformPolygon(Platform1, Beige);
    NewPolygon1->setFixed(true);
    m_PolygonList.push_back(NewPolygon1);
//    1333.44 999.586
//    1570.07 867.672
//    1796.91 995.309
    std::vector<Vector2> Platform2;
    Platform2.push_back(Vector2{1333.44, 999.586});
    Platform2.push_back(Vector2{1570.07, 867.672});
    Platform2.push_back(Vector2{1796.91, 995.309});
    SATRotatingPlatformPolygon* NewPolygon2 = new SATRotatingPlatformPolygon(Platform2, Beige);
    NewPolygon2->setFixed(true);
    m_PolygonList.push_back(NewPolygon2);
//    1377.39 301.812
//    1386.64 408.582
//    1642.19 418.914
//    1666.28 304.242
    std::vector<Vector2> Platform3;
    Platform3.push_back(Vector2{1377.39, 301.812});
    Platform3.push_back(Vector2{1386.64, 408.582});
    Platform3.push_back(Vector2{1642.19, 418.914});
    Platform3.push_back(Vector2{1666.28, 304.242});
    SATRotatingPlatformPolygon* NewPolygon3 = new SATRotatingPlatformPolygon(Platform3, Beige);
    NewPolygon3->setFixed(true);
    m_PolygonList.push_back(NewPolygon3);
//    650.102 176.219
//    650.75 197.855
//    1245.1 225.082
//    1245.98 201.574
    std::vector<Vector2> Platform4;
    Platform4.push_back(Vector2{650.102, 176.219});
    Platform4.push_back(Vector2{650.75, 197.855});
    Platform4.push_back(Vector2{1245.1, 225.082});
    Platform4.push_back(Vector2{1245.98, 201.574});
    SATRotatingPlatformPolygon* NewPolygon4 = new SATRotatingPlatformPolygon(Platform4, Beige);
    NewPolygon4->setFixed(true);
    m_PolygonList.push_back(NewPolygon4);
    setupBoundaries();
    for (auto& polygon : m_PolygonList)
    {
        m_Engine.attachRotatingPlatformPolygon(polygon);
    }
}
void MoonLanderState::onNotify() {
    exitState();
}
void MoonLanderState::draw() {
    m_Engine.draw();
    DrawRectangle(0, 0, 1800, 1000, WHITE);
    DrawTextureEx(m_Background, Vector2{0, 0}, 0.0f, 0.45f, WHITE);
    DrawText("Moon Lander State", 100, 10, 20, RED);
    static float Angle = -90.0f;
    m_LandingCraft.draw();
    for (int i = 1; i < m_PolygonList.size(); ++i)
    {
        m_PolygonList[i]->draw();
    }
}
void MoonLanderState::setupBoundaries() {
    Color SOIL = Color{61, 61, 61, 255};
    float BoundaryThickness = 100;
    std::vector<Vector2> LeftBoundary;
    LeftBoundary.push_back(Vector2{0, 0});
    LeftBoundary.push_back(Vector2{0, 1000});
    LeftBoundary.push_back(Vector2{-BoundaryThickness, 1000});
    LeftBoundary.push_back(Vector2{-BoundaryThickness, 0});
    SATRotatingPlatformPolygon* LeftBoundaryPolygon = new SATRotatingPlatformPolygon(LeftBoundary, SOIL);
    LeftBoundaryPolygon->setFixed(true);
    m_PolygonList.push_back(LeftBoundaryPolygon);
    std::vector<Vector2> RightBoundary;
    RightBoundary.push_back(Vector2{1800 + BoundaryThickness, 0});
    RightBoundary.push_back(Vector2{1800, 0});
    RightBoundary.push_back(Vector2{1800, 1000});
    RightBoundary.push_back(Vector2{1800 + BoundaryThickness, 1000});
    SATRotatingPlatformPolygon* RightBoundaryPolygon = new SATRotatingPlatformPolygon(RightBoundary, SOIL);
    RightBoundaryPolygon->setFixed(true);
    m_PolygonList.push_back(RightBoundaryPolygon);
    std::vector<Vector2> BottomBoundary;
    BottomBoundary.push_back(Vector2{0, 1000});
    BottomBoundary.push_back(Vector2{1800, 1000});
    BottomBoundary.push_back(Vector2{1800, 1000 + BoundaryThickness});
    BottomBoundary.push_back(Vector2{0, 1000 + BoundaryThickness});
    SATRotatingPlatformPolygon* BottomBoundaryPolygon = new SATRotatingPlatformPolygon(BottomBoundary, SOIL);
    BottomBoundaryPolygon->setFixed(true);
    m_PolygonList.push_back(BottomBoundaryPolygon);

}
MoonLanderState::LandingCraft::LandingCraft() : LeftRotateNozzle(LoadTexture("./Assets/Textures/Fire.png"), Vector2{31, 1}, 0.016f),
RightRotateNozzle(LoadTexture("./Assets/Textures/Fire.png"), Vector2{31, 1}, 0.016f),
LeftThrustNozzle(LoadTexture("./Assets/Textures/Fire.png"), Vector2{31, 1}, 0.016f),
RightThrustNozzle(LoadTexture("./Assets/Textures/Fire.png"), Vector2{31, 1}, 0.016f) {
    SetSoundVolume(m_Sound, 0.2f);
}
void MoonLanderState::LandingCraft::draw() {
    if (!Body) return;
    float Angle = -(Body->getAngle());
    Vector2 Arrow = {0, -100};
//    Angle = radianToDegree(Angle);
    Angle -= PI/4;
    Arrow = Vector2Rotate(Arrow, Angle);
//    drawArrow(Body->getCenter(), Vector2Add(Body->getCenter(), Arrow), RED);
    Vector2 LeftArrow = Vector2Rotate(Arrow, PI/2);
    Vector2 LeftRotatingPosition = Vector2Add(Body->getCenter(), LeftArrow);
    DrawCircle(LeftRotatingPosition.x, LeftRotatingPosition.y, 5, RED);
    LeftArrow = Vector2Scale(LeftArrow, 0.7);
    Vector2 LeftThrustPosition = Vector2Add(Body->getCenter(), LeftArrow);
    DrawCircle(LeftThrustPosition.x, LeftThrustPosition.y, 5, RED);
    Vector2 RightArrow = Vector2Rotate(Arrow, -PI/2);
    Vector2 RightRotatingPosition = Vector2Add(Body->getCenter(), RightArrow);
    DrawCircle(RightRotatingPosition.x, RightRotatingPosition.y, 5, BLUE);
    RightArrow = Vector2Scale(RightArrow, 0.7);
    Vector2 RightThrustPosition = Vector2Add(Body->getCenter(), RightArrow);
    DrawCircle(RightThrustPosition.x, RightThrustPosition.y, 5, BLUE);
    DrawLineEx(Body->getCenter(), LeftRotatingPosition, 5, BLACK);
    DrawLineEx(Body->getCenter(), RightRotatingPosition, 5, BLACK);
    float LanderScale = 7.2f;
    Vector2 Position = Body->getCenter();
//    Position = Vector2Subtract(Position, {50, 50});
    Rectangle SourceRectLander = Rectangle{0, 0, (float)m_Lander.width, (float)m_Lander.height};
    Rectangle DestRect = Rectangle{Position.x, Position.y, (float)m_Lander.width * LanderScale, (float)m_Lander.height * LanderScale};
    Vector2 Origin = Vector2{(float)DestRect.width/2, (float)DestRect.height /2};
    DrawCircle(Position.x, Position.y, 50, BLACK);
    DrawCircle(Position.x, Position.y, 45, BLUE);

    DrawTexturePro(m_Lander, SourceRectLander, DestRect, Origin, radianToDegree(Angle) - 180, WHITE);

    float AngleOffset = 90;
    if (IsKeyDown(KEY_RIGHT))
    {
        LeftRotateNozzle.draw(LeftRotatingPosition, 2.0f, radianToDegree(Angle) + AngleOffset);
    }
    if (IsKeyDown(KEY_LEFT))
    {
        RightRotateNozzle.draw(RightRotatingPosition, 2.0f, radianToDegree(Angle) + AngleOffset);
    }
    if (IsKeyDown(KEY_UP))
    {
        LeftThrustNozzle.draw(LeftThrustPosition, 2.0f, radianToDegree(Angle) + AngleOffset);
        RightThrustNozzle.draw(RightThrustPosition, 2.0f, radianToDegree(Angle) + AngleOffset);
    }
    float NozzleScale = 1.5f;
    Rectangle SourceRectNozzle = Rectangle{0, 0, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height};
    Rectangle LeftRotateDestRect = Rectangle{LeftRotatingPosition.x, LeftRotatingPosition.y, (float)m_NozzleTexture.width * NozzleScale, (float)m_NozzleTexture.height * NozzleScale};
    Vector2 LeftRotateOrigin = Vector2{(float)m_NozzleTexture.width/NozzleScale, (float)m_NozzleTexture.height / NozzleScale};
    DrawTexturePro(m_NozzleTexture, SourceRectNozzle, LeftRotateDestRect, LeftRotateOrigin, radianToDegree(Angle) + 180, WHITE);
    Rectangle RightRotateDestRect = Rectangle{RightRotatingPosition.x, RightRotatingPosition.y, (float)m_NozzleTexture.width * NozzleScale, (float)m_NozzleTexture.height * NozzleScale};
    Vector2 RightRotateOrigin = Vector2{(float)m_NozzleTexture.width/NozzleScale, (float)m_NozzleTexture.height / NozzleScale};
    DrawTexturePro(m_NozzleTexture, SourceRectNozzle, RightRotateDestRect, RightRotateOrigin, radianToDegree(Angle) + 180, WHITE);
    Rectangle LeftThrustDestRect = Rectangle{LeftThrustPosition.x, LeftThrustPosition.y, (float)m_NozzleTexture.width * NozzleScale, (float)m_NozzleTexture.height * NozzleScale};
    Vector2 LeftThrustOrigin = Vector2{(float)m_NozzleTexture.width/NozzleScale, (float)m_NozzleTexture.height / NozzleScale};
    DrawTexturePro(m_NozzleTexture, SourceRectNozzle, LeftThrustDestRect, LeftThrustOrigin, radianToDegree(Angle) + 180, WHITE);
    Rectangle RightThrustDestRect = Rectangle{RightThrustPosition.x, RightThrustPosition.y, (float)m_NozzleTexture.width * NozzleScale, (float)m_NozzleTexture.height * NozzleScale};
    Vector2 RightThrustOrigin = Vector2{(float)m_NozzleTexture.width/NozzleScale, (float)m_NozzleTexture.height / NozzleScale};
    DrawTexturePro(m_NozzleTexture, SourceRectNozzle, RightThrustDestRect, RightThrustOrigin, radianToDegree(Angle) + 180, WHITE);
//    Rectangle DestRect = Rectangle{LeftRotatingPosition.x, LeftRotatingPosition.y, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height};
//    DrawTexturePro(m_NozzleTexture, Rectangle{0, 0, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height}, Rectangle{LeftRotatingPosition.x, LeftRotatingPosition.y, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height}, Vector2{(float)m_NozzleTexture.width/2, (float)m_NozzleTexture.height/2}, radianToDegree(Angle) + 180, WHITE);
//    DrawTexturePro(m_NozzleTexture, Rectangle{0, 0, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height}, Rectangle{RightRotatingPosition.x, RightRotatingPosition.y, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height}, Vector2{(float)m_NozzleTexture.width/2, (float)m_NozzleTexture.height/2}, radianToDegree(Angle) + 180, WHITE);
//    DrawTexturePro(m_NozzleTexture, Rectangle{0, 0, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height}, Rectangle{LeftThrustPosition.x, LeftThrustPosition.y, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height}, Vector2{(float)m_NozzleTexture.width/2, (float)m_NozzleTexture.height/2}, radianToDegree(Angle) + 180, WHITE);
//    DrawTexturePro(m_NozzleTexture, Rectangle{0, 0, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height}, Rectangle{RightThrustPosition.x, RightThrustPosition.y, (float)m_NozzleTexture.width, (float)m_NozzleTexture.height}, Vector2{(float)m_NozzleTexture.width/2, (float)m_NozzleTexture.height/2}, radianToDegree(Angle) + 180, WHITE);
}
void MoonLanderState::LandingCraft::update() {
    if (!Body) return;
    LeftRotateNozzle.update(GetFrameTime());
    RightRotateNozzle.update(GetFrameTime());
    LeftThrustNozzle.update(GetFrameTime());
    RightThrustNozzle.update(GetFrameTime());
    float Angle = -(Body->getAngle());
    Vector2 Arrow = {0, -100};
    Angle -= PI/4;
    Arrow = Vector2Rotate(Arrow, Angle);
    static bool Playing = false;
    bool Nozzle = false;
    if (IsKeyDown(KEY_LEFT))
    {
//        Body->rotate(-0.05);
        Body->accelerateRotation(-0.05);
        Nozzle = true;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
//        Body->rotate(0.05);
        Body->accelerateRotation(+0.05);
        Nozzle = true;
    }
    if (IsKeyDown(KEY_UP))
    {
//        Body->move(Vector2{0, -4});
//        std::cout << "Up\n";
        Arrow = Vector2Normalize(Arrow);
        Arrow = Vector2Scale(Arrow, -300);
        if (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_RIGHT))
        {
//            std::cout << "Both\n";
            Arrow = Vector2Scale(Arrow, 1.5);
            Body->accelerate({Arrow});
        }
        Body->accelerate({Arrow});
        Nozzle = true;
    }
    if (IsKeyDown(KEY_DOWN))
    {
//        Body->move(Vector2{0, 4});
        Arrow = Vector2Normalize(Arrow);
        Arrow = Vector2Scale(Arrow, 300);
        Body->accelerate(Arrow);
        Nozzle = true;
    }
    if (IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_RIGHT))
    {
//        std::cout << "Both\n";
        Arrow = Vector2Normalize(Arrow);
        Arrow = Vector2Scale(Arrow, -300);
        Body->accelerate({Arrow});
        Nozzle = true;
    }
    if (!Nozzle)
    {
        StopSound(m_Sound);
        Playing = false;
    }
    if (Nozzle && !Playing)
    {
        PlaySound(m_Sound);
        Playing = true;
    }

//    Vector2 Acceleration = Body->getTotalAcceleration();
//    std::cout << "Total Acceleration: " << Acceleration.x << " " << Acceleration.y << std::endl;
}
void MoonLanderState::LandingCraft::GiveBody(SATRotatingPlatformPolygon *Body) {
    this->Body = Body;
}

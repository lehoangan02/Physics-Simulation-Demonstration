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
    ifstream fin("PressureSoftBodyCordinate.txt");
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
SATTriangleState::SATTriangleState() {
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
    readCordinates();
}
void SATTriangleState::onNotify() {
    exitState();
}
void SATTriangleState::draw() {
    DrawText("SAT Triangle State", 100, 10, 20, RED);
    for (auto& triangle : m_TriangleList)
    {
        triangle -> draw();
    }
    SATCollider* Collider = TriangleSATCollider::getTriangleSATCollider();
    TriangleSATCollider* TriangleCollider = dynamic_cast<TriangleSATCollider*>(Collider);
    TriangleCollider->drawProjectionY(m_TriangleList[0]->getVertices(), m_TriangleList[1]->getVertices());
}
SimulationState* SATTriangleState::update() {
    if (!m_IsActive) {
        return HomeState::getHomeState();
    }

    return nullptr;
}
void SATTriangleState::readCordinates() {
    ifstream fin;
    fin.open("TriangleCordinates.txt");
    if (!fin.is_open())
    {
        cerr << "Error opening file\n";
        return;
    }
    Vector2 Temp[3];
    for (int i = 0; i < 3; ++i)
    {
        float x, y;
        fin >> x >> y;
        Temp[i] = Vector2{x, y};
    }
    SATPlatformTriangle* NewTriangle = new SATPlatformTriangle(Temp[0], Temp[1], Temp[2], BLACK);
    for (int i = 0; i < 3; ++i)
    {
        fin >> Temp[i].x >> Temp[i].y;
    }
    SATPlatformTriangle* NewTriangle2 = new SATPlatformTriangle(Temp[0], Temp[1], Temp[2], BLACK);
    m_TriangleList.push_back(NewTriangle);
    m_TriangleList.push_back(NewTriangle2);
}
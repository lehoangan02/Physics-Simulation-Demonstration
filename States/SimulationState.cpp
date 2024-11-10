//
// Created by Le Hoang An on 17/10/24.
//
#include "SimulationState.hpp"
#define MY_ORANGE Color(235, 131, 23, 255)
#define MY_YELLOW Color(243, 198, 35, 255)
void SimulationState::exitState() {
    m_IsActive = false;
}
SimulationState* VerletDropState::getVerletDropState()
{
    static VerletDropState MyVerletDropState;
    return &MyVerletDropState;
}
VerletDropState::VerletDropState() : m_Engine(1800, 1040) {
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
//    printf("Reset\n");

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
    if (IsKeyPressed(KEY_A))
    {
        BackHomeButton::getBackHomeButton()->m_Active = true;
        return VerletDropState::getVerletDropState();
    }
    else if (IsKeyPressed(KEY_B))
    {
        BackHomeButton::getBackHomeButton()->m_Active = true;
        return VerletChainState::getVerletChainState();
    }
    else if (IsKeyPressed(KEY_C))
    {
        BackHomeButton::getBackHomeButton()->m_Active = true;
        return VerletChainBasketState::getVerletChainBasketState();
    }
    else if (IsKeyPressed(KEY_D))
    {
        BackHomeButton::getBackHomeButton()->m_Active = true;
        return EulerianDropState::getEulerianDropState();
    }
    else if (IsKeyPressed(KEY_E))
    {
        BackHomeButton::getBackHomeButton()->m_Active = true;
        return TunnellingComparisonState::getComparisonState();
    }

    BackHomeButton::getBackHomeButton()->m_Active = false;
    return nullptr;
}
void HomeState::onNotify() {
    return;
}
void HomeState::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);
    DrawText("Press A to start the simulation", 100, 10, 20, WHITE);
//    DrawText("VERLET_STATE", 10, 80, 20, BLACK);
}
void HomeState::reset() {
    return;
}
SimulationState* VerletChainState::getVerletChainState()
{
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
VerletChainState::VerletChainState() : m_Engine(1800, 1040) {
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
VerletChainBasketState::VerletChainBasketState() : m_Engine(1800, 1040) {
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
EulerianDropState::EulerianDropState() : m_Engine(1800, 1040) {
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
    RoundBall1 -> m_Velocity = Vector2{50, 0};
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
    static TunnellingComparisonState MyComparisonState;
    return &MyComparisonState;
}
TunnellingComparisonState::TunnellingComparisonState() : m_EulerianEngine(1800 / 2 - 10, 1040), m_VerletEngine(1800 / 2 - 10, 1040) {
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
//    EulerianRoundBall* EulerianRoundBall2 = new EulerianRoundBall(Vector2{200, 200}, GREEN, 1.0f);
    m_EulerianRoundBallList.push_back(EulerianRoundBall1);
//    m_EulerianRoundBallList.push_back(EulerianRoundBall2);
    m_EulerianEngine.attachRoundBall(EulerianRoundBall1);
//    m_EulerianEngine.attachRoundBall(EulerianRoundBall2);
    VerletRoundBall::m_Radius = 100.0f;
    VerletRoundBall* VerletRoundBall1 = new VerletRoundBall(Vector2{100, 100}, MY_ORANGE);
    VerletRoundBall1 -> giveSpeed(Vector2{5, -5});
    VerletRoundBall1 -> m_SpeedScale = 1.2f;
//    VerletRoundBall* VerletRoundBall2 = new VerletRoundBall(Vector2{400, 400}, GREEN);
//    VerletRoundBall2 -> giveSpeed(Vector2{5, 10});
    m_VerletRoundBallList.push_back(VerletRoundBall1);
//    m_VerletRoundBallList.push_back(VerletRoundBall2);
    m_VerletEngine.attachRoundBall(VerletRoundBall1);
//    m_VerletEngine.attachRoundBall(VerletRoundBall2);

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
//    RenderTexture screenCamera1 = LoadRenderTexture(1800/2, 1040);
//    RenderTexture screenCamera2 = LoadRenderTexture(1800/2, 1040);
//    Rectangle splitScreenRect = { 0.0f, 0.0f, (float)screenCamera1.texture.width, (float)-screenCamera1.texture.height };

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
    DrawRectangle(1800 / 2 - 10, 0, 20, 1040, MidBarColor);
}
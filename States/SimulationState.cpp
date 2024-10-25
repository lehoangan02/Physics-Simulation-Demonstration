//
// Created by Le Hoang An on 17/10/24.
//
#include "SimulationState.hpp"
#define START_X 500
#define START_Y 300
#define START_SPEED_X 20
#define START_SPEED_Y -15
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
    RoundBall* RoundBall1 = new RoundBall(Vector2(START_X, START_Y), Color(16, 55, 92, 255));
    RoundBall1 -> giveSpeed(Vector2{START_SPEED_X, START_SPEED_Y});
    m_RoundBall.push_back(RoundBall1);
    m_Engine.attachRoundBall(RoundBall1);
    PlatformTriangle* PlatformTriangle1 = new PlatformTriangle(Vector2{300, 400}, Vector2{300, 500}, Vector2{500, 500}, Color(243, 198, 35, 255));
    PlatformTriangle* PlatformTriangle2 = new PlatformTriangle( Vector2{600, 700}, Vector2{800, 700}, Vector2{850, 500}, Color(235, 131, 23, 255));
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
    if (m_IsActive == false || IsKeyPressed(KEY_BACKSPACE))
    {
        return HomeState::getHomeState();
    }
    m_TotalTime += GetFrameTime();
    if (m_TotalTime > m_DropInterval && m_RoundBall.size() < m_NumBall)
    {
//        printf("Create new ball\n");
        RoundBall* RoundBall2 = new RoundBall(Vector2(START_X, START_Y), Color(16, 55, 92, 255));
        RoundBall2 -> giveSpeed(Vector2{START_SPEED_X, START_SPEED_Y});
        m_RoundBall.push_back(RoundBall2);
        m_Engine.attachRoundBall(RoundBall2);
        m_TotalTime -= m_DropInterval;
    }
    m_Engine.update(0.05f);
    return nullptr;
}
void VerletDropState::draw() {
    BeginDrawing();
    ClearBackground(Color(244, 246, 255, 255));
    m_Engine.draw();
}
void VerletDropState::onNotify() {
    exitState();
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
    BackHomeButton::getBackHomeButton()->m_Active = false;
    return nullptr;
}
void HomeState::onNotify() {
    return;
}
void HomeState::draw() {
    DrawTexture(m_Background, 0, 0, WHITE);
    DrawText("Press A to start the simulation", 10, 10, 20, BLACK);
//    DrawText("VERLET_STATE", 10, 80, 20, BLACK);
}
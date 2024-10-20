//
// Created by Le Hoang An on 17/10/24.
//
#include "SimulationState.hpp"
VerletDropState::VerletDropState() : m_Engine(1800, 1040), m_RoundBall(Vector2(400, 400)) {
    m_RoundBall = RoundBall( Vector2{400, 100});
    m_Engine.attachRoundBall(&m_RoundBall);
    PlatformTriangle* PlatformTriangle1 = new PlatformTriangle(Vector2{300, 400}, Vector2{300, 500}, Vector2{500, 500}, GREEN);
    m_PlatformTriangleList.push_back(PlatformTriangle1);
    m_Engine.attachPlatformTriangle(PlatformTriangle1);
}
VerletDropState::~VerletDropState() {
    for (auto& platform : m_PlatformTriangleList)
    {
        delete platform;
    }
}
void VerletDropState::update() {
    m_Engine.update(0.1f);
}
void VerletDropState::draw() {
    m_Engine.draw();

}

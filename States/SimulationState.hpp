//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
#include "../Engine.hpp"
#include "../RoundBall.hpp"
#include "../PlatformTriangle.hpp"
#include <vector>
class SimulationState {
public:
    SimulationState() = default;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual ~SimulationState() = default;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
class VerletDropState : public SimulationState {
private:
    Engine m_Engine;
    vector<RoundBall*> m_RoundBall;
    vector<PlatformTriangle*> m_PlatformTriangleList;
    float m_TotalTime = 0.0f;
    const int m_NumBall = 200;
    const float m_DropInterval = 0.1f;
public:
    VerletDropState();
    ~VerletDropState();
    void update() override;
    void draw() override;
};
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
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
class VerletDropState : public SimulationState {
private:
    Engine m_Engine;
    RoundBall m_RoundBall;
    vector<PlatformTriangle*> m_PlatformTriangleList;
public:
    VerletDropState();
    ~VerletDropState();
    void update() override;
    void draw() override;
};
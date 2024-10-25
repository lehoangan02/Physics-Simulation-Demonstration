//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
#include "../Physics/Engine.hpp"
#include "../Physics/RoundBall.hpp"
#include "../Physics/PlatformTriangle.hpp"
#include <vector>
#include "Observer.hpp"
#include "../Buttons/Buttons.hpp"
enum StateNumber {
    HOME_STATE,
    VERLET_DROP_STATE
};
class SimulationState: public Observer{
    friend class BackHome;
public:
    SimulationState() = default;
    virtual SimulationState* update() = 0;
    virtual void draw() = 0;
    virtual ~SimulationState() = default;
protected:
    bool m_IsActive = true;
    int m_ExitState = StateNumber::HOME_STATE;
    void exitState();
//    virtual void reset() = 0;
};
class HomeState : public SimulationState {
private:
    Texture2D m_Background;
public:
    static SimulationState* getHomeState();
private:
    HomeState();
    ~HomeState() = default;
public:
    SimulationState* update() override;
    void draw() override;
private:
    void onNotify() override;
};
class VerletDropState : public SimulationState {
public:
    static SimulationState* getVerletDropState();
private:
    Engine m_Engine;
    vector<RoundBall*> m_RoundBall;
    vector<PlatformTriangle*> m_PlatformTriangleList;
    float m_TotalTime = 0.0f;
    const int m_NumBall = 200;
    const float m_DropInterval = 0.1f;
private:
    VerletDropState();
    ~VerletDropState();

public:
    SimulationState* update() override;
    void draw() override;
private:
    void onNotify() override;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
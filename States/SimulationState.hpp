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
#include "../Physics/Chains.hpp"
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
    virtual void reset() = 0;
protected:
    bool m_IsActive = true;
    int m_ExitState = StateNumber::HOME_STATE;
    void exitState();
    const float m_FrameTime = 0.01666666667f;
};
class HomeState : public SimulationState {
private:
    Texture2D m_Background;
public:
    static SimulationState* getHomeState();
    void reset() override;
    SimulationState* update() override;
    void draw() override;
private:
    HomeState();
    ~HomeState() = default;
    void onNotify() override;
};
class VerletDropState : public SimulationState {
public:
    static SimulationState* getVerletDropState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    VerletEngine m_Engine;
    vector<VerletRoundBall*> m_RoundBall;
    vector<PlatformTriangle*> m_PlatformTriangleList;
    float m_TotalTime = 0.0f;
    const int m_NumBall = 200;
    const float m_DropInterval = 0.1f;
    const float StartSppedX = 20;
    const float StartSppedY = 15;
    const float StartX = 500;
    const float StartY = 300;
private:
    VerletDropState();
    ~VerletDropState();
    void reset() override;
};
class VerletChainState : public SimulationState {
public:
    static SimulationState* getVerletChainState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    VerletEngine m_Engine;
    vector<VerletRoundBall*> m_RoundBall;
    VerletChain* m_Chain;
private:
    ~VerletChainState();
    VerletChainState();
    void reset() override;
};
class VerletChainBasketState : public SimulationState {
public:
    static SimulationState* getVerletChainBasketState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    float m_TotalTime = 0.0f;
    const float m_DropInterval = 0.3;
    const int m_NumBall = 200;
    VerletEngine m_Engine;
    vector<VerletRoundBall*> m_RoundBall;
    VerletChain* m_Chain;
    vector<VerletRoundBall*> m_Basket;
    const float StartX = 900;
    const float StartY = 400;
private:
    ~VerletChainBasketState();
    VerletChainBasketState();
    void reset() override;
};
class EulerianDropState : public SimulationState {
public:
    static SimulationState* getEulerianDropState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    ContinuousEulerianEngine m_Engine;
    vector<EulerianRoundBall*> m_RoundBallList;
    vector<PlatformTriangle*> m_PlatformTriangleList;
private:
    EulerianDropState();
    ~EulerianDropState();
    void reset() override;
//    float m_TotalTime = 0.0f;
//    const int m_NumBall = 200;
//    const float m_DropInterval = 0.1f;
//    const float StartSppedX = 20;
//    const float StartSppedY = 15;
//    const float StartX = 500;
//    const float StartY = 300;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
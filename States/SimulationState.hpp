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
};
class TunnellingComparisonState : public SimulationState {
public:
    static SimulationState* getComparisonState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    ContinuousEulerianEngine m_EulerianEngine;
    VerletEngine m_VerletEngine;
    vector<EulerianRoundBall*> m_EulerianRoundBallList;
    vector<VerletRoundBall*> m_VerletRoundBallList;
    Camera2D m_EulerianCamera = {0};
    Camera2D m_VerletCamera = {0};
private:
    RenderTexture screenCamera1 = LoadRenderTexture(1800/2, 1040);
    RenderTexture screenCamera2 = LoadRenderTexture(1800/2, 1040);
    Rectangle splitScreenRect = { 0.0f, 0.0f, (float)screenCamera1.texture.width, (float)-screenCamera1.texture.height };
private:
    TunnellingComparisonState();
    ~TunnellingComparisonState();
    void reset() override;
};
class EnergyComparisonState : public SimulationState {
public:
    static SimulationState* getEnergyComparisonState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
private:
    ContinuousEulerianEngine m_EulerianEngine;
    VerletEngine m_VerletEngine;
    vector<EulerianRoundBall*> m_EulerianRoundBallList;
    vector<VerletRoundBall*> m_VerletRoundBallList;
    Camera2D m_EulerianCamera = {0};
    Camera2D m_VerletCamera = {0};
private:
    RenderTexture screenCamera1 = LoadRenderTexture(1800/2, 1040);
    RenderTexture screenCamera2 = LoadRenderTexture(1800/2, 1040);
    Rectangle splitScreenRect = { 0.0f, 0.0f, (float)screenCamera1.texture.width, (float)-screenCamera1.texture.height };
private:
    EnergyComparisonState();
    ~EnergyComparisonState();
    void reset() override;
};
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
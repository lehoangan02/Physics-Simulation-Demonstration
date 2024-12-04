//
// Created by Le Hoang An on 17/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
#include "../Physics/Engine.hpp"
#include "../Physics/RoundBall.hpp"
#include "../Physics/PlatformTriangle.hpp"
#include "../Physics/PlatformRectangle.h"
#include <vector>
#include "Observer.hpp"
#include "../Buttons/Buttons.hpp"
#include "../Physics/Chains.hpp"
#include "../Physics/Spring.hpp"
#include "../Machine Learning/KMeansCalculator.h"
enum StateNumber {
    HOME_STATE,
    VERLET_DROP_STATE,
    VERLET_CHAIN_STATE,
    VERLET_CHAIN_BASKET_STATE,
    EULERIAN_DROP_STATE,
    TUNNELLING_COMPARISON_STATE,
    ENERGY_COMPARISON_STATE,
    FPS_INVARIANT_STATE_FOR_CONTINUOUS_INTEGRATION,
    PARTICLE_GRAVITY_STATE,
    OPTIMIZATION_1_STATE,
    SPRING_STATE,
    SPRING_SOFT_BODY_STATE,
    PLAYABLE_SPRING_SOFT_BODY_STATE,
    KMEANS_GROUPING_STATE,
    KMEANS_OPTIMIZED_STATE
};
class SimulationState;
class StateFactory
{
public:
    static SimulationState* getState(int StateNumber);
    static StateFactory* getStateFactory();
private:
    StateFactory() = default;
    ~StateFactory() = default;
};
class SimulationState: public Observer{
    friend class BackHome;
public:
    SimulationState() = default;
    virtual SimulationState* update() = 0;
    virtual void draw() = 0;
    virtual ~SimulationState() = default;
    virtual void reset() = 0;
    int m_StateNumber = 0;
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
    ~VerletDropState() override;
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
    ~VerletChainState() override;
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
    ~VerletChainBasketState() override;
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
    ~EulerianDropState() override;
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
    RenderTexture screenCamera1 = LoadRenderTexture(1800/2, 1000);
    RenderTexture screenCamera2 = LoadRenderTexture(1800/2, 1000);
    Rectangle splitScreenRect = { 0.0f, 0.0f, (float)screenCamera1.texture.width, (float)-screenCamera1.texture.height };
private:
    TunnellingComparisonState();
    ~TunnellingComparisonState() override;
    void reset() override;
};
class EnergyComparisonState : public SimulationState {
public:
    static SimulationState* getEnergyComparisonState();
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
    RenderTexture screenCamera1 = LoadRenderTexture(1800/2, 1000);
    RenderTexture screenCamera2 = LoadRenderTexture(1800/2, 1000);
    Rectangle splitScreenRect = { 0.0f, 0.0f, (float)screenCamera1.texture.width, (float)-screenCamera1.texture.height };
private:
    EnergyComparisonState();
    ~EnergyComparisonState() override;
    void reset() override;
};
class FPSInvariantStateForContinuousIntegration : public SimulationState {
public:
    static SimulationState* getFPSInvariantStateForContinuousIntegration();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    ContinuousEulerianEngine m_Engine1;
    ContinuousEulerianEngine m_Engine2;
    vector<EulerianRoundBall*> m_RoundBallList1;
    vector<EulerianRoundBall*> m_RoundBallList2;
    Camera2D m_Camera1 = {0};
    Camera2D m_Camera2 = {0};
    RenderTexture screenCamera1 = LoadRenderTexture(1800/2, 1000);
    RenderTexture screenCamera2 = LoadRenderTexture(1800/2, 1000);
    Rectangle splitScreenRect = { 0.0f, 0.0f, (float)screenCamera1.texture.width, (float)-screenCamera1.texture.height };
    int m_FrameCount = 0;
private:
    FPSInvariantStateForContinuousIntegration();
    ~FPSInvariantStateForContinuousIntegration() override;
    void reset() override;
};
class ParticleGravityState : public SimulationState {
public:
    static SimulationState* getParticleGravityState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    DiscreteEulerianEngine m_Engine;
    vector<EulerianRoundBall*> m_RoundBallList;
private:
    ParticleGravityState();
    ~ParticleGravityState() override;
    void reset() override;
};
class Optimization1State : public SimulationState {
public:
    static SimulationState* getOptimization1State();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    UniformGridEngine m_Engine;
    vector<EulerianRoundBall*> m_RoundBallList;
private:
    Optimization1State();
    ~Optimization1State() override;
    void reset() override;
};
class SpringState : public SimulationState {
public:
    static SimulationState* getSpringState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    DiscreteEulerianEngine m_Engine;
    vector<EulerianRoundBall*> m_RoundBallList;
    vector<Spring*> m_SpringList;
    vector<PlatformRectangle*> m_PlatformRectangleList;
private:
    SpringState();
    ~SpringState() override;
    void reset() override;
};
class SpringSoftBodyState : public SimulationState {
public:
    static SimulationState* getSpringSoftBodyState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
protected:
    DiscreteEulerianEngine m_Engine;
    vector<vector<ClickableEulerianRoundBall*>> m_RoundBallMatrix;
    vector<Spring*> m_SpringList;
    vector<PlatformRectangle*> m_PlatformRectangleList;
    float m_PushRectangleTime = 14.0f;
    float m_TotalTime = 0.0f;
    Color m_BallColor = Color(66, 62, 117, 255);
    Color m_SpringColor = Color(66, 62, 117, 255);
protected:
    SpringSoftBodyState();
    ~SpringSoftBodyState() override;
    void reset() override;
};
class PlayableSpringSoftBodyState : public SpringSoftBodyState {
public:
    static SimulationState* getPlayableSpringSoftBodyState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    PlayableSpringSoftBodyState();
    ~PlayableSpringSoftBodyState() override;
    void reset() override;
private:
    EulerianRoundBall* m_SelectedBall = nullptr;
    Color m_SelectedColor = Color(255, 128, 0, 255);
    float m_Force = 300000.f;
};
class KmeansGroupingState : public SimulationState {
public:
    static SimulationState* getKmeansGroupingState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    DiscreteEulerianEngine m_Engine;
    vector<EulerianRoundBall*> m_RoundBallList;
    vector<Vector2> m_Centroids;
private:
    KmeansGroupingState();
    ~KmeansGroupingState() override;
    void reset() override;
};
class KMeansOptimizedState : public SimulationState {
public:
    static SimulationState* getKMeansOptimizedState();
    SimulationState* update() override;
    void draw() override;
    void onNotify() override;
private:
    KMeansEngine m_Engine;
    vector<EulerianRoundBall*> m_RoundBallList;
private:
    KMeansOptimizedState();
    ~KMeansOptimizedState() override;
    void reset() override;
};

#endif //PHYSICS_SIMULATION_DEMONSTRATION_SIMULATIONSTATE_HPP
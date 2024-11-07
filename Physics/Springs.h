//
// Created by Le Hoang An on 27/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_SPRINGS_H
#define PHYSICS_SIMULATION_DEMONSTRATION_SPRINGS_H
#include "RoundBall.hpp"
#include "../Math/Geometry.hpp"
class Spring
{
private:
    float m_Length;
    float m_k;
    EulerianRoundBall* m_Ball1;
    EulerianRoundBall* m_Ball2;
public:
    Spring(EulerianRoundBall* Ball1, EulerianRoundBall* Ball2, float Length, float k);
    void update();
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_SPRINGS_H

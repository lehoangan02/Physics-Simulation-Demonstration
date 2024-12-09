//
// Created by Le Hoang An on 25/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_CHAINS_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_CHAINS_HPP
#include "RoundBall.hpp"
#include "raylib.h"
#include <vector>

class VerletChain {
    class Link
    {
    private:
        float& m_Length;
        VerletRoundBall* m_Ball1;
        VerletRoundBall* m_Ball2;
    public:
        Link(VerletRoundBall* Ball1, VerletRoundBall* Ball2, float& Length) : m_Ball1(Ball1), m_Ball2(Ball2), m_Length(Length) {};
        void update();
    };
private:
    std::vector<Link> m_Links;
    float m_Length;
public:
    VerletChain(std::vector<VerletRoundBall*> BallList, float Length);
    void update();
    virtual ~VerletChain() = default;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_CHAINS_HPP

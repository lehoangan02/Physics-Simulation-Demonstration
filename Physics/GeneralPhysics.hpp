//
// Created by lehoangan on 22/12/2024.
//

#ifndef GENERALPHYSICS_HPP
#define GENERALPHYSICS_HPP

#endif //GENERALPHYSICS_HPP
#include "raylib.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
Vector2 calculateTangentalVelocity(const Vector2& Center, const Vector2& Point, const float& AngularVelocity);
float calculateImpulse(const float& Mass1, const float& Mass2, const Vector2& Velocity1, const Vector2& Velocity2,
    const float& CoefficientOfRestitution, Vector2 Normal, const float& Inertia1, const float& Inertia2,
    const Vector2& Tangental1, const Vector2& Tangental2);
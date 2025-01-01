//
// Created by lehoangan on 22/12/2024.
//

#include "GeneralPhysics.hpp"
#include "raymath.h"
Vector2 calculateTangentalVelocity(const Vector2 &Center, const Vector2 &Point, const float &AngularVelocity) {
    Vector2 Radial = Vector2Subtract(Point, Center);
    Vector2 Tangental = Vector2{-Radial.y, Radial.x};
    Vector2 Result = Vector2Scale(Vector2Normalize(Tangental), AngularVelocity * Vector2Length(Radial));
    return Result;
}
float calculateImpulse(const float &Mass1, const float &Mass2, const Vector2 &Velocity1, const Vector2 &Velocity2,
                       const float &CoefficientOfRestitution, Vector2 Normal, const float &Inertia1, const float &Inertia2,
                       const Vector2 &Tangental1, const Vector2 &Tangental2) {
    Vector2 RelativeVelocity = Vector2Subtract(Velocity2, Velocity1);
    Normal = Vector2Normalize(Normal);
    float j;
    j = -(1 + CoefficientOfRestitution) * Vector2DotProduct(RelativeVelocity, Normal);
    std::cout << "Inertia 1: " << Inertia1 << std::endl;
    std::cout << "Inertia 2: " << Inertia2 << std::endl;
    std::cout << "Numerator: " << j << std::endl;
    float Denominator = (1/Mass1 + 1/Mass2) + (Vector2DotProduct(Tangental1, Normal) * Vector2DotProduct(Tangental1, Normal) / Inertia1) + (Vector2DotProduct(Tangental2, Normal) * Vector2DotProduct(Tangental2, Normal) / Inertia2);
    std::cout << "Second term: " << (Vector2DotProduct(Tangental1, Normal) * Vector2DotProduct(Tangental1, Normal) / Inertia1) << std::endl;
    std::cout << "Third term: " << (Vector2DotProduct(Tangental2, Normal) * Vector2DotProduct(Tangental2, Normal) / Inertia2) << std::endl;
    std::cout << "Denominator: " << Denominator << std::endl;
    j /= Denominator;
    return j;
}
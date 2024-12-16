//
// Created by Le Hoang An on 15/12/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H
#define PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H

#include <vector>
#include "raylib.h"
#include "SATPlatform.hpp"
struct CollisionResolve
{
    Vector2 FirstResolution;
    Vector2 SecondResolution;
};
class SATCollider {
public:
    virtual bool isColliding(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) = 0;
    virtual CollisionResolve getCollisionResolution(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) = 0;
};
class TriangleSATCollider : public SATCollider {
private:
    TriangleSATCollider() = default;
    ~TriangleSATCollider() = default;
public:
    static SATCollider* getTriangleSATCollider();
    bool isColliding(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) override;
    void drawProjectionY(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2);
    CollisionResolve getCollisionResolution(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) override;
};
class SATPolygonCollider : public SATCollider {
private:
    SATPolygonCollider() = default;
    ~SATPolygonCollider() = default;
public:
    static SATCollider* getSATPolygonCollider();
    bool isColliding(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) override;
    CollisionResolve getCollisionResolution(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) override;
};
class SATCirclePolygonCollider{
private:
    SATCirclePolygonCollider() = default;
    ~SATCirclePolygonCollider() = default;
public:
    static SATCirclePolygonCollider* getSATCirclePolygonCollider();
    bool isColliding(const SATPlatformCircle& Circle, const std::vector<Vector2>& Shape);
    CollisionResolve getCollisionResolution(const SATPlatformCircle& Circle, const std::vector<Vector2>& Shape);
};

#endif //PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H

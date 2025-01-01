//
// Created by Le Hoang An on 15/12/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H
#define PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H

#include <vector>
#include "raylib.h"
#include "SATPlatform.hpp"
#include <climits>
#include "GeneralPhysics.hpp"
struct CollisionResolve
{
    Vector2 FirstResolution;
    Vector2 SecondResolution;
};
struct AngularCollisionResolve {
    Vector2 FirstPositionResolution;
    Vector2 SecondPositionResolution;
    Vector2 FirstVelocityResolution;
    Vector2 SecondVelocityResolution;
    float FirstAngularResolution;
    float SecondAngularResolution;
};
class SATCollider {
public:
    virtual bool isColliding(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) = 0;
    virtual CollisionResolve getCollisionResolution(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) = 0;
    virtual std::vector<Vector2> getPointsOfIntersection(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) { return {}; }
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
    std::vector<Vector2> getPointsOfIntersection(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2);
    Vector2 getNormalDirection(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2);
    bool IsAVertexOf(const Vector2& Point, const std::vector<Vector2>& Shape);
    CollisionResolve getCollisionResolution(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) override;
    CollisionResolve getCollisionResolution(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2, bool IsShape1Fixed, bool IsShape2Fixed);
};
class SATCirclePolygonCollider{
private:
    SATCirclePolygonCollider() = default;
    ~SATCirclePolygonCollider() = default;
public:
    static SATCirclePolygonCollider* getSATCirclePolygonCollider();
    bool isColliding(const SATPlatformCircle& Circle, const std::vector<Vector2>& Shape);
    CollisionResolve getCollisionResolution(const SATPlatformCircle& Circle, std::vector<Vector2> Shape);
    std::vector<Vector2> getPointsOfIntersection(const SATPlatformCircle& Circle, const std::vector<Vector2>& Shape);
    CollisionResolve getCollisionResolution(const SATPlatformCircle& Circle, std::vector<Vector2> Shape, bool IsCircleFixed, bool IsShapeFixed);
};
class SATRotatingCollider {
private:
    SATRotatingCollider() = default;
    ~SATRotatingCollider() = default;
public:
    static SATRotatingCollider* getSATRotatingCollider();
    AngularCollisionResolve getCollisionResolution(SATRotatingPlatformPolygon* Shape1, SATRotatingPlatformPolygon* Shape2);
};

#endif //PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H

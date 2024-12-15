//
// Created by Le Hoang An on 15/12/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H
#define PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H

#include <vector>
#include "raylib.h"
class SATCollider {
public:
    virtual bool isColliding(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) = 0;
};

class TriangleSATCollider : public SATCollider {
private:
    TriangleSATCollider() = default;
    ~TriangleSATCollider() = default;
public:
    static SATCollider* getTriangleSATCollider();
    bool isColliding(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2) override;
    void drawProjectionY(const std::vector<Vector2>& Shape1, const std::vector<Vector2>& Shape2);
};


#endif //PHYSICS_SIMULATION_DEMONSTRATION_COLLIDER_H

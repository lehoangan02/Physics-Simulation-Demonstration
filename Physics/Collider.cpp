//
// Created by Le Hoang An on 15/12/24.
//

#include "Collider.h"
#include "../Math/Geometry.hpp"
SATCollider* TriangleSATCollider::getTriangleSATCollider() {
    static TriangleSATCollider m_TriangleSATCollider;
    return &m_TriangleSATCollider;
}
void TriangleSATCollider::drawProjectionY(const std::vector<Vector2> &Shape1, const std::vector<Vector2> &Shape2) {
//    std::cout << "Drawing Projection" << std::endl;
    Vector2 Origin = {100, 0};
    Vector2 Direction = {0, -1};
    std::pair<Vector2, Vector2> DirectionVectorAndPoint(Direction, Origin);
    Line Axis(DirectionVectorAndPoint);
    Axis.draw(RED);
    // find the point with largest Y coordinate
    int MaxYIndex1 = 0;
    for (int i = 1; i < Shape1.size(); ++i) {
        if (Shape1[i].y > Shape1[MaxYIndex1].y) {
            MaxYIndex1 = i;
        }
    }
    int MaxYIndex2 = 0;
    for (int i = 1; i < Shape2.size(); ++i) {
        if (Shape2[i].y > Shape2[MaxYIndex2].y) {
            MaxYIndex2 = i;
        }
    }
    Vector2 MaxYPoint1 = Shape1[MaxYIndex1];
    Vector2 MaxYPoint2 = Shape2[MaxYIndex2];
    // find the point with smallest Y coordinate
    int MinYIndex1 = 0;
    for (int i = 1; i < Shape1.size(); ++i) {
        if (Shape1[i].y < Shape1[MinYIndex1].y) {
            MinYIndex1 = i;
        }
    }
    int MinYIndex2 = 0;
    for (int i = 1; i < Shape2.size(); ++i) {
        if (Shape2[i].y < Shape2[MinYIndex2].y) {
            MinYIndex2 = i;
        }
    }
    Vector2 MinYPoint1 = Shape1[MinYIndex1];
    Vector2 MinYPoint2 = Shape2[MinYIndex2];

    Vector2 ProjectionMax1 = Axis.projection(MaxYPoint1);
    Vector2 ProjectionMax2 = Axis.projection(MaxYPoint2);

    Vector2 ProjectionMin1 = Axis.projection(MinYPoint1);
    Vector2 ProjectionMin2 = Axis.projection(MinYPoint2);


    DrawCircle(ProjectionMax1.x, ProjectionMax1.y, 10, RED);
    DrawCircle(Shape1[MaxYIndex1].x, Shape1[MaxYIndex1].y, 10, RED);
    DrawLineEx(ProjectionMax1, ProjectionMin1, 10, RED);
    DrawLineEx(ProjectionMax2, ProjectionMin2, 10, RED);


}
bool TriangleSATCollider::isColliding(const std::vector<Vector2> &Shape1, const std::vector<Vector2> &Shape2) {
    std::vector<Vector2> AxisDirectionList1;
    std::vector<Vector2> AxisDirectionList2;
    for (int i = 0; i < Shape1.size(); ++i) {
        Line Line1(Shape1[i], Shape1[(i + 1) % Shape1.size()]);
        AxisDirectionList1.push_back(Line1.getNormalDirection());
    }
    for (int i = 0; i < Shape2.size(); ++i) {
        Line Line2(Shape2[i], Shape2[(i + 1) % Shape2.size()]);
        AxisDirectionList2.push_back(Line2.getNormalDirection());
    }
    std::vector<Vector2> AxisDirectionList;
    for (auto& AxisDirection : AxisDirectionList1) {
        AxisDirectionList.push_back(AxisDirection);
    }
    for (auto& AxisDirection : AxisDirectionList2) {
        AxisDirectionList.push_back(AxisDirection);
    }
    for (auto& AxisDirection : AxisDirectionList) {
        if (AxisDirection.x == 0 && AxisDirection.y == 0)
        {
            std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0)
        {
            std::cout << "There is a vertical line" << std::endl;
        }
        else {
            Vector2 Orgin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Orgin);
            Line Axis(DirectionVectorAndPoint);
            std::vector<Vector2> ProjectionList1;
            std::vector<Vector2> ProjectionList2;
            for (auto& Point : Shape1) {
                ProjectionList1.push_back(Axis.projection(Point));
            }
            for (auto& Point : Shape2) {
                ProjectionList2.push_back(Axis.projection(Point));
            }
            int MinIndex1 = 0;
            int MaxIndex1 = 0;
            for (int i = 1; i < ProjectionList1.size(); ++i) {
                if (ProjectionList1[i].x < ProjectionList1[MinIndex1].x) {
                    MinIndex1 = i;
                }
                if (ProjectionList1[i].x > ProjectionList1[MaxIndex1].x) {
                    MaxIndex1 = i;
                }
            }
            int MinIndex2 = 0;
            int MaxIndex2 = 0;
            for (int i = 1; i < ProjectionList2.size(); ++i) {
                if (ProjectionList2[i].x < ProjectionList2[MinIndex2].x) {
                    MinIndex2 = i;
                }
                if (ProjectionList2[i].x > ProjectionList2[MaxIndex2].x) {
                    MaxIndex2 = i;
                }
            }
            if (ProjectionList1[MaxIndex1].x < ProjectionList2[MinIndex2].x || ProjectionList2[MaxIndex2].x < ProjectionList1[MinIndex1].x) {
                return false;
            }
        }
    }
    return true;
}

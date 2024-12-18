//
// Created by Le Hoang An on 15/12/24.
//

#include "Collider.h"
#include "../Math/Geometry.hpp"
#include "raymath.h"
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
//    Axis.draw(RED);
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


//    DrawCircle(ProjectionMax1.x, ProjectionMax1.y, 10, RED);
//    DrawCircle(Shape1[MaxYIndex1].x, Shape1[MaxYIndex1].y, 10, RED);
//    DrawLineEx(ProjectionMax1, ProjectionMin1, 10, RED);
//    DrawLineEx(ProjectionMax2, ProjectionMin2, 10, RED);


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
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
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
                if (ProjectionList1[i].y < ProjectionList1[MinIndex1].y) {
                    MinIndex1 = i;
                }
                if (ProjectionList1[i].y > ProjectionList1[MaxIndex1].y) {
                    MaxIndex1 = i;
                }
            }
            int MinIndex2 = 0;
            int MaxIndex2 = 0;
            for (int i = 1; i < ProjectionList2.size(); ++i) {
                if (ProjectionList2[i].y < ProjectionList2[MinIndex2].y) {
                    MinIndex2 = i;
                }
                if (ProjectionList2[i].y > ProjectionList2[MaxIndex2].y) {
                    MaxIndex2 = i;
                }
            }
            if (ProjectionList1[MaxIndex1].y < ProjectionList2[MinIndex2].y || ProjectionList2[MaxIndex2].y < ProjectionList1[MinIndex1].y) {
                return false;
            }
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
CollisionResolve TriangleSATCollider::getCollisionResolution(const std::vector<Vector2> &Shape1,
                                                             const std::vector<Vector2> &Shape2) {
    SATCollider* Collider = SATPolygonCollider::getSATPolygonCollider();
    return Collider->getCollisionResolution(Shape1, Shape2);
}
SATCollider* SATPolygonCollider::getSATPolygonCollider() {
    static SATPolygonCollider m_SATPolygonCollider;
    return &m_SATPolygonCollider;
}
bool SATPolygonCollider::isColliding(const std::vector<Vector2> &Shape1, const std::vector<Vector2> &Shape2) {
    // I accidentally wrote the full fetch polygon detection algorithm in the triangle SAT collider so I will just copy it here
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
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
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
                if (ProjectionList1[i].y < ProjectionList1[MinIndex1].y) {
                    MinIndex1 = i;
                }
                if (ProjectionList1[i].y > ProjectionList1[MaxIndex1].y) {
                    MaxIndex1 = i;
                }
            }
            int MinIndex2 = 0;
            int MaxIndex2 = 0;
            for (int i = 1; i < ProjectionList2.size(); ++i) {
                if (ProjectionList2[i].y < ProjectionList2[MinIndex2].y) {
                    MinIndex2 = i;
                }
                if (ProjectionList2[i].y > ProjectionList2[MaxIndex2].y) {
                    MaxIndex2 = i;
                }
            }
            if (ProjectionList1[MaxIndex1].y < ProjectionList2[MinIndex2].y || ProjectionList2[MaxIndex2].y < ProjectionList1[MinIndex1].y) {
                return false;
            }
        }
        else {
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
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
CollisionResolve SATPolygonCollider::getCollisionResolution(const std::vector<Vector2> &Shape1,
                                                                       const std::vector<Vector2> &Shape2) {
//    std::cout << "Getting Collision Resolution" << std::endl;
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
    Vector2 MinTranslationVectorMin1Max2 (INT_MAX, INT_MAX);
    Vector2 MinTranslationVectorMin2Max1 (INT_MAX, INT_MAX);
    for (auto& AxisDirection : AxisDirectionList) {
        if (AxisDirection.x == 0 && AxisDirection.y == 0)
        {
            std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0)
        {
            std::cout << "There is a vertical line" << std::endl;
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
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
                if (ProjectionList1[i].y < ProjectionList1[MinIndex1].y) {
                    MinIndex1 = i;
                }
                if (ProjectionList1[i].y > ProjectionList1[MaxIndex1].y) {
                    MaxIndex1 = i;
                }
            }
            int MinIndex2 = 0;
            int MaxIndex2 = 0;
            for (int i = 1; i < ProjectionList2.size(); ++i) {
                if (ProjectionList2[i].y < ProjectionList2[MinIndex2].y) {
                    MinIndex2 = i;
                }
                if (ProjectionList2[i].y > ProjectionList2[MaxIndex2].y) {
                    MaxIndex2 = i;
                }
            }
            if (ProjectionList1[MaxIndex1].y < ProjectionList2[MinIndex2].y || ProjectionList2[MaxIndex2].y < ProjectionList1[MinIndex1].y) {
                return {Vector2{0, 0}, Vector2{0, 0}};
            }
            Vector2 Min1Max2 = Vector2Subtract(ProjectionList2[MaxIndex2], ProjectionList1[MinIndex1]);
            Vector2 Min2Max1 = Vector2Subtract(ProjectionList1[MaxIndex1], ProjectionList2[MinIndex2]);
            if (Vector2Length(Min1Max2) < Vector2Length(MinTranslationVectorMin1Max2)) {
                MinTranslationVectorMin1Max2 = Min1Max2;
            }
            if (Vector2Length(Min2Max1) < Vector2Length(MinTranslationVectorMin2Max1)) {
                MinTranslationVectorMin2Max1 = Min2Max1;
            }
        }
        else {
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
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
                return {Vector2{0, 0}, Vector2{0, 0}};
            }
            Vector2 Min1Max2 = Vector2Subtract(ProjectionList2[MaxIndex2], ProjectionList1[MinIndex1]);
            Vector2 Min2Max1 = Vector2Subtract(ProjectionList1[MaxIndex1], ProjectionList2[MinIndex2]);
            if (Vector2Length(Min1Max2) < Vector2Length(MinTranslationVectorMin1Max2)) {
                MinTranslationVectorMin1Max2 = Min1Max2;
            }
            if (Vector2Length(Min2Max1) < Vector2Length(MinTranslationVectorMin2Max1)) {
                MinTranslationVectorMin2Max1 = Min2Max1;
            }
        }
    }
    if (Vector2Length(MinTranslationVectorMin1Max2) < Vector2Length(MinTranslationVectorMin2Max1)) {
        Vector2 FirstResolution = Vector2Scale(MinTranslationVectorMin1Max2, 0.5);
        Vector2 SecondResolution = Vector2Scale(MinTranslationVectorMin1Max2, -0.5);
        return {FirstResolution, SecondResolution};
    }
    else {
        Vector2 FirstResolution = Vector2Scale(MinTranslationVectorMin2Max1, 0.5);
        Vector2 SecondResolution = Vector2Scale(MinTranslationVectorMin2Max1, -0.5);
        return {FirstResolution, SecondResolution};
    }
}

SATCirclePolygonCollider *SATCirclePolygonCollider::getSATCirclePolygonCollider() {
    static SATCirclePolygonCollider m_SATCirclePolygonCollider;
    return &m_SATCirclePolygonCollider;
}
bool SATCirclePolygonCollider::isColliding(const SATPlatformCircle &Circle, const std::vector<Vector2> &Shape) {

    std::vector<Vector2> AxisDirectionList;
    Vector2 CircleClosestVertexDirection = Shape[0];
    for (int i = 0; i < Shape.size(); ++i)
    {
        Line Line1(Shape[i], Shape[(i + 1) % Shape.size()]);
        AxisDirectionList.push_back(Line1.getNormalDirection());
    }
    for (int i = 0; i < Shape.size(); ++i)
    {
        Vector2 Direction = Vector2Subtract(Shape[i], Circle.getCenter());
        if (Vector2Length(Direction) < Vector2Length(Vector2Subtract(CircleClosestVertexDirection, Circle.getCenter())))
        {
            CircleClosestVertexDirection = Shape[i];
        }
    }
    AxisDirectionList.push_back(Vector2Normalize(Vector2Subtract(Circle.getCenter(), CircleClosestVertexDirection)));
//    DrawLineEx(Circle.getCenter(), CircleClosestVertexDirection, 5, RED);
    Vector2 Min1;
    Vector2 Max2;
    Vector2 Min2;
    Vector2 Max1;
    for (auto& AxisDirection : AxisDirectionList) {
        AxisDirection = Vector2Normalize(AxisDirection);
        if (AxisDirection.x == 0 && AxisDirection.y == 0)
        {
            std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0)
        {
            std::cout << "There is a vertical line" << std::endl;
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
            Line Axis(DirectionVectorAndPoint);
            std::vector<Vector2> ProjectionList1;
            std::vector<Vector2> ProjectionList2;
            Vector2 Point1;
            Vector2 Point2;
            Point1 = Vector2Normalize(AxisDirection);
            Point1 = Vector2Scale(Point1, Circle.getRadius());
            Point1 = Vector2Add(Point1, Circle.getCenter());
            Point2 = Vector2Normalize(AxisDirection);
            Point2 = Vector2Scale(Point2, -Circle.getRadius());
            Point2 = Vector2Add(Point2, Circle.getCenter());
            ProjectionList1.push_back(Axis.projection(Point1));
            ProjectionList1.push_back(Axis.projection(Point2));
            for (auto& Point : Shape) {
                ProjectionList2.push_back(Axis.projection(Point));
            }
            int MinIndex1 = 0;
            int MaxIndex1 = 0;
            for (int i = 1; i < ProjectionList1.size(); ++i) {
                if (ProjectionList1[i].y < ProjectionList1[MinIndex1].y) {
                    MinIndex1 = i;
                }
                if (ProjectionList1[i].y > ProjectionList1[MaxIndex1].y) {
                    MaxIndex1 = i;
                }
            }
            int MinIndex2 = 0;
            int MaxIndex2 = 0;
            for (int i = 1; i < ProjectionList2.size(); ++i) {
                if (ProjectionList2[i].y < ProjectionList2[MinIndex2].y) {
                    MinIndex2 = i;
                }
                if (ProjectionList2[i].y > ProjectionList2[MaxIndex2].y) {
                    MaxIndex2 = i;
                }
            }
            Min1 = ProjectionList1[MinIndex1];
            Max1 = ProjectionList1[MaxIndex1];
            Min2 = ProjectionList2[MinIndex2];
            Max2 = ProjectionList2[MaxIndex2];
            if (ProjectionList1[MaxIndex1].y < ProjectionList2[MinIndex2].y || ProjectionList2[MaxIndex2].y < ProjectionList1[MinIndex1].y) {
                return false;
            }
        }
        else
        {
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
            Line Axis(DirectionVectorAndPoint);
            std::vector<Vector2> ProjectionList1;
            std::vector<Vector2> ProjectionList2;
            Vector2 Point1;
            Vector2 Point2;
            Point1 = Vector2Normalize(AxisDirection);
            Point1 = Vector2Scale(Point1, Circle.getRadius());
            Point1 = Vector2Add(Point1, Circle.getCenter());
            Point2 = Vector2Normalize(AxisDirection);
            Point2 = Vector2Scale(Point2, -Circle.getRadius());
            Point2 = Vector2Add(Point2, Circle.getCenter());
            ProjectionList1.push_back(Axis.projection(Point1));
            ProjectionList1.push_back(Axis.projection(Point2));
            for (auto& Point : Shape) {
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
            Min1 = ProjectionList1[MinIndex1];
            Max1 = ProjectionList1[MaxIndex1];
            Min2 = ProjectionList2[MinIndex2];
            Max2 = ProjectionList2[MaxIndex2];
            if (ProjectionList1[MaxIndex1].x < ProjectionList2[MinIndex2].x || ProjectionList2[MaxIndex2].x < ProjectionList1[MinIndex1].x) {
                return false;
            }
//            DrawLineEx(Min1, Max1, 5, RED);
//            DrawLineEx(Min2, Max2, 3, BLUE);
        }
        if (Vector2Length(Vector2Subtract(Min1, Max2)) < Vector2Length(Vector2Subtract(Min2, Max1)))
        {
//            std::cout << "Case 1" << std::endl;
        }
        else
        {
//            std::cout << "Case 2" << std::endl;
        }
    }
    return true;
}
CollisionResolve SATCirclePolygonCollider::getCollisionResolution(const SATPlatformCircle &Circle, std::vector<Vector2> Shape) {
    Shape = dilatePolygon(Shape, 1.01f);
    std::vector<Vector2> AxisDirectionList;
    Vector2 CircleClosestVertexDirection = Shape[0];
    for (int i = 0; i < Shape.size(); ++i)
    {
        Line Line1(Shape[i], Shape[(i + 1) % Shape.size()]);
        AxisDirectionList.push_back(Line1.getNormalDirection());
    }
    for (int i = 0; i < Shape.size(); ++i)
    {
        Vector2 Direction = Vector2Subtract(Shape[i], Circle.getCenter());
        if (Vector2Length(Direction) < Vector2Length(Vector2Subtract(CircleClosestVertexDirection, Circle.getCenter())))
        {
            CircleClosestVertexDirection = Shape[i];
        }
    }
    AxisDirectionList.push_back(Vector2Normalize(Vector2Subtract(Circle.getCenter(), CircleClosestVertexDirection)));
    Vector2 MinTranslationVectorMin1Max2 (INT_MAX, INT_MAX);
    Vector2 MinTranslationVectorMin2Max1(INT_MAX, INT_MAX);
    Vector2 Min1;
    Vector2 Max2;
    Vector2 Min2;
    Vector2 Max1;
    for (auto& AxisDirection : AxisDirectionList) {
        if (AxisDirection.x == 0 && AxisDirection.y == 0) {
            std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0) {
            std::cout << "There is a vertical line" << std::endl;
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
            Line Axis(DirectionVectorAndPoint);
            std::vector<Vector2> ProjectionList1;
            std::vector<Vector2> ProjectionList2;
            Vector2 Point1;
            Vector2 Point2;
            Point1 = Vector2Normalize(AxisDirection);
            Point1 = Vector2Scale(Point1, Circle.getRadius());
            Point1 = Vector2Add(Point1, Circle.getCenter());
            Point2 = Vector2Normalize(AxisDirection);
            Point2 = Vector2Scale(Point2, -Circle.getRadius());
            Point2 = Vector2Add(Point2, Circle.getCenter());
            ProjectionList1.push_back(Axis.projection(Point1));
            ProjectionList1.push_back(Axis.projection(Point2));
            for (auto& Point : Shape) {
                ProjectionList2.push_back(Axis.projection(Point));
            }
            int MinIndex1 = 0;
            int MaxIndex1 = 0;
            for (int i = 1; i < ProjectionList1.size(); ++i) {
                if (ProjectionList1[i].y < ProjectionList1[MinIndex1].y) {
                    MinIndex1 = i;
                }
                if (ProjectionList1[i].y > ProjectionList1[MaxIndex1].y) {
                    MaxIndex1 = i;
                }
            }
            int MinIndex2 = 0;
            int MaxIndex2 = 0;
            for (int i = 1; i < ProjectionList2.size(); ++i) {
                if (ProjectionList2[i].y < ProjectionList2[MinIndex2].y) {
                    MinIndex2 = i;
                }
                if (ProjectionList2[i].y > ProjectionList2[MaxIndex2].y) {
                    MaxIndex2 = i;
                }
            }
            Min1 = ProjectionList1[MinIndex1];
            Max1 = ProjectionList1[MaxIndex1];
            Min2 = ProjectionList2[MinIndex2];
            Max2 = ProjectionList2[MaxIndex2];
            if (ProjectionList1[MaxIndex1].y < ProjectionList2[MinIndex2].y || ProjectionList2[MaxIndex2].y < ProjectionList1[MinIndex1].y) {
                return {Vector2{0, 0}, Vector2{0, 0}};
            }
            Vector2 Min1Max2 = Vector2Subtract(ProjectionList2[MaxIndex2], ProjectionList1[MinIndex1]);
            Vector2 Min2Max1 = Vector2Subtract(ProjectionList1[MaxIndex1], ProjectionList2[MinIndex2]);
            if (Vector2Length(Min1Max2) < Vector2Length(MinTranslationVectorMin1Max2)) {
                MinTranslationVectorMin1Max2 = Min1Max2;
            }
            if (Vector2Length(Min2Max1) < Vector2Length(MinTranslationVectorMin2Max1)) {
                MinTranslationVectorMin2Max1 = Min2Max1;
            }
        }
        else {
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
            Line Axis(DirectionVectorAndPoint);
            std::vector<Vector2> ProjectionList1;
            std::vector<Vector2> ProjectionList2;
            Vector2 Point1;
            Vector2 Point2;
            Point1 = Vector2Normalize(AxisDirection);
            Point1 = Vector2Scale(Point1, Circle.getRadius());
            Point1 = Vector2Add(Point1, Circle.getCenter());
            Point2 = Vector2Normalize(AxisDirection);
            Point2 = Vector2Scale(Point2, -Circle.getRadius());
            Point2 = Vector2Add(Point2, Circle.getCenter());
            ProjectionList1.push_back(Axis.projection(Point1));
            ProjectionList1.push_back(Axis.projection(Point2));
            for (auto& Point : Shape) {
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
            Min1 = ProjectionList1[MinIndex1];
            Max1 = ProjectionList1[MaxIndex1];
            Min2 = ProjectionList2[MinIndex2];
            Max2 = ProjectionList2[MaxIndex2];
            if (ProjectionList1[MaxIndex1].x < ProjectionList2[MinIndex2].x || ProjectionList2[MaxIndex2].x < ProjectionList1[MinIndex1].x) {
                return {Vector2{0, 0}, Vector2{0, 0}};
            }
            Vector2 Min1Max2 = Vector2Subtract(ProjectionList2[MaxIndex2], ProjectionList1[MinIndex1]);
            Vector2 Min2Max1 = Vector2Subtract(ProjectionList1[MaxIndex1], ProjectionList2[MinIndex2]);
            if (Vector2Length(Min1Max2) < Vector2Length(MinTranslationVectorMin1Max2)) {
                MinTranslationVectorMin1Max2 = Min1Max2;
            }
            if (Vector2Length(Min2Max1) < Vector2Length(MinTranslationVectorMin2Max1)) {
                MinTranslationVectorMin2Max1 = Min2Max1;
            }
        }

    }
//    DrawLineEx(Min1, Max1, 5, RED);
//    DrawLineEx(Min2, Max2, 3, BLUE);
    if (Vector2Length(MinTranslationVectorMin1Max2) < Vector2Length(MinTranslationVectorMin2Max1)) {
        Vector2 FirstResolution = Vector2Scale(MinTranslationVectorMin1Max2, 0.5);
        Vector2 SecondResolution = Vector2Scale(MinTranslationVectorMin1Max2, -0.5);
        return {FirstResolution, SecondResolution};
    }
    else {
        Vector2 FirstResolution = Vector2Scale(MinTranslationVectorMin2Max1, -0.5);
        Vector2 SecondResolution = Vector2Scale(MinTranslationVectorMin2Max1, 0.5);
        return {FirstResolution, SecondResolution};
    }
    return {Vector2{0, 0}, Vector2{0, 0}};
}

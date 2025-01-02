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
            // std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0)
        {
            // std::cout << "There is a vertical line" << std::endl;
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
        if (AxisDirection.x == 0 && AxisDirection.y == 0) {
            // std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0) {
            // std::cout << "There is a vertical line" << std::endl;
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
            Line Axis(DirectionVectorAndPoint);
            std::vector<Vector2> ProjectionList1;
            std::vector<Vector2> ProjectionList2;
            for (auto &Point: Shape1) {
                ProjectionList1.push_back(Axis.projection(Point));
            }
            for (auto &Point: Shape2) {
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
            if (ProjectionList1[MaxIndex1].y < ProjectionList2[MinIndex2].y ||
                ProjectionList2[MaxIndex2].y < ProjectionList1[MinIndex1].y) {
                return false;
            }
        } else {
            Vector2 Origin = {600, 600};
            std::pair<Vector2, Vector2> DirectionVectorAndPoint(AxisDirection, Origin);
            Line Axis(DirectionVectorAndPoint);
            std::vector<Vector2> ProjectionList1;
            std::vector<Vector2> ProjectionList2;
            for (auto &Point: Shape1) {
                ProjectionList1.push_back(Axis.projection(Point));
            }
            for (auto &Point: Shape2) {
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
            if (ProjectionList1[MaxIndex1].x < ProjectionList2[MinIndex2].x ||
                ProjectionList2[MaxIndex2].x < ProjectionList1[MinIndex1].x) {
                return false;
            }
        }
    }
    return true;
}
std::vector<Vector2> SATPolygonCollider::getPointsOfIntersection(const std::vector<Vector2> &Shape1, const std::vector<Vector2> &Shape2) {
    std::vector<Vector2> Result;
    SATCollider* Collider = SATPolygonCollider::getSATPolygonCollider();
    if (Collider -> isColliding(Shape1, Shape2)) {
//        std::cout << "Colliding" << std::endl;
        vector<Vector2> VerticesWithProjection1;
        vector<Vector2> VerticesWithProjection2;
        vector<LineSegment> LineSegments1;
        for (int i = 0; i < Shape1.size(); ++i) {
            LineSegment Edge = {Shape1[i], Shape1[(i + 1) % Shape1.size()]};
            LineSegments1.push_back(Edge);
        }
        vector<LineSegment> LineSegments2;
        for (int i = 0; i < Shape2.size(); ++i) {
            LineSegment Edge = {Shape2[i], Shape2[(i + 1) % Shape2.size()]};
            LineSegments2.push_back(Edge);
        }
        for (auto& Vertex : Shape1) {
            for (auto& Edge : LineSegments2) {
                if (Edge.haveProjection(Vertex)) {
                    if (isInsidePolygon(Vertex, LineSegments2)) {
                        VerticesWithProjection1.push_back(Vertex);
                        break;
                    }
                }
            }
        }
        for (auto& Vertex : Shape2) {
            for (auto& Edge : LineSegments1) {
                if (Edge.haveProjection(Vertex)) {
                    if (isInsidePolygon(Vertex, LineSegments1)) {
                        VerticesWithProjection2.push_back(Vertex);
                        break;
                    }
                }
            }
        }
//        std::cout << "Number of vertices with projection 1: " << VerticesWithProjection1.size() << std::endl;
//        std::cout << "Number of vertices with projection 2: " << VerticesWithProjection2.size() << std::endl;
        const float InaccuracyThreshold = 0.000000001f;
        float MinDistance1 = INT_MAX;
        int MinimumDistanceIndex1 = 0;
        for (int i = 0; i < VerticesWithProjection1.size(); ++i) {
            float LocalMinDistance = INT_MAX;
            for (auto& Edge : LineSegments2) {
                float Distance = Line(Edge).distanceToPoint(VerticesWithProjection1[i]);
                if (Distance < LocalMinDistance) {
                    LocalMinDistance = Distance;
                }
            }
            if (LocalMinDistance < MinDistance1) {
                MinDistance1 = LocalMinDistance;
                MinimumDistanceIndex1 = i;
            }
        }
        float MinDistance2 = INT_MAX;
        int MinimumDistanceIndex2 = 0;
        for (int i = 0; i < VerticesWithProjection2.size(); ++i) {
            float LocalMinDistance = INT_MAX;
            for (auto& Edge : LineSegments1) {
                float Distance = Line(Edge).distanceToPoint(VerticesWithProjection2[i]);
                if (Distance < LocalMinDistance) {
                    LocalMinDistance = Distance;
                }
            }
            if (LocalMinDistance < MinDistance2) {
                MinDistance2 = LocalMinDistance;
                MinimumDistanceIndex2 = i;
            }
        }
        float MinimumDistance;
        for (auto& Point : VerticesWithProjection2) {
            DrawCircle(Point.x, Point.y, 10, RED);
        }
//        Vector2 Vertex = VerticesWithProjection2[2];
//        DrawCircle(Vertex.x, Vertex.y, 10, PURPLE);
//        bool haveEdgeProjection = false;
//        for (auto& Edge : LineSegments1) {
////            if (Edge.haveProjection(Vertex)) {
//                float Distance = Line(Edge).distanceToPoint(Vertex);
//                std::cout << "Distance: " << Distance << std::endl;
//                if (Distance == MinDistance2) {
//
//                    Vector2 Projection = Line(Edge).projection(Vertex);
//                    DrawCircle(Projection.x, Projection.y, 10, GREEN);
//                    DrawLineEx(Vertex, Projection, 10, BLUE);
//                    haveEdgeProjection = true;
//                    break;
//                }
////                break;
////            }
//        }
//        if (!haveEdgeProjection) {
//            std::cout << "No projection" << std::endl;
//        }
        if (MinDistance1 < MinDistance2) {
            MinimumDistance = MinDistance1;
//            std::cout << "Min is 1" << std::endl;
//            std::cout << "Min 2: " << MinDistance2 << std::endl;
//            Vector2 Vertex = VerticesWithProjection1[MinimumDistanceIndex1];
//            DrawCircle(Vertex.x, Vertex.y, 10, YELLOW);
//            for (auto& Edge : LineSegments2) {
//                if (Edge.haveProjection(Vertex)) {
//                    if (Line(Edge).distanceToPoint(Vertex)== MinimumDistance) {
//                        Vector2 Projection = Line(Edge).projection(Vertex);
//                        DrawCircle(Projection.x, Projection.y, 10, GREEN);
//                        DrawLineEx(Vertex, Projection, 10, BLUE);
//                        break;
//                    }
//                }
//            }

        }
        else {
            MinimumDistance = MinDistance2;
//            std::cout << "Min is 2" << std::endl;
//            std::cout << "Min 1: " << MinDistance1 << std::endl;
//            Vector2 Vertex = VerticesWithProjection2[MinimumDistanceIndex2];
//            DrawCircle(Vertex.x, Vertex.y, 10, YELLOW);
//            for (auto& Edge : LineSegments2) {
//                if (Edge.haveProjection(Vertex)) {
//                    if (Line(Edge).distanceToPoint(Vertex)== MinimumDistance) {
//                        Vector2 Projection = Line(Edge).projection(Vertex);
//                        DrawCircle(Projection.x, Projection.y, 10, GREEN);
//                        DrawLineEx(Vertex, Projection, 10, BLUE);
//                        break;
//                    }
//                }
//            }
        }
//        std::cout << "Minimum distance: " << MinimumDistance << std::endl;
        for (int i = 0; i < VerticesWithProjection1.size(); ++i) {
            for (auto& Edge : LineSegments2) {
                if (!Edge.haveProjection(VerticesWithProjection1[i])) continue;
                float Distance = Line(Edge).distanceToPoint(VerticesWithProjection1[i]);
                if (Distance <= MinimumDistance + InaccuracyThreshold) {
                    Vector2 Projection = Line(Edge).projection(VerticesWithProjection1[i]);
                    Result.push_back(Projection);
                    break;
                }
            }
        }
        for (int i = 0; i < VerticesWithProjection2.size(); ++i) {
            for (auto &Edge: LineSegments1) {
                if (!(Edge.haveProjection(VerticesWithProjection2[i]))) continue;
                float Distance = Line(Edge).distanceToPoint(VerticesWithProjection2[i]);
                if (Distance <= MinimumDistance + InaccuracyThreshold) {
                    Vector2 Projection = Line(Edge).projection(VerticesWithProjection2[i]);
                    Result.push_back(VerticesWithProjection2[i]);
                    break;
                }
            }
        }
//        std::cout << "Number of contact points: " << Result.size() << std::endl;
    }
    else
    {
         std::cout << "Not colliding" << std::endl;
    }
    return Result;
}
bool SATPolygonCollider::IsAVertexOf(const Vector2& Point, const std::vector<Vector2> &Shape) {
    for (auto& Vertex : Shape) {
        if (Point == Vertex) {
            return true;
        }
    }
    return false;
}
Vector2 SATPolygonCollider::getNormalDirection(const std::vector<Vector2> &Shape1, const std::vector<Vector2> &Shape2) {
    bool IsColliding = isColliding(Shape1, Shape2);
    if (!IsColliding) {
        return {0, 0};
    }
//    Vector2 SelectedContactPoint;
    std::vector<LineSegment> LineSegments1;
    for (int i = 0; i < Shape1.size(); ++i) {
        LineSegment Edge = {Shape1[i], Shape1[(i + 1) % Shape1.size()]};
        LineSegments1.push_back(Edge);
    }
    std::vector<LineSegment> LineSegments2;
    for (int i = 0; i < Shape2.size(); ++i) {
        LineSegment Edge = {Shape2[i], Shape2[(i + 1) % Shape2.size()]};
        LineSegments2.push_back(Edge);
    }
    for (auto& ContactPoint : getPointsOfIntersection(Shape1, Shape2)) {
        if (IsAVertexOf(ContactPoint, Shape1)) {
            if (isInsidePolygon(ContactPoint, LineSegments2)) {
                float MinDistance = INT_MAX;
                int ClosestEdgeIndex = 0;
                for (int i = 0; i < LineSegments2.size(); ++i) {
                    Line Line2(LineSegments2[i]);
                    if (Line2.distanceToPoint(ContactPoint) < MinDistance) {
                        MinDistance = Line2.distanceToPoint(ContactPoint);
                        ClosestEdgeIndex = i;
                    }
                }
                Vector2 NormalDirection = Line(LineSegments2[ClosestEdgeIndex]).getNormalDirection();
                NormalDirection = Vector2Normalize(NormalDirection);
                return NormalDirection;
                break;
            }
        }
        else {
            if (isInsidePolygon(ContactPoint, LineSegments1)) {
                float MinDistance = INT_MAX;
                int ClosestEdgeIndex = 0;
                for (int i = 0; i < LineSegments1.size(); ++i) {
                    Line Line1(LineSegments1[i]);
                    if (Line1.distanceToPoint(ContactPoint) < MinDistance) {
                        MinDistance = Line1.distanceToPoint(ContactPoint);
                        ClosestEdgeIndex = i;
                    }
                }
                Vector2 NormalDirection = Line(LineSegments1[ClosestEdgeIndex]).getNormalDirection();
                NormalDirection = Vector2Normalize(NormalDirection);
                return NormalDirection;
                break;
            }
        }
    }

}
Vector2 SATPolygonCollider::getNormalDirection(const Vector2 &Point, const std::vector<Vector2> &Shape1,
                                               const std::vector<Vector2> &Shape2) {

    std::vector<LineSegment> LineSegments1;
    for (int i = 0; i < Shape1.size(); ++i) {
        LineSegment Edge = {Shape1[i], Shape1[(i + 1) % Shape1.size()]};
        LineSegments1.push_back(Edge);
    }
    std::vector<LineSegment> LineSegments2;
    for (int i = 0; i < Shape2.size(); ++i) {
        LineSegment Edge = {Shape2[i], Shape2[(i + 1) % Shape2.size()]};
        LineSegments2.push_back(Edge);
    }
    for (auto& Vertex : Shape1) {
        if (Vertex == Point) {
            float MinDistance = INT_MAX;
            int ClosestEdgeIndex = 0;
            for (int i = 0; i < LineSegments2.size(); ++i)
            {
                Line Line2(LineSegments2[i]);
                if (Line2.distanceToPoint(Point) < MinDistance)
                {
                    MinDistance = Line2.distanceToPoint(Point);
                    ClosestEdgeIndex = i;
                }
            }
            Vector2 Center = {0, 0};
            for (auto& Vertex : Shape2) {
                Center = Vector2Add(Center, Vertex);
            }
            Center = Vector2Scale(Center, 1.0f / Shape2.size());
            Line SelectedLine = Line(LineSegments2[ClosestEdgeIndex]);
            Vector2 Projection = SelectedLine.projection(Center);
            Vector2 NormalDirection = Vector2Subtract(Projection, Center);
            NormalDirection = Vector2Normalize(NormalDirection);
            return NormalDirection;
        }
    }
    for (auto& Vertex : Shape2) {
        if (Vertex == Point) {
            float MinDistance = INT_MAX;
            int ClosestEdgeIndex = 0;
            for (int i = 0; i < LineSegments1.size(); ++i)
            {
                Line Line1(LineSegments1[i]);
                if (Line1.distanceToPoint(Point) < MinDistance)
                {
                    MinDistance = Line1.distanceToPoint(Point);
                    ClosestEdgeIndex = i;
                }
            }
            Vector2 Center = {0, 0};
            for (auto& Vertex : Shape1) {
                Center = Vector2Add(Center, Vertex);
            }
            Center = Vector2Scale(Center, 1.0f / Shape1.size());
            Line SelectedLine = Line(LineSegments1[ClosestEdgeIndex]);
            Vector2 Projection = SelectedLine.projection(Center);
            Vector2 NormalDirection = Vector2Subtract(Projection, Center);
            NormalDirection = Vector2Normalize(NormalDirection);
            return NormalDirection;
        }
    }
    if (isInsidePolygon(Point, LineSegments1))
    {
        float MinDistance = INT_MAX;
        int ClosestEdgeIndex = 0;
        for (int i = 0; i < LineSegments1.size(); ++i) {
            Line Line1(LineSegments1[i]);
            if (Line1.distanceToPoint(Point) < MinDistance) {
                MinDistance = Line1.distanceToPoint(Point);
                ClosestEdgeIndex = i;
            }
        }
        Vector2 Center = {0, 0};
        for (auto& Vertex : Shape1) {
            Center = Vector2Add(Center, Vertex);
        }
        Center = Vector2Scale(Center, 1.0f / Shape1.size());
        Line SelectedLine = Line(LineSegments1[ClosestEdgeIndex]);
        Vector2 Projection = SelectedLine.projection(Point);
        Vector2 NormalDirection = Vector2Subtract(Point, Center);
        NormalDirection = Vector2Normalize(NormalDirection);
        return NormalDirection;
    }
    else if (isInsidePolygon(Point, LineSegments2))
    {
        float MinDistance = INT_MAX;
        int ClosestEdgeIndex = 0;
        for (int i = 0; i < LineSegments2.size(); ++i) {
            Line Line2(LineSegments2[i]);
            if (Line2.distanceToPoint(Point) < MinDistance) {
                MinDistance = Line2.distanceToPoint(Point);
                ClosestEdgeIndex = i;
            }
        }
        Vector2 Center = {0, 0};
        for (auto& Vertex : Shape2) {
            Center = Vector2Add(Center, Vertex);
        }
        Center = Vector2Scale(Center, 1.0f / Shape2.size());

        Line SelectedLine = Line(LineSegments2[ClosestEdgeIndex]);
        Vector2 Projection = SelectedLine.projection(Point);
        Vector2 NormalDirection = Vector2Subtract(Point, Center);
        NormalDirection = Vector2Normalize(NormalDirection);
        return NormalDirection;
    }
    else
    {
        return {1, 1};
//        throw std::invalid_argument("Point is not inside any of the polygons");
    }
}
CollisionResolve SATPolygonCollider::getCollisionResolution(const std::vector<Vector2> &Shape1,
                                                                       const std::vector<Vector2> &Shape2) {
//    std::cout << "Getting collision resolution" << std::endl;
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
            // std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0)
        {
            // std::cout << "There is a vertical line" << std::endl;
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
        Vector2 FirstResolution = Vector2Scale(MinTranslationVectorMin1Max2, +0.5);
        Vector2 SecondResolution = Vector2Scale(MinTranslationVectorMin1Max2, -0.5);
//        std::cout << "First resolution: " << FirstResolution.x << " " << FirstResolution.y << std::endl;
//        std::cout << "Second resolution: " << SecondResolution.x << " " << SecondResolution.y << std::endl;
        return {FirstResolution, SecondResolution};
    }
    else {
        Vector2 FirstResolution = Vector2Scale(MinTranslationVectorMin2Max1, -0.5);
        Vector2 SecondResolution = Vector2Scale(MinTranslationVectorMin2Max1, +0.5);
//        std::cout << "First resolution: " << FirstResolution.x << " " << FirstResolution.y << std::endl;
//        std::cout << "Second resolution: " << SecondResolution.x << " " << SecondResolution.y << std::endl;
        return {FirstResolution, SecondResolution};
    }
}
CollisionResolve SATPolygonCollider::getCollisionResolution(const std::vector<Vector2> &Shape1, const std::vector<Vector2> &Shape2, bool IsShape1Fixed, bool IsShape2Fixed) {
    CollisionResolve CollisionResolution = getCollisionResolution(Shape1, Shape2);
    if (IsShape1Fixed && IsShape2Fixed) {
        return {0, 0};
    }
    if (IsShape1Fixed) {
        Vector2 Resolution = CollisionResolution.SecondResolution;
        Resolution = Vector2Add(Vector2Negate(CollisionResolution.FirstResolution), Resolution);
        CollisionResolution.FirstResolution = {0, 0};
        CollisionResolution.SecondResolution = Resolution;
        return CollisionResolution;
    }
    if (IsShape2Fixed) {
        Vector2 Resolution = CollisionResolution.FirstResolution;
        Resolution = Vector2Add(Vector2Negate(CollisionResolution.SecondResolution), Resolution);
        CollisionResolution.FirstResolution = Resolution;
        CollisionResolution.SecondResolution = {0, 0};
        return CollisionResolution;
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
            // std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0)
        {
            // std::cout << "There is a vertical line" << std::endl;
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
            // std::cout << "Not checking this axis" << std::endl;
            continue;
        }
        if (AxisDirection.x == 0) {
            // std::cout << "There is a vertical line" << std::endl;
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
std::vector<Vector2> SATCirclePolygonCollider::getPointsOfIntersection(const SATPlatformCircle &Circle, const std::vector<Vector2> &Shape) {
    std::vector<Vector2> Result;
    SATCirclePolygonCollider* Collider = SATCirclePolygonCollider::getSATCirclePolygonCollider();
    if (Collider->isColliding(Circle, Shape))
    {
        // find if there is a vertex inside the circle
        bool Inside = false;
        vector<Vector2> Vertices = Shape;
        for (int i = 0; i < Vertices.size(); ++i)
        {
            if (Vector2Distance(Circle.getCenter(), Vertices[i]) < Circle.getRadius())
            {
                Inside = true;
                break;
            }
        }
        if (Inside) {
            // find the closest vertex to the circle center
            int ClosestVertexIndex = 0;
            for (int i = 0; i < Vertices.size(); ++i)
            {
                if (Vector2Distance(Circle.getCenter(), Vertices[i]) < Vector2Distance(Circle.getCenter(), Vertices[ClosestVertexIndex]))
                {
                    ClosestVertexIndex = i;
                }
            }
            Vector2 Projection = Vertices[ClosestVertexIndex];
            Result.push_back(Projection);
        }
        else {
            // find the closest edge to the circle center
            vector<Vector2> Vertices = Shape;
            vector<LineSegment> Edges;
            for (int i = 0; i < Vertices.size(); ++i)
            {
                LineSegment Edge = {Vertices[i], Vertices[(i + 1) % Vertices.size()]};
                Edges.push_back(Edge);
            }
            vector<bool> HaveProjection(Edges.size(), false);
            int ClosestEdgeIndex;
            for (int i = 0; i < Edges.size(); ++i)
            {
                if (Edges[i].haveProjection(Circle.getCenter()))
                {
                    HaveProjection[i] = true;
                    ClosestEdgeIndex = i;
                }
            }
            for (int i = 0; i < Edges.size(); ++i)
            {
                if (!HaveProjection[i]) continue;
                Line LineEdge(Edges[i]);
                Line MinLineEdge(Edges[ClosestEdgeIndex]);
                if (LineEdge.distanceToPoint(Circle.getCenter()) < MinLineEdge.distanceToPoint(Circle.getCenter()))
                {
                    ClosestEdgeIndex = i;
                }
            }
            Line ClosestEdgeLine(Edges[ClosestEdgeIndex]);
            Vector2 Projection = ClosestEdgeLine.projection(Circle.getCenter());
            Result.push_back(Projection);
        }
    }
}

CollisionResolve SATCirclePolygonCollider::getCollisionResolution(const SATPlatformCircle &Circle, std::vector<Vector2> Shape, bool IsCircleFixed, bool IsShapeFixed) {
    if (IsCircleFixed && IsShapeFixed) {
        return {0, 0};
    }
    if (IsCircleFixed) {
        CollisionResolve CollisionResolution = getCollisionResolution(Circle, Shape);
        Vector2 Resolution = CollisionResolution.SecondResolution;
        Resolution = Vector2Add(Vector2Negate(CollisionResolution.FirstResolution), Resolution);
        CollisionResolution.FirstResolution = {0, 0};
        CollisionResolution.SecondResolution = Resolution;
        return CollisionResolution;
    }
    if (IsShapeFixed) {
        CollisionResolve CollisionResolution = getCollisionResolution(Circle, Shape);
        Vector2 Resolution = CollisionResolution.FirstResolution;
        Resolution = Vector2Add(Vector2Negate(CollisionResolution.SecondResolution), Resolution);
        CollisionResolution.FirstResolution = Resolution;
        CollisionResolution.SecondResolution = {0, 0};
        return CollisionResolution;
    }
}
SATRotatingCollider* SATRotatingCollider::getSATRotatingCollider() {
    static SATRotatingCollider m_SATRotatingCollider;
    return &m_SATRotatingCollider;
}
AngularCollisionResolve SATRotatingCollider::getCollisionResolution(SATRotatingPlatformPolygon *Shape1, SATRotatingPlatformPolygon *Shape2) {
    if (Shape1->isFixed() && Shape2->isFixed()) {
        AngularCollisionResolve Result;
        Result.FirstPositionResolution = {0, 0};
        Result.SecondPositionResolution = {0, 0};
        Result.FirstAngularResolution = Shape1->getRotationalVelocity();
        Result.SecondAngularResolution = Shape2->getRotationalVelocity();
        Result.FirstVelocityResolution = Shape1->getVelocity();
        Result.SecondVelocityResolution = Shape2->getVelocity();
        return Result;
    }
    SATPolygonCollider* PolygonCollider = dynamic_cast<SATPolygonCollider*>(SATPolygonCollider::getSATPolygonCollider());
    bool IsColliding = PolygonCollider->isColliding(Shape1->getVertices(), Shape2->getVertices());
    if (IsColliding) {
        float Mass1 = Shape1->getMass();
        std::cout << "Mass 1: " << Mass1 << std::endl;
        float Mass2 = Shape2->getMass();
        std::cout  << "Mass 2: " << Mass2 << std::endl;
        Vector2 Velocity1 = Shape1->getVelocity();
        Vector2 Velocity2 = Shape2->getVelocity();
        float AngularVelocity1 = Shape1->getRotationalVelocity();
        float AngularVelocity2 = Shape2->getRotationalVelocity();
        SATPolygonCollider* PolygonCollider = dynamic_cast<SATPolygonCollider*>(SATPolygonCollider::getSATPolygonCollider());
        int NumberOfContactPoints = PolygonCollider->getPointsOfIntersection(Shape1->getVertices(), Shape2->getVertices()).size();
        if (NumberOfContactPoints == 0) {
            AngularCollisionResolve Result;
            Result.FirstPositionResolution = {0, 0};
            Result.SecondPositionResolution = {0, 0};
            Result.FirstAngularResolution = Shape1->getRotationalVelocity();
            Result.SecondAngularResolution = Shape2->getRotationalVelocity();
            Result.FirstVelocityResolution = Shape1->getVelocity();
            Result.SecondVelocityResolution = Shape2->getVelocity();
            return Result;
        }
//        Vector2 ContactPoint = PolygonCollider->getPointsOfIntersection(Shape1->getVertices(), Shape2->getVertices())[0];
        Vector2 ContactPoint = {0, 0};
        for (auto& Point : PolygonCollider->getPointsOfIntersection(Shape1->getVertices(), Shape2->getVertices())) {
            ContactPoint = Vector2Add(ContactPoint, Point);
        }
        ContactPoint = Vector2Scale(ContactPoint, 1.0f / NumberOfContactPoints);
        Vector2 Normal = PolygonCollider->getNormalDirection(ContactPoint, Shape1->getVertices(), Shape2->getVertices());
        Vector2 Normal10 = Vector2Scale(Normal, 100);
        drawArrow(ContactPoint, Vector2Add(ContactPoint, Normal10), GREEN);
        Vector2 Tangent1 = calculateTangentalVelocity(Shape1->getCenter(), ContactPoint, Shape1->getRotationalVelocity());
        Vector2 Tangent2 = calculateTangentalVelocity(Shape2->getCenter(), ContactPoint, Shape2->getRotationalVelocity());
        float Inertia1 = Shape1->calculateMomentOfInertia();
        float Inertia2 = Shape2->calculateMomentOfInertia();
        float Impulse = calculateImpulse(Mass1, Mass2, Vector2Add(Velocity1, Tangent1), Vector2Add(Velocity2, Tangent2), 1.0f, Normal, Inertia1, Inertia2, Tangent1, Tangent2);
        AngularCollisionResolve Result;

        Vector2 NewVelocity1 = Vector2Scale(Normal, Impulse / Mass1);
        NewVelocity1 = Vector2Negate(NewVelocity1);
        //        NewVelocity1 = Vector2Add(NewVelocity1, Velocity1);

        Vector2 NewVelocity2 = Vector2Scale(Normal, -Impulse / Mass2);
        NewVelocity2 = Vector2Negate(NewVelocity2);
        //        NewVelocity2 = Vector2Add(NewVelocity2, Velocity2);

        Result.FirstVelocityResolution = NewVelocity1;
        Result.SecondVelocityResolution = NewVelocity2;
                Vector2 ra = Vector2Subtract(ContactPoint, Shape1->getCenter());
                Vector2 rb = Vector2Subtract(ContactPoint, Shape2->getCenter());
                float NewAngularVelocity1 = Vector2DotProduct(ra, Normal) * (-float(1) / Inertia1);
                std::cout << "New Angular Velocity 1: " << NewAngularVelocity1 << std::endl;
                float NewAngularVelocity2 = Vector2DotProduct(rb, Normal) * (float(1) / Inertia2);
                std::cout << "New Angular Velocity 2: " << NewAngularVelocity2 << std::endl;
                Vector2 ImpulseVector = Vector2Scale(Normal, Impulse);

                float CrossProduct1 = crossProduct(ra, ImpulseVector);
                drawArrow(Shape1->getCenter(), ContactPoint, RED);
                NewAngularVelocity1 = NewAngularVelocity2 * CrossProduct1;
                float CrossProduct2 = crossProduct(rb, ImpulseVector);
                drawArrow(Shape2->getCenter(), ContactPoint, BLUE);
                NewAngularVelocity2 = NewAngularVelocity2 * CrossProduct2;
                Result.FirstAngularResolution = -NewAngularVelocity1 * 0.01;
                Result.SecondAngularResolution = -NewAngularVelocity2 * 0.01;

//        Vector2 ra = Vector2Subtract(ContactPoint, Shape1->getCenter());
//        Vector2 rb = Vector2Subtract(ContactPoint, Shape2->getCenter());
//        Vector2 ImpulseVector = Vector2Scale(Normal, Impulse);
//
//        // Calculate torque
//        float Torque1 = crossProduct(ra, ImpulseVector);
//        float Torque2 = crossProduct(rb, ImpulseVector);
//
//        // Calculate change in angular velocities
//        float DeltaAngularVelocity1 = Torque1 / Inertia1;
//        float DeltaAngularVelocity2 = Torque2 / Inertia2;
//
//
//        Result.FirstAngularResolution = DeltaAngularVelocity1;
//        Result.SecondAngularResolution = DeltaAngularVelocity2;


        CollisionResolve PositionResolution = PolygonCollider->getCollisionResolution(Shape1->getVertices(), Shape2->getVertices());
                Result.FirstPositionResolution = PositionResolution.FirstResolution;
        Result.SecondPositionResolution = PositionResolution.SecondResolution;
        if (Shape1->isFixed())
        {
            Result.FirstVelocityResolution = {0, 0};
//            Result.SecondVelocityResolution = Vector2Add(Vector2Negate(NewVelocity1), NewVelocity2);
            Result.FirstAngularResolution = 0;
            Result.FirstPositionResolution = {0, 0};
//            return Result;
        }
        else if (Shape2->isFixed())
        {
            Result.SecondVelocityResolution = {0, 0};
//            Result.FirstVelocityResolution = Vector2Add(NewVelocity1, Vector2Negate(NewVelocity2));
            Result.SecondAngularResolution = 0;
            Result.SecondPositionResolution = {0, 0};
//            return Result;
        }
        std::cout << "Impulse: " << Impulse << std::endl;
        std::cout << "Normal: " << Normal.x << " " << Normal.y << std::endl;
        std::cout << "Old Velocity 1: " << Velocity1.x << " " << Velocity1.y << std::endl;
        std::cout << "New Velocity 1: " << Result.FirstVelocityResolution.x << " " << Result.FirstVelocityResolution.y << std::endl;
        std::cout << "Old Rotational Velocity 1: " << AngularVelocity1 << std::endl;
        std::cout << "New Rotational Velocity 1: " << Result.FirstAngularResolution << std::endl;
        std::cout << "Old Velocity 2: " << Velocity2.x << " " << Velocity2.y << std::endl;
        std::cout << "New Velocity 2: " << Result.SecondVelocityResolution.x << " " << Result.SecondVelocityResolution.y << std::endl;
        std::cout << "Old Rotational Velocity 2: " << AngularVelocity2 << std::endl;
        std::cout << "New Rotational Velocity 2: " << Result.SecondAngularResolution << std::endl;
        std::vector<LineSegment> Edges2;
        for (int i = 0; i < Shape1->getVertices().size(); ++i)
        {
            LineSegment Edge = {Shape1->getVertices()[i], Shape2->getVertices()[(i + 1) % Shape2->getVertices().size()]};
            Edges2.push_back(Edge);
        }
        if (Shape2->isFixed())
        {
                Result.FirstAngularResolution *= -2;
        }
        return Result;
    }
    else
    {
        AngularCollisionResolve Result;
        Result.FirstPositionResolution = {0, 0};
        Result.SecondPositionResolution = {0, 0};
        Result.FirstAngularResolution = Shape1->getRotationalVelocity();
        Result.SecondAngularResolution = Shape2->getRotationalVelocity();
        Result.FirstVelocityResolution = Shape1->getVelocity();
        Result.SecondVelocityResolution = Shape2->getVelocity();
        return Result;

    }
}





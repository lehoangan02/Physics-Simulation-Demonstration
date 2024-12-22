//
// Created by Le Hoang An on 16/12/24.
//
#include "SATPlatform.hpp"
#include "raymath.h"
const Color SATPlatformTriangle::Color1 = Color(100, 13, 95, 255);
const Color SATPlatformTriangle::Color2 = Color(217, 22, 86, 255);
const Color SATPlatformTriangle::Color3 = Color(231, 91, 0, 255);
SATPlatformTriangle::SATPlatformTriangle(Vector2 A, Vector2 B, Vector2 C, Color Color) : PlatformTriangle(A, B, C, Color) {
    m_A1.Position = A;
    m_B1.Position = B;
    m_C1.Position = C;
    m_A1.Angle = 0;
    m_B1.Angle = 0;
    m_C1.Angle = 0;
    sortVerticesCounterClockWise();
}
Vector2 SATPlatformTriangle::getCenter() const {
    Vector2 Center;
    Center.x = (m_A1.Position.x + m_B1.Position.x + m_C1.Position.x) / 3;
    Center.y = (m_A1.Position.y + m_B1.Position.y + m_C1.Position.y) / 3;
    return Center;
}
void SATPlatformTriangle::sortVerticesCounterClockWise() {
    Vector2 Center = getCenter();
    Vector2 Reference = Vector2Subtract(m_A1.Position, Center);
    m_A1.Angle = angle360InRadian(Reference, Reference);
    m_B1.Angle = angle360InRadian(Vector2Subtract(m_B1.Position, Center), Reference);
    m_C1.Angle = angle360InRadian(Vector2Subtract(m_C1.Position, Center), Reference);
    if (m_B1.Angle > m_C1.Angle) {
        std::swap(m_B1, m_C1);
    }
}
void SATPlatformTriangle::draw() {
//    std::cout << "Drawing\n";
    if (m_DrawInternal) {
        DrawTriangle(m_A1.Position, m_B1.Position, m_C1.Position, Color{255, 178, 0, 255});
    }
    DrawLineEx(m_A1.Position, m_B1.Position, 5, Color1);
    DrawLineEx(m_B1.Position, m_C1.Position, 5, Color2);
    DrawLineEx(m_C1.Position, m_A1.Position, 5, Color3);
    DrawCircle((int)m_A1.Position.x, (int)m_A1.Position.y, 10, Color1);
    DrawCircle((int)m_B1.Position.x, (int)m_B1.Position.y, 10, Color2);
    DrawCircle((int)m_C1.Position.x, (int)m_C1.Position.y, 10, Color3);
}
std::vector<Vector2> SATPlatformTriangle::getVertices() const {
    std::vector<Vector2> Vertices;
    Vertices.push_back(m_A1.Position);
    Vertices.push_back(m_B1.Position);
    Vertices.push_back(m_C1.Position);
    return Vertices;
}
void SATPlatformTriangle::move(Vector2 Direction) {
    m_A1.Position = Vector2Add(m_A1.Position, Direction);
    m_B1.Position = Vector2Add(m_B1.Position, Direction);
    m_C1.Position = Vector2Add(m_C1.Position, Direction);
}
void SATPlatformTriangle::rotate(float Angle) {
    Vector2 Center = getCenter();
    Vector2 DirectionA = Vector2Subtract(m_A1.Position, Center);
    Vector2 DirectionB = Vector2Subtract(m_B1.Position, Center);
    Vector2 DirectionC = Vector2Subtract(m_C1.Position, Center);
    DirectionA = Vector2Rotate(DirectionA, Angle);
    DirectionB = Vector2Rotate(DirectionB, Angle);
    DirectionC = Vector2Rotate(DirectionC, Angle);
    m_A1.Position = Vector2Add(Center, DirectionA);
    m_B1.Position = Vector2Add(Center, DirectionB);
    m_C1.Position = Vector2Add(Center, DirectionC);
}
SATPlatformPolygon::SATPlatformPolygon(const std::vector<Vector2>& Vertices, Color Color) : m_NumberOfVertices(Vertices.size()), m_DrawInternal(false) {
    for (int i = 0; i < m_NumberOfVertices; ++i) {
        Point NewPoint;
        NewPoint.Position = Vertices[i];
        NewPoint.Angle = 0;
        m_Vertices.push_back(NewPoint);
    }
    sortVerticesCounterClockWise();
}
Vector2 SATPlatformPolygon::getCenter() const {
    Vector2 Center;
    Center.x = 0;
    Center.y = 0;
    for (int i = 0; i < m_NumberOfVertices; ++i) {
        Center = Vector2Add(Center, m_Vertices[i].Position);
    }
    Center.x /= m_NumberOfVertices;
    Center.y /= m_NumberOfVertices;
    return Center;
}
void SATPlatformPolygon::sortVerticesCounterClockWise() {
    Vector2 Center = getCenter();
    Vector2 Reference = Vector2Subtract(m_Vertices[0].Position, Center);
    m_Vertices[0].Angle = angle360InRadian(Reference, Reference);
    for (int i = 1; i < m_NumberOfVertices; ++i) {
        m_Vertices[i].Angle = angle360InRadian(Vector2Subtract(m_Vertices[i].Position, Center), Reference);
    }
    for (int i = 1; i < m_NumberOfVertices; ++i) {
        for (int j = i + 1; j < m_NumberOfVertices; ++j) {
            if (m_Vertices[i].Angle > m_Vertices[j].Angle) {
                std::swap(m_Vertices[i], m_Vertices[j]);
            }
        }
    }
}
void SATPlatformPolygon::rotate(float Angle) {
    Vector2 Center = getCenter();
    for (int i = 0; i < m_NumberOfVertices; ++i) {
        Vector2 Direction = Vector2Subtract(m_Vertices[i].Position, Center);
        Direction = Vector2Rotate(Direction, Angle);
        m_Vertices[i].Position = Vector2Add(Center, Direction);
    }
}
void SATPlatformPolygon::move(Vector2 Direction) {
    for (int i = 0; i < m_NumberOfVertices; ++i) {
        m_Vertices[i].Position = Vector2Add(m_Vertices[i].Position, Direction);
    }
}
void SATPlatformPolygon::draw() {
    if (!m_UseCustomColor) {
        if (m_DrawInternal) {
            for (int i = 0; i < m_NumberOfVertices; ++i) {
                DrawTriangle(m_Vertices[0].Position, m_Vertices[i].Position,
                             m_Vertices[(i + 1) % m_NumberOfVertices].Position, m_ActiveInternalColor);
            }
        } else {
            for (int i = 0; i < m_NumberOfVertices; ++i) {
                DrawTriangle(m_Vertices[0].Position, m_Vertices[i].Position,
                             m_Vertices[(i + 1) % m_NumberOfVertices].Position, m_InactiveInternalColor);
            }
        }
        for (int i = 0; i < m_NumberOfVertices; ++i) {
            DrawLineEx(m_Vertices[i].Position, m_Vertices[(i + 1) % m_NumberOfVertices].Position, 5, m_EdgeColor);
        }
        for (int i = 0; i < m_NumberOfVertices; ++i) {
            DrawCircle((int) m_Vertices[i].Position.x, (int) m_Vertices[i].Position.y, 10, m_VertexColor);
        }
    } else {
        for (int i = 0; i < m_NumberOfVertices; ++i) {
            DrawTriangle(m_Vertices[0].Position, m_Vertices[i].Position,
                         m_Vertices[(i + 1) % m_NumberOfVertices].Position, m_CustomColor);
        }
        for (int i = 0; i < m_NumberOfVertices; ++i) {
            DrawLineEx(m_Vertices[i].Position, m_Vertices[(i + 1) % m_NumberOfVertices].Position, 6, BLACK);
        }
        for (int i = 0; i < m_NumberOfVertices; ++i) {
            DrawCircle((int) m_Vertices[i].Position.x, (int) m_Vertices[i].Position.y, 3, BLACK);
        }
    }
}
std::vector<Vector2> SATPlatformPolygon::getVertices() const {
    std::vector<Vector2> Vertices;
    for (int i = 0; i < m_NumberOfVertices; ++i) {
        Vertices.push_back(m_Vertices[i].Position);
    }
    return Vertices;
}
void SATPlatformPolygon::accelerate(Vector2 Acceleration) {
    m_Acceleration = Vector2Add(m_Acceleration, Acceleration);
}
void SATPlatformPolygon::update(float DeltaTime) {
    m_Velocity = Vector2Add(m_Velocity, Vector2Scale (m_Acceleration, DeltaTime));
    for (int i = 0; i < m_NumberOfVertices; ++i) {
        m_Vertices[i].Position = Vector2Add(m_Vertices[i].Position, Vector2Scale(m_Velocity, DeltaTime));
    }
    m_Acceleration = Vector2Scale(m_Acceleration, 0);
}
void SATPlatformPolygon::addVelocity(Vector2 Velocity) {
    m_Velocity = Vector2Add(m_Velocity, Velocity);
}
void SATPlatformPolygon::setMassUsingArea() {
    float Area = calculateAreaPolygon(this->getVertices());
    m_Mass = Area;
}
void SATPlatformPolygon::setFixed(bool Fixed) {
    m_Fixed = Fixed;
    m_Mass = INT_MAX;
}
SATPlatformCircle::SATPlatformCircle(Vector2 Position, Color Color, float Mass) : EulerianRoundBall(Position, Color, Mass) {
    m_CurrentPosition = Position;
    m_Color = Color;
    m_Radius = 10;
    m_Acceleration = Vector2{0, 0};
}
void SATPlatformCircle::draw() {
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius, BLACK);
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius - 6, m_Color);

}
void SATPlatformCircle::move(Vector2 Direction) {
    m_CurrentPosition = Vector2Add(m_CurrentPosition, Direction);
}
void SATPlatformCircle::setRadius(float Radius) {
    if (m_Radius < 0) {
        std::cerr << "Radius must be positive\n";
        m_Radius = 10;
    }
    m_Radius = Radius;
}
void SATPlatformCircle::update(float DeltaTime) {
    EulerianRoundBall::update(DeltaTime);
}
void SATPlatformCircle::accelerate(Vector2 Acceleration) {
//    print("SATPlatformCircle::accelerate\n");
//    std::cout << m_CurrentPosition.x << " " << m_CurrentPosition.y << std::endl;
    EulerianRoundBall::accelerate(Acceleration);
}
void SATPlatformCircle::addVelocity(Vector2 Velocity) {
    m_Velocity = Vector2Add(m_Velocity, Velocity);
}
void SATPlatformCircle::setVelocity(Vector2 Velocity) {
    m_Velocity = Velocity;
}
void SATPlatformCircle::setMassUsingArea() {
    m_Mass = PI * m_Radius * m_Radius;
}
void SATPlatformCircle::setFixed(bool Fixed) {
    m_Fixed = Fixed;
    m_Mass = INT_MAX;
}
SATRotatingPlatformPolygon::SATRotatingPlatformPolygon(const std::vector<Vector2>& Vertices, Color Color) : SATPlatformPolygon(Vertices, Color) {
    m_RotationalVelocity = 0;
    calculateVirtualRadius();
}
void SATRotatingPlatformPolygon::calculateVirtualRadius() {
    Vector2 Center = getCenter();
    for (int i = 0; i < this -> getVertices().size(); ++i) {
        Vector2 Direction = Vector2Subtract(this -> getVertices()[i], Center);
        float Distance = Vector2Length(Direction);
        m_VirtualRadius += Distance;
    }
    m_VirtualRadius /= this -> getVertices().size();
}
void SATRotatingPlatformPolygon::update(float DeltaTime) {


    SATPlatformPolygon::update(DeltaTime);
    float Angle = m_RotationalVelocity * DeltaTime;
    rotate(Angle);
}
float SATRotatingPlatformPolygon::calculateMomentOfInertia() const {
    float MomentOfInertia = 1/2 * this->getMass() * m_VirtualRadius * m_VirtualRadius;
    return MomentOfInertia;
}
void SATRotatingPlatformPolygon::draw() {
    SATPlatformPolygon::draw();
    // DrawCircle(getCenter().x, getCenter().y, m_VirtualRadius, BLACK);
}
SATRotatingPlatformCircle::SATRotatingPlatformCircle(Vector2 Position, Color Color, float Mass) : SATPlatformCircle(Position, Color, Mass) {
    m_RotationalVelocity = 0;
    m_Radius = 100;
    m_TopPosition = Vector2Add(m_TopPosition, Vector2{0, -m_Radius});
}
void SATRotatingPlatformCircle::update(float DeltaTime) {
    m_Velocity = Vector2Add(m_Velocity, Vector2Scale(m_Acceleration, DeltaTime));
    m_PreviousPosition = m_CurrentPosition;
    m_CurrentPosition = Vector2Add(m_CurrentPosition, Vector2Scale(m_Velocity, DeltaTime));
    m_TopPosition = Vector2Add(m_TopPosition, Vector2Scale(m_Velocity, DeltaTime));
    m_Acceleration = Vector2{0, 0};
    float Angle = m_RotationalVelocity * DeltaTime;
    rotate(Angle);
}
void SATRotatingPlatformCircle::rotate(float Radian) {
    Vector2 Center = m_CurrentPosition;
    Vector2 Direction = Vector2Subtract(m_TopPosition, Center);
    Direction = Vector2Rotate(Direction, Radian);
    m_TopPosition = Vector2Add(Center, Direction);
}
void SATRotatingPlatformCircle::setRadius(float Radius) {
    Vector2 Center = m_CurrentPosition;
    m_Radius = Radius;
    Vector2 Direction = Vector2Subtract(m_TopPosition, Center);
    Direction = Vector2Normalize(Direction);
    Direction = Vector2Scale(Direction, m_Radius);
    m_TopPosition = Vector2Add(Center, Direction);
    return;
}
void SATRotatingPlatformCircle::draw() {
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius, BLACK);
    DrawCircle(m_CurrentPosition.x, m_CurrentPosition.y, m_Radius - 6, m_Color);
    DrawLineEx(m_CurrentPosition, m_TopPosition, 5, BLACK);
    // DrawCircle(m_TopPosition.x, m_TopPosition.y, 10, BLACK);
}


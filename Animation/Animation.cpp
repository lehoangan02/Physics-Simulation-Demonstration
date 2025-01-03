//
// Created by Le Hoang An on 3/1/25.
//

#include "Animation.hpp"
#include <iostream>
Animation::Animation(Texture2D Texture, Vector2 FrameCount, float TimeStep) {
    m_Texture = Texture;
    m_FrameCount = FrameCount;
    m_TimeStep = TimeStep;
    m_CurrentFrame = {0, 0};
    m_CurrentTime = 0;
    m_SourceRect = {0, 0, m_Texture.width / m_FrameCount.x, m_Texture.height / m_FrameCount.y};
}
void Animation::update(float DeltaTime) {
    m_CurrentTime += DeltaTime;
//    std::cout << m_CurrentTime << std::endl;
//    std::cout << m_TimeStep << std::endl;
    if (m_CurrentTime >= m_TimeStep) {
        m_CurrentTime = 0;
        m_CurrentFrame.x++;
        if (m_CurrentFrame.x >= m_FrameCount.x) {
            m_CurrentFrame.x = 0;
        }
    }
    m_SourceRect.x = m_CurrentFrame.x * m_SourceRect.width;
    m_SourceRect.y = m_CurrentFrame.y * m_SourceRect.height;
}
void Animation::draw(Vector2 Position, float Scale, float Rotation) {

    Rectangle DestRect = {Position.x, Position.y, m_SourceRect.width * Scale, m_SourceRect.height * Scale};
    Vector2 Origin = {DestRect.width / 2, DestRect.height / 2};
    Origin = {Origin.x + 30, Origin.y + 10};
    DrawTexturePro(m_Texture, m_SourceRect, DestRect, Origin, Rotation, WHITE);
    Origin = (Vector2){(Origin.x * Scale)+ Position.x, (Origin.y * Scale) + Position.y};
//    DrawCircle(Origin.x * Scale  + Position.x, Origin.y * Scale + Position.y, 100, BLUE);
//    DrawCircle(Position.x, Position.y, 5, GREEN);

}
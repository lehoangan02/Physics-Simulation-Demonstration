//
// Created by Le Hoang An on 23/10/24.
//

#include "Buttons.hpp"

SoundButton *SoundButton::getSoundButton() {
    static SoundButton soundButton;
    return &soundButton;
}
SoundButton::SoundButton() {
    m_Position = {1600, 100};
    m_TextureOn = LoadTexture("Assets/Textures/SoundOn.png");
    m_TextureOff = LoadTexture("Assets/Textures/SoundOff.png");
    m_CurrentTexture = &m_TextureOn;
}
bool SoundButton::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), {m_Position.x, m_Position.y, static_cast<float>(m_CurrentTexture->width), static_cast<float>(m_CurrentTexture->height)}))
        {
//            printf("Mouse left button is pressed\n");
            switchSoundState();
            switchTexture();
            m_SoundState = !m_SoundState;
            return true;
        }
    }
    return false;
}
void SoundButton::draw() {
    DrawTexture(*m_CurrentTexture, m_Position.x, m_Position.y, WHITE);
}
void SoundButton::switchSoundState() {
    if (m_SoundState)
    {
        SoundOff::getSoundOff()->execute();
    }
    else
    {
        SoundOn::getSoundOn()->execute();
    }
}
void SoundButton::switchTexture() {
    if (m_SoundState)
    {
        m_CurrentTexture = &m_TextureOff;
    }
    else
    {
        m_CurrentTexture = &m_TextureOn;
    }

}
BackHomeButton *BackHomeButton::getBackHomeButton() {
    static BackHomeButton backHomeButton;
    return &backHomeButton;
}
BackHomeButton::BackHomeButton() {
    m_Position = {100, 100};
    m_Texture = LoadTexture("Assets/Textures/BackHome.png");
}
bool BackHomeButton::update() {
    if (!m_Active) return false;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), {m_Position.x, m_Position.y, static_cast<float>(m_Texture.width), static_cast<float>(m_Texture.height)}))
        {
//            printf("Mouse left button is pressed\n");
            BackHome::getBackHome()->execute();
            return true;
        }
    }
    return false;
}
void BackHomeButton::draw() {
    if (!m_Active) return;
    DrawTexture(m_Texture, m_Position.x, m_Position.y, WHITE);
}
TextButton::TextButton(std::string Text, Vector2 Position, Vector2 Size) {
    m_Text = Text;
    m_Position = Position;
    m_Size = Size;
}
bool TextButton::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), {m_Position.x, m_Position.y, m_Size.x, m_Size.y}))
        {
            return true;
        }
    }
    return false;
}
void TextButton::draw() {

    DrawRectangle(m_Position.x, m_Position.y, m_Size.x, m_Size.y, WHITE);
    DrawLine(m_Position.x, m_Position.y, m_Position.x + m_Size.x, m_Position.y, BLACK);
    DrawText(m_Text.c_str(), m_Position.x + 10.0f, m_Position.y + 10.0f, 20, BLACK);
    if (isMouseOver())
    {
//        std::cout << "Mouse is over\n";
        DrawRectangleLinesEx({m_Position.x, m_Position.y, m_Size.x, m_Size.y}, 2, BLACK);
        DrawRectangle(m_Position.x, m_Position.y, m_Size.x, m_Size.y, ColorAlpha(GRAY, 0.3f));
    }

}
void TextButton::scroll() {
    m_ScrollY = 0;
    if (IsKeyDown(KEY_DOWN))
    {
        m_ScrollY += 3;
    }
    if (IsKeyDown(KEY_UP))
    {
        m_ScrollY -= 3;
    }
    m_ScrollY += (GetMouseWheelMove() * 10.0f);
    m_Position.y += m_ScrollY;
}
void TextButton::setPosition(Vector2 Position) {
    m_Position = Position;
}
void TextButton::printPosition() {
    printf("Position: %f %f\n", m_Position.x, m_Position.y);
}
bool TextButton::isMouseOver() {
    return CheckCollisionPointRec(GetMousePosition(), {m_Position.x, m_Position.y, m_Size.x, m_Size.y});
}
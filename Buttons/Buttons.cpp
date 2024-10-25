//
// Created by Le Hoang An on 23/10/24.
//

#include "Buttons.hpp"

SoundButton *SoundButton::getSoundButton() {
    static SoundButton soundButton;
    return &soundButton;
}
SoundButton::SoundButton() {
    m_TextureOn = LoadTexture("Assets/Textures/SoundOn.png");
    m_TextureOff = LoadTexture("Assets/Textures/SoundOff.png");
    m_CurrentTexture = &m_TextureOn;
}
void SoundButton::update() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), {m_Position.x, m_Position.y, static_cast<float>(m_CurrentTexture->width), static_cast<float>(m_CurrentTexture->height)}))
        {
//            printf("Mouse left button is pressed\n");
            switchSoundState();
            switchTexture();
            m_SoundState = !m_SoundState;
        }
    }
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
    m_Texture = LoadTexture("Assets/Textures/BackHome.png");
}
void BackHomeButton::update() {
    if (!m_Active) return;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (CheckCollisionPointRec(GetMousePosition(), {m_Position.x, m_Position.y, static_cast<float>(m_Texture.width), static_cast<float>(m_Texture.height)}))
        {
            printf("Mouse left button is pressed\n");
            BackHome::getBackHome()->execute();
        }
    }
}
void BackHomeButton::draw() {
    if (!m_Active) return;
    DrawTexture(m_Texture, m_Position.x, m_Position.y, WHITE);
}
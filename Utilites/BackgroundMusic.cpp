//
// Created by Le Hoang An on 24/10/24.
//

#include "BackgroundMusic.hpp"
BackgroundMusic* BackgroundMusic::getBackgroundMusic()
{
    static BackgroundMusic m_BackgroundMusic;
    return &m_BackgroundMusic;
}
void BackgroundMusic::turnOn()
{
    SetMusicVolume(m_Music, 1.0f);
}
void BackgroundMusic::turnOff() {
    SetMusicVolume(m_Music, 0.0f);
}
BackgroundMusic::BackgroundMusic()
{
    InitAudioDevice();
    m_Music = LoadMusicStream("Assets/Music/BackgroundMusic.mp3");
    PlayMusicStream(m_Music);
}
BackgroundMusic::~BackgroundMusic()
{
    UnloadMusicStream(m_Music);
    CloseAudioDevice();
}
void BackgroundMusic::update()
{
    UpdateMusicStream(m_Music);
}
//
// Created by Le Hoang An on 24/10/24.
//
#include "Utilities.hpp"
void Utilities::update()
{
    if (IsKeyPressed(KEY_M))
    {
        MyBackgroundMusic -> turnOff();
    }
    if (IsKeyPressed(KEY_N))
    {
        MyBackgroundMusic -> turnOn();
    }
    MySoundButton -> update();
//    MyBackHomeButton -> update();
    BackgroundMusic::getBackgroundMusic() -> update();
}
void Utilities::draw()
{
    MySoundButton -> draw();
//    MyBackHomeButton -> draw();
}
Utilities* Utilities::getUtilities()
{
    static Utilities m_Utilities;
    return &m_Utilities;
}
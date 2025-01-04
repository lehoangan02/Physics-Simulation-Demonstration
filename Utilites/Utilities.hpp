//
// Created by Le Hoang An on 24/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_UTILITIES_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_UTILITIES_HPP
#include "raylib.h"
#include "BackgroundMusic.hpp"
#include "../Buttons/Buttons.hpp"
#include <memory>

class SoundUtilities
{
private:
    BackgroundMusic* MyBackgroundMusic= BackgroundMusic::getBackgroundMusic();
    SoundButton *MySoundButton = SoundButton::getSoundButton();
//    BackHomeButton *MyBackHomeButton = BackHomeButton::getBackHomeButton();

public:
    static SoundUtilities* getUtilities();
    void update();
    void draw();
private:
    SoundUtilities() = default;
    ~SoundUtilities() = default;
};
class StateSelector
{
private:
    StateSelector(Vector2 Position);
    ~StateSelector();
    std::vector<std::shared_ptr<TextButton>> m_Buttons;
    void scroll();
private:
    Vector2 m_Position;
    Vector2 m_Size = {800, 50 * 10};
    Vector2 m_ScollRange = {0, 900};
    std::string m_ButtonText[24];
public:
    bool m_Active = true;
public:
    static StateSelector* getStateSelector();
    void setPosition(Vector2 Position);
    int update();
    void draw();
};
class FPSShifter
{
public:
    static FPSShifter* getFPSShifter();
    void update();
private:
    FPSShifter() = default;
    ~FPSShifter() = default;
};
class ShortcutSelector
{
public:
    static ShortcutSelector* getShortcutSelector();
    int update();
private:
    ShortcutSelector() = default;
    ~ShortcutSelector() = default;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_UTILITIES_HPP

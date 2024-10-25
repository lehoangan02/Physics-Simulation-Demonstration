//
// Created by Le Hoang An on 24/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_UTILITIES_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_UTILITIES_HPP
#include "raylib.h"
#include "BackgroundMusic.hpp"
#include "../Buttons/Buttons.hpp"
class Utilities
{
private:
    BackgroundMusic* MyBackgroundMusic= BackgroundMusic::getBackgroundMusic();
    SoundButton *MySoundButton = SoundButton::getSoundButton();
//    BackHomeButton *MyBackHomeButton = BackHomeButton::getBackHomeButton();

public:
    static Utilities* getUtilities();
    void update();
    void draw();
private:
    Utilities() = default;
    ~Utilities() = default;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_UTILITIES_HPP

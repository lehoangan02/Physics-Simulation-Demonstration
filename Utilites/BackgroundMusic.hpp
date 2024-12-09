//
// Created by Le Hoang An on 24/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_BACKGROUNDMUSIC_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_BACKGROUNDMUSIC_HPP
#include "raylib.h"
class BackgroundMusic
{
private:
    Music m_Music;
public:
    static BackgroundMusic* getBackgroundMusic();
    void turnOn();
    void turnOff();
    void update();
private:
    BackgroundMusic();
    ~BackgroundMusic();
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_BACKGROUNDMUSIC_HPP

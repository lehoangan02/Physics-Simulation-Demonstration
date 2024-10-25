//
// Created by Le Hoang An on 23/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_BUTTONS_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_BUTTONS_HPP
#include <raylib.h>
#include "../Commands/Command.hpp"
#include <stdio.h>

class Button {
public:
    virtual void update() = 0;
    virtual void draw() = 0;
};
class SoundButton : public Button {
private:
    Texture2D m_TextureOn;
    Texture2D m_TextureOff;
    Texture2D* m_CurrentTexture;
    Vector2 m_Position = {1600, 100};
    bool m_SoundState = true;
public:
    static SoundButton *getSoundButton();
    void update() override;
    void draw() override;
private:
    SoundButton();
    ~SoundButton() = default;
    void switchTexture();
    void switchSoundState();
};
class BackHomeButton : public Button {
    friend class HomeState;
private:
    Texture2D m_Texture;
    Vector2 m_Position = {200, 100};
    bool m_Active = true;
public:
    static BackHomeButton *getBackHomeButton();
    void update() override;
    void draw() override;
private:
    BackHomeButton();
    ~BackHomeButton() = default;
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_BUTTONS_HPP

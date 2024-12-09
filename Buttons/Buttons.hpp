//
// Created by Le Hoang An on 23/10/24.
//

#ifndef PHYSICS_SIMULATION_DEMONSTRATION_BUTTONS_HPP
#define PHYSICS_SIMULATION_DEMONSTRATION_BUTTONS_HPP
#include <raylib.h>
#include "../Commands/Command.hpp"
#include <stdio.h>
#include <string>

class Button {
protected:
    Vector2 m_Position;
    Vector2 m_Size;
public:
    virtual bool update() = 0;
    virtual void draw() = 0;
};
class SoundButton : public Button {
private:
    Texture2D m_TextureOn;
    Texture2D m_TextureOff;
    Texture2D* m_CurrentTexture;
//    Vector2 m_Position = {1600, 100};
    bool m_SoundState = true;
public:
    static SoundButton *getSoundButton();
    bool update() override;
    void draw() override;
private:
    SoundButton();
    ~SoundButton() = default;
    void switchTexture();
    void switchSoundState();
};
class BackHomeButton : public Button {
    friend class Mediator;
private:
    Texture2D m_Texture;
//    Vector2 m_Position = {100, 100};
    bool m_Active = true;
public:
    static BackHomeButton *getBackHomeButton();
    bool update() override;
    void draw() override;
private:
    BackHomeButton();
    ~BackHomeButton() = default;
};
class TextButton : public Button {
private:
    std::string m_Text;
    float m_ScrollY = 0;
public:
    TextButton(std::string Text, Vector2 Position, Vector2 Size);
    bool update() override;
    void draw() override;
    void setPosition(Vector2 Position);
    void scroll();
    void printPosition();
private:
    bool isMouseOver();
};
#endif //PHYSICS_SIMULATION_DEMONSTRATION_BUTTONS_HPP

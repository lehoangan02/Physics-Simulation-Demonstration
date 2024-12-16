//
// Created by Le Hoang An on 24/10/24.
//
#include "Utilities.hpp"
void SoundUtilities::update()
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
void SoundUtilities::draw()
{
    MySoundButton -> draw();
//    MyBackHomeButton -> draw();
}
SoundUtilities* SoundUtilities::getUtilities()
{
    static SoundUtilities m_Utilities;
    return &m_Utilities;
}
StateSelector* StateSelector::getStateSelector()
{
    static StateSelector m_StateSelector({100, 100});
    return &m_StateSelector;
}
int StateSelector::update()
{
    if (!CheckCollisionPointRec(GetMousePosition(), {m_Position.x, m_Position.y, m_Size.x, m_Size.y}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return -1;
    }
    scroll();
    for (int i = 0; i < m_Buttons.size(); i++)
    {
        if (m_Buttons[i] -> update())
        {
            return i + 1;
        }
    }
    return -1;
}
void StateSelector::scroll() {
    static float Offset = 0;
    if (IsKeyDown(KEY_DOWN))
    {
        Offset -= 3;
    }
    if (IsKeyDown(KEY_UP))
    {
        Offset += 3;
    }
    Offset -= GetMouseWheelMove() * 10;
    for (auto &button : m_Buttons)
    {
        button -> scroll();
    }

    if (Offset < m_ScollRange.x)
    {
        Offset = m_ScollRange.x;
        for (int i = 0; i < m_Buttons.size(); i++)
        {
            Vector2 Size = {500, 50};
            m_Buttons[i] -> setPosition({m_Position.x, m_Position.y + Size.y * i});
        }
    }
    if (Offset > m_ScollRange.y)
    {
        Offset = m_ScollRange.y;
        for (int i = 0; i < m_Buttons.size(); i++)
        {
            Vector2 Size = {500, 50};
            m_Buttons[i] -> setPosition({m_Position.x, m_Position.y - m_ScollRange.y + Size.y * i});
        }
    }
}
void StateSelector::draw() {
    if (!m_Active)
    {
        return;
    }
    float Thickness = 100;
    Vector2 RectanglePosition = {m_Position.x - Thickness, m_Position.y - Thickness};
    Vector2 RectangleSize = {m_Size.x + Thickness * 2, m_Size.y + Thickness * 2};
    DrawRectangle(RectanglePosition.x, RectanglePosition.y, RectangleSize.x, RectangleSize.y, Color(229, 225, 218, 100));
    DrawRectangleLinesEx({RectanglePosition.x, RectanglePosition.y, RectangleSize.x, RectangleSize.y}, 10, Color(241, 240, 232, 150));
    BeginScissorMode(m_Position.x, m_Position.y, m_Size.x, m_Size.y);
    for (int i = 0; i < m_Buttons.size(); i++) {
        m_Buttons[i]->draw();
    }
    EndScissorMode();
}
void StateSelector::setPosition(Vector2 Position) {
    m_Position = Position;
    for (int i = 0; i < m_Buttons.size(); i++)
    {
        Vector2 Size = {500, 50};
        m_Buttons[i] -> setPosition({Position.x, Position.y + Size.y * i});
    }
}
StateSelector::StateSelector(Vector2 Position) : m_Position(Position) {
    m_ButtonText[0] = "Verlet Platform Collision";
    m_ButtonText[1] = "Verlet Chain";
    m_ButtonText[2] = "Verlet Ball Bag";
    m_ButtonText[3] = "Continuous Eulerian Integration";
    m_ButtonText[4] = "Tunneling";
    m_ButtonText[5] = "Energy Comparison";
    m_ButtonText[6] = "FPS Invariant";
    m_ButtonText[7] = "Mutual Gravity";
    m_ButtonText[8] = "Uniform Grid Optimization";
    m_ButtonText[9] = "Spring";
    m_ButtonText[10] = "Soft Body";
    m_ButtonText[11] = "Playable Jelly";
    for (int i = 0; i < 12; i++)
    {
//        std::string Text = "Button " + std::to_string(i);
        std::string Text = m_ButtonText[i];
        Vector2 Size = {800, 50};
        Vector2 ButtonPosition = {Position.x, float(Position.y + Size.y * i)};
        m_Buttons.push_back(std::make_shared<TextButton>(Text, ButtonPosition, Size));
    }
}
StateSelector::~StateSelector() {
    m_Buttons.clear();
}
FPSShifter* FPSShifter::getFPSShifter()
{
    static FPSShifter m_FPSShifter;
    return &m_FPSShifter;
}
void FPSShifter::update()
{
    if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_COMMA))
    {
        SetTargetFPS(15);
    }
    if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_PERIOD))
    {
        SetTargetFPS(60);
    }
}
ShortcutSelector* ShortcutSelector::getShortcutSelector()
{
    static ShortcutSelector m_ShortcutSelector;
    return &m_ShortcutSelector;
}
int ShortcutSelector::update()
{
    switch (GetKeyPressed())
    {
        case KEY_BACKSPACE:
            return 0;
        case KEY_A:
            return 1;
        case KEY_B:
            return 2;
        case KEY_C:
            return 3;
        case KEY_D:
            return 4;
        case KEY_E:
            return 5;
        case KEY_F:
            return 6;
        case KEY_G:
            return 7;
        case KEY_H:
            return 8;
        case KEY_I:
            return 9;
        case KEY_J:
            return 10;
        case KEY_K:
            return 11;
        case KEY_L:
            return 12;
        case KEY_N:
            return 13;
        case KEY_O:
            return 14;
        case KEY_P:
            return 16;
        case KEY_Q:
            return 17;
        case KEY_R:
            return 18;
        case KEY_S:
            return 19;
        case KEY_T:
            return 20;
        case KEY_Z:
            return 15;
        default:
            return -1;
    }

}
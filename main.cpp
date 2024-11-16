#include <iostream>
#include "Physics/RoundBall.hpp"
#include "raylib.h"
#include "Physics/Engine.hpp"
#include "States/SimulationState.hpp"
#include "Utilites/Utilities.hpp"
#include "Buttons/Buttons.hpp"
using namespace std;
int main() {
    InitWindow(1800, 1000, "Physics Simulation Demonstration");
//    SetTargetFPS(10);
    SetTargetFPS(60);
    Utilities* MyUtilities = Utilities::getUtilities();
    SimulationState* ActiveState;
    ActiveState = HomeState::getHomeState();
    Command *MyBackHome = BackHome::getBackHome();
    MyBackHome -> addObserver(ActiveState);
    while(!WindowShouldClose())
    {
        if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_COMMA))
        {
            SetTargetFPS(15);
        }
        if ((IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) && IsKeyPressed(KEY_PERIOD))
        {
            SetTargetFPS(60);
        }
        SimulationState* NextState = ActiveState->update();
        if (NextState != nullptr)
        {
            MyBackHome -> removeObserver(ActiveState);
            ActiveState -> reset();
            ActiveState = NextState;
            MyBackHome -> addObserver(ActiveState);
        }
        MyUtilities -> update();
        BackHomeButton::getBackHomeButton() -> update();
        BeginDrawing();
        ClearBackground(BLACK);
        ActiveState->draw();
        MyUtilities -> draw();
        BackHomeButton::getBackHomeButton() -> draw();
        DrawFPS(10, 10);
        EndDrawing();
    }
}

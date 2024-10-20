#include <iostream>
#include "RoundBall.hpp"
#include "raylib.h"
#include "Engine.hpp"
#include "States/SimulationState.hpp"
using namespace std;
int main() {
    InitWindow(1800, 1040, "Physics Simulation Demonstration");
    SetTargetFPS(60);
    SimulationState* ActiveState;
    VerletDropState VerletDropState;
    ActiveState = &VerletDropState;
    while(!WindowShouldClose())
    {
        ActiveState->update();
        BeginDrawing();
        ClearBackground(RAYWHITE);
        ActiveState->draw();
        DrawFPS(10, 10);
        EndDrawing();
    }
}

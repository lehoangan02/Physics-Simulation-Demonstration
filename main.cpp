#include <iostream>
#include "Physics/RoundBall.hpp"
#include "raylib.h"
#include "Physics/Engine.hpp"
#include "States/SimulationState.hpp"
#include "Utilites/Utilities.hpp"
#include "Buttons/Buttons.hpp"
#include "Utilites/Mediator.h"
using namespace std;
int main() {
    InitWindow(1800, 1000, "Physics Simulation Demonstration");
    SetTargetFPS(60);
    Mediator* MyMediator = new Mediator();
    int frameCount = 0;
    while(!WindowShouldClose())
    {
        MyMediator -> update();
        BeginDrawing();
        ClearBackground(BLACK);
        MyMediator -> draw();
        DrawFPS(10, 10);
        EndDrawing();

    }
}

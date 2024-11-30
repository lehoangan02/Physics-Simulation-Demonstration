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
//    RenderTexture2D screen = LoadRenderTexture(1800, 1000);
    while(!WindowShouldClose())
    {
        MyMediator -> update();

//        BeginTextureMode(screen);
////        BeginDrawing();
//        ClearBackground(BLACK);
//        MyMediator -> draw();
//        DrawFPS(10, 10);
////        EndDrawing();
//        EndTextureMode();
//        frameCount++;
//        Image frame = LoadImageFromTexture(screen.texture);
//        ExportImage(frame, TextFormat("frame_%05i.png", frameCount));
        BeginDrawing();
        ClearBackground(BLACK);
        MyMediator -> draw();
        DrawFPS(10, 10);
        EndDrawing();

    }
//    UnloadRenderTexture(screen);
//    CloseWindow();
//    system("ffmpeg -framerate 60 -i frame_%05d.png -c:v libx264 -pix_fmt yuv420p output.mp4");

}

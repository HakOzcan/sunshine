#include "rlImGui.h"
#include "Tilemap.h"
#include "Rat.h"
#include <time.h>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

Tilemap map;
Rat* rat;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);

    srand(time(NULL));

    map.RandomizeMap();
    Texture2D ratTexture = LoadTexture("../game/assets/textures/Rat.png");

    bool useGUI = false;
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (rat != nullptr)
        {
            rat->Update();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);


        if (IsKeyPressed(KEY_GRAVE)) useGUI = !useGUI;
        if (useGUI)
        {
            rlImGuiBegin();
            if (ImGui::Button("Randomize Map"))
            {
                map.RandomizeMap();
            }
            if (ImGui::Button("Spawn Rat"))
            {
                rat = new Rat();
                rat->SpawnRat();
            }
            rlImGuiEnd();
        }

        map.DrawTiles();
        map.DrawBorders();
        if(rat != nullptr)
        DrawTexture(ratTexture, rat->ratScreenPosX, rat->ratScreenPosY, WHITE);

        DrawFPS(10, 10);
        DrawText("Press ~ to open/close GUI", 10, 30, 20, GRAY);
        EndDrawing();
    }
    
    delete rat;
    rat = nullptr;
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
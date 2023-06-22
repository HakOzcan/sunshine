#include "rlImGui.h"
#include "Tilemap.h"
#include <time.h>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

Tilemap map;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);

    srand(time(NULL));

    map.RandomizeMap();


    bool useGUI = false;
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
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
            rlImGuiEnd();
        }

        map.DrawTiles();
        map.DrawBorders();

        DrawFPS(10, 10);
        DrawText("Press ~ to open/close GUI", 10, 30, 20, GRAY);
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
#include "rlImGui.h"
#include "Tilemap.h"
#include "Rat.h"
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
    Texture2D ratTexture = LoadTexture("../game/assets/textures/Rat.png");
    Vector2 currentRatPos = { 100,100 };
    Tilemap rat = Tilemap(currentRatPos);

    bool useGUI = false;
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        //if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        //{
        //    Vector2 currentRatPos = GetMousePosition();
        //    Tilemap rat = Tilemap(currentRatPos);
        //}

        if (IsKeyPressed(KEY_W))
        {
            Vector2 tempPos = rat.ratPos + rat.NORTH;
            //bool isInside = this->IsInsideGrid(ratPos.x, ratPos.y);
            //bool isItFloor = this->tileMap.IsItFloor(ratPos.x, ratPos.y);
            if (map.IsTraversible(tempPos))
            {
                //this->ratPos = this->ratPos + this->NORTH;
                rat.ratPos = rat.ratPos + rat.NORTH;
                rat.ratScreenPosY -= rat.tileSizeY;
            }
        }
        if (IsKeyPressed(KEY_A))
        {
            Vector2 tempPos = rat.ratPos + rat.WEST;
            //bool canRatGoW = this->tileMap.IsTraversible(ratPos);
            if (map.IsTraversible(tempPos))
            {
                //this->ratPos = this->ratPos + this->WEST;
                rat.ratPos = rat.ratPos + rat.WEST;
                rat.ratScreenPosX -= rat.tileSizeX;
            }
        }
        if (IsKeyPressed(KEY_S))
        {
            Vector2 tempPos = rat.ratPos + rat.SOUTH;
            //bool canRatGoS = this->tileMap.IsTraversible(ratPos);
            if (map.IsTraversible(tempPos))
            {
                //this->ratPos = this->ratPos + this->SOUTH;
                rat.ratPos = rat.ratPos + rat.SOUTH;
                rat.ratScreenPosY += rat.tileSizeY;
            }
        }
        if (IsKeyPressed(KEY_D))
        {
            Vector2 tempPos = rat.ratPos + rat.EAST;
            //bool canRatGoE = this->tileMap.IsTraversible(ratPos);
            if (map.IsTraversible(tempPos))
            {
                //this->ratPos = this->EAST;
                rat.ratPos = rat.ratPos + rat.EAST;
                rat.ratScreenPosX += rat.tileSizeX;
            }
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
            }
            rlImGuiEnd();
        }

        map.DrawTiles();
        map.DrawBorders();
        //if (rat != nullptr)
            DrawTexture(ratTexture, rat.ratScreenPosX, rat.ratScreenPosY, WHITE);


        DrawFPS(10, 10);
        DrawText("Press ~ to open/close GUI", 10, 30, 20, GRAY);
        EndDrawing();
    }
    
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
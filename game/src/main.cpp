//HELLO SUNSHINE
#include "rlImGui.h"
#include "Physics.h"
#include "Collision.h"
#include "Math.h"

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

//Vector2 ChangePosition(const Vector2& position1, const Vector2& position2)
//{
//    return position1 + position2;
//}

int main(void)
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Sunshine");

    float playerRotation = 0.0f;
    const float playerSpriteWidth = 32.0f;
    const float playerSpriteHeight = 32.0f;
    const float playerSpeed = 500.0f;
    const float playerRotationSpeed = 100.0f;

    const char* playerSpriteFilePath = "../game/assets/textures/New Piskel (2).png";
    Texture2D playerSprite = LoadTexture(playerSpriteFilePath);

    Vector2 playerPosition = { 0.0f, 0.0f };
    //const Vector2 playerDirection = Direction(playerRotation * DEG2RAD);
    const Rectangle playerRecSrc{ 0, 0, playerSpriteWidth, playerSpriteHeight };
    const Vector2 playerOrigin{ playerPosition.x + (playerSpriteWidth * 2), playerPosition.y + (playerSpriteHeight * 2)};

    const char* backgroundSpriteFilePath = "../game/assets/textures/Castle1.png";
    Texture2D backgroundSprite = LoadTexture(backgroundSpriteFilePath);
    const float backgroundSpriteWidth = 1280.0f;
    const float backgroundSpriteHeight = 500.0f;
    //const Vector2 backgroundPosition{ screenWidth / 2, screenHeight / 2 };
    const Vector2 backgroundOrigin{ screenWidth / 2, screenHeight / 2 };
    const Rectangle backgroundRecSrc{ 0, 0, backgroundSpriteWidth, backgroundSpriteHeight };
    const Rectangle backgroundRecDst{ screenWidth / 2, screenHeight / 2, screenWidth, screenHeight };

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        if (IsKeyDown(KEY_E))
            playerRotation += playerRotationSpeed * dt;
        if (IsKeyDown(KEY_Q))
            playerRotation -= playerRotationSpeed * dt;
        //if (IsKeyDown(KEY_D))
        //    playerPosition.x += playerSpeed * dt;
        //if (IsKeyDown(KEY_A))
        //    playerPosition.x -= playerSpeed * dt;
        //if (IsKeyDown(KEY_W))
        //    playerPosition.y -= playerSpeed * dt;
        //if (IsKeyDown(KEY_S))
        //    playerPosition.y += playerSpeed * dt;

        playerPosition = GetMousePosition();
        const Rectangle playerRecDst{ playerPosition.x, playerPosition.y, playerSpriteWidth * 4, playerSpriteHeight * 4 };
        const Vector2 playerCircleOrigin{ playerPosition.x, playerPosition.y};
        const Vector2 stationaryCircleOrigin{ 1600.0f, 200.0f };
        const float circleRadius = playerRecDst.width / 2;
        Color playerCircleColor = WHITE;
        Color stationaryCircleColor = YELLOW;
        Color backgroundColor = WHITE;

        if (CheckCollisionCircles(playerCircleOrigin, circleRadius, stationaryCircleOrigin, circleRadius))
        {
            playerCircleColor = BLACK;
            stationaryCircleColor = YELLOW;
                if (playerCircleOrigin.x == stationaryCircleOrigin.x || playerCircleOrigin.y == stationaryCircleOrigin.y)
                { 
                backgroundColor = BLACK;
                stationaryCircleColor = BLACK;
                }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexturePro(backgroundSprite, backgroundRecSrc, backgroundRecDst, backgroundOrigin, NULL, backgroundColor);
        DrawCircleV(playerCircleOrigin, circleRadius, playerCircleColor);
        DrawCircleV(stationaryCircleOrigin, circleRadius, stationaryCircleColor);
        // Render player
        DrawTexturePro(playerSprite, playerRecSrc, playerRecDst, playerOrigin, playerRotation, YELLOW);



        EndDrawing();
    }

    CloseWindow();

    return 0;
}

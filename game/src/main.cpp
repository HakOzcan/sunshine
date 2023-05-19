//HELLO SUNSHINE
#include "rlImGui.h"

#include <iostream>

using namespace std;

int main(void)
{
    //INIT//
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Sunshine");
    InitAudioDevice();//Initiliaze the audio device to be able to play sound, music etc.

    //LOAD//
    //Load Player..
    float playerRotation = 0.0f;
    const float playerSpriteWidth = 32.0f;
    const float playerSpriteHeight = 32.0f;
    const float playerRotationSpeed = 200.0f;
    Texture2D playerSprite = LoadTexture("../game/assets/textures/New Piskel (2).png");//Load 2d texture into GPU memory
    Vector2 playerPosition = { 0.0f, 0.0f };
    const Rectangle playerRecSrc{ 0, 0, playerSpriteWidth, playerSpriteHeight };
    const Vector2 playerOrigin{ playerPosition.x + (playerSpriteWidth * 2), playerPosition.y + (playerSpriteHeight * 2)};
    //Load Background..
    Texture2D backgroundSprite = LoadTexture("../game/assets/textures/Castle1.png");//Load 2d texture into GPU memory
    const float backgroundSpriteWidth = 1280.0f;
    const float backgroundSpriteHeight = 500.0f;
    const Vector2 backgroundOrigin{ screenWidth / 2, screenHeight / 2 };
    const Rectangle backgroundRecSrc{ 0, 0, backgroundSpriteWidth, backgroundSpriteHeight };
    const Rectangle backgroundRecDst{ screenWidth / 2, screenHeight / 2, screenWidth, screenHeight };
    //Load Music..
    Music mainThemeMusic = LoadMusicStream("../game/assets/audio/Main theme.mp3");//Enable us to load music file to the buffer
    Music darkThemeMusic = LoadMusicStream("../game/assets/audio/Dark theme.ogg");//Enable us to load music file to the buffer

    //SET FPS//
    SetTargetFPS(60);
    //GAME LOOP//
    while (!WindowShouldClose())
    {
        //Play and update main music..
        PlayMusicStream(mainThemeMusic);//Stream the music which was loaded into buffer before
        UpdateMusicStream(mainThemeMusic);//Update the buffer to play continuously music

        //GET DELTA TIME//
        float dt = GetFrameTime();

        //TAKE INPUT//
        if (IsKeyDown(KEY_E))//Repeatedly takes input from keyboard while key pressed and held
            playerRotation += playerRotationSpeed * dt;
        if (IsKeyDown(KEY_Q))//Repeatedly takes input from keyboard while key pressed and held
            playerRotation -= playerRotationSpeed * dt;

        //Update player position according to mouse position..
        playerPosition = GetMousePosition();//Provides mouse current position as Vector2(x and y)
        const Rectangle playerRecDst{ playerPosition.x, playerPosition.y, playerSpriteWidth * 4, playerSpriteHeight * 4 };

        //Variables related to player and stationary circle
        const Vector2 playerCircleOrigin{ playerPosition.x, playerPosition.y};
        const float circleRadius = playerRecDst.width / 2;
        const Vector2 stationaryCircleOrigin{ 1600.0f, 200.0f };
        Color playerCircleColor = WHITE;
        Color stationaryCircleColor = YELLOW;
        Color backgroundColor = WHITE;

        //COLLISION//
        if (CheckCollisionCircles(playerCircleOrigin, circleRadius, stationaryCircleOrigin, circleRadius))//Checks collision between two circles and return boolean
        {
            playerCircleColor = BLACK;
            if (playerCircleOrigin.x == stationaryCircleOrigin.x || playerCircleOrigin.y == stationaryCircleOrigin.y)//Checks if the circles completely overlap, changes music and colors accorcdingly
            {
                backgroundColor = BLACK;
                stationaryCircleColor = BLACK;
                PauseMusicStream(mainThemeMusic);//Pause the streaming music
                PlayMusicStream(darkThemeMusic);//Stream the music which was loaded into buffer before
                UpdateMusicStream(darkThemeMusic);//Update the buffer to play continuously music
            }
            else
            {
                backgroundColor = WHITE;
                stationaryCircleColor = YELLOW;
                playerCircleColor = BLACK;
                StopMusicStream(darkThemeMusic);//Stop playing music
                ResumeMusicStream(mainThemeMusic);//Resume playing the music where it left
            }

        }

        //DRAW//
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTexturePro(backgroundSprite, backgroundRecSrc, backgroundRecDst, backgroundOrigin, NULL, backgroundColor);//Draw a texture defined by a rectangle with detailed parameters
        DrawCircleV(stationaryCircleOrigin, circleRadius, stationaryCircleColor);//Draw a basic circle based on origin, circle and color
        DrawCircleV(playerCircleOrigin, circleRadius, playerCircleColor);//Draw a basic circle based on origin, circle and color
        DrawTexturePro(playerSprite, playerRecSrc, playerRecDst, playerOrigin, playerRotation, YELLOW);//Draw a texture defined by a rectangle with detailed parameters


        EndDrawing();
    }

    //CLEAN//
    UnloadTexture(playerSprite);//Unload the 2D texture from VRAM before closing the window
    UnloadTexture(backgroundSprite);//Unload the 2D texture from VRAM before closing the window
    UnloadMusicStream(darkThemeMusic);//Unload the music file from RAM before closing the window
    UnloadMusicStream(mainThemeMusic);//Unload the music file from RAM before closing the window
    CloseAudioDevice();//Close the audio device which was initiliazed at the beginning
    CloseWindow();

    return 0;
}

#include "rlImGui.h"
#include "Math.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

Vector2 Divide(float number, Vector2 vector)
{
    return { number / vector.x, number / vector.y };
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(60);

    Vector2 velocity{ 50.0f, 50.0f };
    Vector2 displacement{ 0.0f, 0.0f };
    Vector2 position{ 100.0f, 100.0f };
    Vector2 acceleration{ 1.0f, 1.0f };
    const float maxSpeed = 50.0f;

    
    float circleRadius = 50.0f;

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();

        displacement = velocity * deltaTime;
        position = position + displacement;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        ImGui::DragFloat2("position", &(position.x), 1, 0, SCREEN_WIDTH);
        ImGui::DragFloat2("velocity", &(velocity.x), 1, 0, 0);
        ImGui::DragFloat2("acceleration", &(acceleration.x), 1, 0, 0);
        rlImGuiEnd();

        DrawCircleV(position, circleRadius, BLACK);

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
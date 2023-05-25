#include "rlImGui.h"
#include "Math.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

Vector2 WraparoundScreen(Vector2 position)
{
    Vector2 outPosition =
    {
        fmodf(position.x + SCREEN_WIDTH, SCREEN_WIDTH),
        fmodf(position.y + SCREEN_HEIGHT, SCREEN_HEIGHT)
    };
    return outPosition;
}

float GetMagnitude(Vector2 vector)
{
    float magnitude = vector.x * vector.x + vector.y * vector.y;
    return sqrtf(magnitude);
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(240);

    Vector2 position{ 100.0f, 100.0f };
    Vector2 velocity{ 100.0f, 0.0f };
    Vector2 acceleration{ 0.0f, 100.0f };
    float maxSpeed = 1000.0f;
    float maxAccel = 1000.0f;
    float circleRadius = 50.0f;

    Vector2 playerPosition{ 0.0f, 0.0f };

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        playerPosition = GetMousePosition();


        Vector2 displacement = velocity * deltaTime;
        position = (position + displacement) + (acceleration * 0.5f * deltaTime * deltaTime);
        velocity = velocity + acceleration * deltaTime;

        Vector2 toPlayer = playerPosition - position;
        Vector2 unitVToPlayer = toPlayer / GetMagnitude(toPlayer);
        Vector2 desiredVel = unitVToPlayer * maxSpeed;
        Vector2 deltaVel = desiredVel - velocity;

        acceleration = unitVToPlayer * maxAccel;
        velocity = deltaVel + acceleration * deltaTime * (GetMagnitude(toPlayer) / circleRadius);

        float currentSpeed = GetMagnitude(velocity);
        if (currentSpeed > maxSpeed)
            velocity = velocity * (maxSpeed / currentSpeed);


        BeginDrawing();
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        ImGui::DragFloat2("position", &(position.x), 1, 0, SCREEN_WIDTH);
        ImGui::DragFloat2("velocity", &(velocity.x), 1, -maxSpeed, maxSpeed);
        ImGui::DragFloat2("acceleration", &(acceleration.x), 1, -maxAccel, maxAccel);
        rlImGuiEnd();

        DrawCircleV(WraparoundScreen(position), circleRadius, BLACK);
        DrawCircleV(playerPosition, circleRadius, BLUE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
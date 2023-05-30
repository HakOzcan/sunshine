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

    Vector2 agentPos{ 100.0f, 100.0f };
    Vector2 agentVel{ 100.0f, 0.0f };
    Vector2 agentAccel{ 0.0f, 100.0f };
    float maxSpeed = 2000.0f;
    float maxAccel = 2000.0f;
    float circleRadius = 50.0f;

    Vector2 playerPos{ 0.0f, 0.0f };

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        playerPos = GetMousePosition();


        Vector2 displacement = agentVel * deltaTime;
        agentPos = (agentPos + displacement) + (agentAccel * 0.5f * deltaTime * deltaTime);
        agentVel = agentVel + agentAccel * deltaTime;

        Vector2 toPlayer = agentPos - playerPos;
        Vector2 unitVToPlayer = toPlayer / GetMagnitude(toPlayer);
        Vector2 desiredVel = unitVToPlayer * maxSpeed;
        Vector2 deltaVel = desiredVel - agentVel;



        agentVel = deltaVel + agentAccel * deltaTime * (GetMagnitude(toPlayer) / circleRadius);

        float currentSpeed = GetMagnitude(agentVel);
        if (currentSpeed > maxSpeed)
            agentVel = agentVel * (maxSpeed / currentSpeed);

        Vector2 agentWrappedPos = WraparoundScreen(agentPos);


        BeginDrawing();
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        ImGui::DragFloat2("position", &(agentPos.x), 1, 0, SCREEN_WIDTH);
        ImGui::DragFloat2("velocity", &(agentVel.x), 1, -maxSpeed, maxSpeed);
        ImGui::DragFloat2("acceleration", &(agentAccel.x), 1, -maxAccel, maxAccel);
        rlImGuiEnd();

        DrawLineV(agentPos, agentVel, RED);
        DrawLineV(agentPos, agentAccel, GREEN);
        DrawCircleV(agentWrappedPos, circleRadius, BLACK);
        DrawCircleV(playerPos, circleRadius, BLUE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
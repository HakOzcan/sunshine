#include "rlImGui.h"
#include "Math.h"
#include <vector>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

Vector2 WraparoundScreen(Vector2& position)
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

class Rigidbody
{
protected:
    Vector2 objectPos;
    Vector2 objectVel{ 50.0f, 0.0f };
    Vector2 objectAccel{ 0.0f, 100.0f };
    const float kCircleRadius { 50.0f };

public:
    Rigidbody() {};

     Vector2 GetPosition() { return objectPos; }
     Vector2 GetVelocity() { return objectVel; }
     Vector2 GetAcceleration() { return objectAccel; }
     float GetCircleRadius() { return kCircleRadius; }

     void SetPosition(Vector2 pos)
     {
         objectPos = pos;
     }

     void Update(Vector2 vel, const float deltaTime)//Vector2& pos, Vector2& vel, Vector2 accel, const float deltaTime
     {
         objectPos = objectPos + (vel * deltaTime) + (objectAccel * 0.5f * deltaTime * deltaTime);
     }
};

class Agent : public Rigidbody
{
private:
    float maxSpeed{ 2000.0f };
    float maxAccel{ 2000.0f };
    Rigidbody Object;
    
public:
    Agent() {};

    Agent(float maxSpeed, float maxAccel)
    {
        this->maxSpeed = maxSpeed, this->maxAccel = maxAccel;
    }

    float GetMaxSpeed()
    {
        return maxSpeed;
    }

    static Vector2 Seek(const Vector2& playerPos, const Vector2& agentPos, const Vector2& agentVel, float maxSpeed, float maxAccel)
    {
        Vector2 toPlayer = Normalize(playerPos - agentPos);
        Vector2 desiredVel = toPlayer * maxSpeed;
        Vector2 deltaVel = desiredVel - agentVel;
        Vector2 accel = Normalize(deltaVel) * maxAccel;

        return accel;
    }

    static Vector2 Flee(const Vector2& playerPos, const Vector2& agentPos, const Vector2& agentVel, float maxSpeed, float maxAccel)
    {
        return Negate(Seek(playerPos, agentPos, agentVel, maxSpeed, maxAccel));
    }

};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    //rlImGuiSetup(true);
    SetTargetFPS(240);

    Agent a1 = Agent(3000.0f, 3000.0f);
    a1.SetPosition({ 100.0f, 100.0f });
    Agent a2 = Agent(2000.0f, 2000.0f);
    a2.SetPosition({ 200.0f, 200.0f });
    Agent a3 = Agent(1500.0f, 1500.0f);
    a3.SetPosition({ 300.0f, 300.0f });

    std::vector<Rigidbody*> pObjects;

    pObjects.push_back(&a1);
    pObjects.push_back(&a2);
    pObjects.push_back(&a3);

    Vector2 playerPos{ 0.0f, 0.0f };

    while (!WindowShouldClose())
    {
        const float deltaTime = GetFrameTime();
        playerPos = GetMousePosition();


        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            for (int i = 0; i < pObjects.size(); i++)
            {
                pObjects[i]->Update(Agent::Seek(playerPos, pObjects[i]->GetPosition(), pObjects[i]->GetVelocity(), 500.0f, 500.0f), deltaTime);
            }
        }
        else
        {
            for (int i = 0; i < pObjects.size(); i++)
            {
                pObjects[i]->Update(Agent::Flee(playerPos, pObjects[i]->GetPosition(), pObjects[i]->GetVelocity(), 500.0f, 500.0f), deltaTime);
            }
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);

        //rlImGuiBegin();
        //ImGui::DragFloat2("position", &(a1.GetPosition().x), 1, 0, SCREEN_WIDTH);
        //ImGui::DragFloat2("velocity", &(agentVel.x), 1, -maxSpeed, maxSpeed);
        //ImGui::DragFloat2("acceleration", &(agentAccel.x), 1, -maxAccel, maxAccel);
        //rlImGuiEnd();

        //DrawLineV(agentPos, agentVel, RED);
        //DrawLineV(agentPos, agentAccel, GREEN);
        for (int i = 0; i < pObjects.size(); i++)
        {
            DrawCircleV(WraparoundScreen(pObjects[i]->GetPosition()), pObjects[i]->GetCircleRadius(), RED);
        }

        DrawCircleV(playerPos, 50, BLUE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    //rlImGuiShutdown();
    CloseWindow();
    return 0;
}
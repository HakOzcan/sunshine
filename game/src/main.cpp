#include "rlImGui.h"
#include "Math.h"
#include <vector>

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

class Rigidbody
{
protected:
    Vector2 objectPos{ 100.0f, 100.0f };
    Vector2 objectVel{ 100.f, 0.0f };
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

     //static void SetVel(Rigidbody* object, Vector2 vel, float maxSpeed, const float deltaTime)
     //{
     //    object->objectVel = object->objectVel + vel * deltaTime;
     //    
     //    if (GetMagnitude(object->objectVel) > maxSpeed)
     //    object->objectVel = object->objectVel * (maxSpeed / GetMagnitude(object->objectVel));
     //}

     static void Update(Rigidbody* object, Vector2 vel, const float deltaTime)//Vector2& pos, Vector2& vel, Vector2 accel, const float deltaTime
     {
         object->objectPos = object->objectPos + (vel * deltaTime) + (object->objectAccel * 0.5f * deltaTime * deltaTime);//pos = pos + (vel * deltaTime) + (accel * 0.5f * deltaTime * deltaTime);
     }
};

class Agent : public Rigidbody
{
private:
    float maxSpeed { 2000.0f };
    float maxAccel { 2000.0f };
    
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

    static Vector2 Seek(Vector2 playerPos, Vector2 agentPos, Vector2 agentVel, float maxSpeed, float maxAccel)
    {

        //objects->GetPosition() = (objects->GetPosition() + displacement) + (objects->GetAcceleration() * 0.5f * deltaTime * deltaTime);
        //objects->GetVelocity() = objects->GetVelocity() + objects->GetAcceleration() * deltaTime;

        Vector2 toPlayer = Normalize(playerPos - agentPos);
        Vector2 desiredVel = toPlayer * maxSpeed;
        Vector2 deltaVel = desiredVel - agentVel;
        Vector2 accel = Normalize(deltaVel) * maxAccel;

        return accel;
    }
    //objects->GetVelocity() = deltaVel + objects->GetAcceleration() * deltaTime * (GetMagnitude(toPlayer) / kCircleRadius);
    static Vector2 Flee(Vector2 playerPos, Vector2 agentPos, Vector2 agentVel, float maxSpeed, float maxAccel)
    {
        return Negate(Seek(playerPos, agentPos, agentVel, maxSpeed, maxAccel));
    }

};

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    rlImGuiSetup(true);
    SetTargetFPS(240);

    Agent a1 = Agent(3000, 3000);
    a1.SetPosition({ 100.0f, 100.0f });
    Agent a2 = Agent(2000, 2000);
    a2.SetPosition({ 200.0f, 200.0f });
    Agent a3 = Agent(1500, 1500);
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

        //if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        //{
        //    for (int i = 0; i < pObjects.size(); i++)
        //    {
        //        Rigidbody::SetVel(pObjects[i], Agent::Seek(playerPos, pObjects[i]->GetPosition(), pObjects[i]->GetVelocity(), 1500.0f, 1500.0f), 1500.0f, deltaTime);
        //        //pObjects[i].Update(pObjects[i].GetPosition(), pObjects[i].GetVelocity(), pObjects[i].Seek(playerPos, pObjects[i].GetPosition(), pObjects[i].GetVelocity(), 2000.0f), deltaTime);
        //    }
        //}

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            for (int i = 0; i < pObjects.size(); i++)
            {
                Rigidbody::Update(pObjects[i], Agent::Seek(playerPos, pObjects[i]->GetPosition(), pObjects[i]->GetVelocity(), 500.0f, 500.0f), deltaTime);
                //pObjects[i].Update(pObjects[i].GetPosition(), pObjects[i].GetVelocity(), pObjects[i].Seek(playerPos, pObjects[i].GetPosition(), pObjects[i].GetVelocity(), 2000.0f), deltaTime);
            }
        }
        else
        {
            for (int i = 0; i < pObjects.size(); i++)
            {
                Rigidbody::Update(pObjects[i], Agent::Flee(playerPos, pObjects[i]->GetPosition(), pObjects[i]->GetVelocity(), 500.0f, 500.0f), deltaTime);
                //pObjects[i].Update(pObjects[i].GetPosition(), pObjects[i].GetVelocity(), pObjects[i].Seek(playerPos, pObjects[i].GetPosition(), pObjects[i].GetVelocity(), 2000.0f), deltaTime);
            }
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        //ImGui::DragFloat2("position", &(a1.GetPosition().x), 1, 0, SCREEN_WIDTH);
        //ImGui::DragFloat2("velocity", &(agentVel.x), 1, -maxSpeed, maxSpeed);
        //ImGui::DragFloat2("acceleration", &(agentAccel.x), 1, -maxAccel, maxAccel);
        rlImGuiEnd();

        //DrawLineV(agentPos, agentVel, RED);
        //DrawLineV(agentPos, agentAccel, GREEN);
        for (int i = 0; i < pObjects.size(); i++)
        {
            Vector2 drawPosition = WraparoundScreen(pObjects[i]->GetPosition());
            DrawCircleV(drawPosition, pObjects[i]->GetCircleRadius(), RED);
        }

        DrawCircleV(playerPos, 50, BLUE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
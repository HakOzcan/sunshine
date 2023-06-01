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
public:
    Vector2 objectPos{ 0.0f, 0.0f };
    Vector2 objectVel{ 50.0f, 0.0f };
    Vector2 objectAccel{ 0.0f, 100.0f };
    const float kCircleRadius { 50.0f };

public:
    Rigidbody() {};

    Rigidbody(Vector2 objectPos, Vector2 objectVel, Vector2 objectAccel)
    {
        this->objectPos = objectPos, this->objectVel = objectVel, this->objectAccel = objectAccel; 
    }

    void SetPosition(Vector2 pos)
    {
        objectPos = pos;
    }

    void SetPositionX(float posx)
    {
        objectPos.x = posx;
    }
    void SetPositionY(float posy)
    {
        objectPos.y = posy;
    }


     Vector2 GetPosition() { return objectPos; }
     Vector2 GetVelocity() { return objectVel; }
     Vector2 GetAcceleration() { return objectAccel; }
     float GetCircleRadius() { return kCircleRadius; }

     void Update(const float deltaTime)//Vector2& pos, Vector2& vel, Vector2 accel, const float deltaTime
     {
         objectPos = objectPos + (objectVel * deltaTime) + (objectAccel * 0.5f * deltaTime * deltaTime);

         objectVel = objectVel + (objectAccel * deltaTime);

         objectAccel = { 0 };
     }
};

class Agent
{
public:
    float maxSpeed = 2000.0f;
    float maxAccel = 2000.0f;
    
public:
    Rigidbody object;

    Agent() {};

    Agent(float maxSpeed, float maxAccel)
    {
        this->maxSpeed = maxSpeed, this->maxAccel = maxAccel;
    }

    float GetMaxSpeed()
    {
        return maxSpeed;
    }
    float GetMaxAccel()
    {
        return maxAccel;
    }
};

Vector2 Seek(const Vector2& targetPos, const Vector2& agentPos, Vector2 agentVel, float maxSpeed, float maxAccel)
{
    Vector2 toPlayer = Normalize(targetPos - agentPos);
    Vector2 desiredVel = toPlayer * maxSpeed;
    Vector2 deltaVel = desiredVel - agentVel;
    Vector2 accel = Normalize(deltaVel) * maxAccel;
    //agentVel = agentVel + accel;

    return accel;
}

Vector2 Flee(const Vector2& targetPos, const Vector2& agentPos, Vector2 agentVel, float maxSpeed, float maxAccel)
{
    Vector2 toPlayer = Normalize(agentPos - targetPos);
    Vector2 desiredVel = toPlayer * maxSpeed;
    Vector2 deltaVel = desiredVel - agentVel;
    Vector2 accel = Normalize(deltaVel) * maxAccel;
    //agentVel = agentVel + accel;

    return accel;
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sunshine");
    //rlImGuiSetup(true);
    SetTargetFPS(240);

    Agent a1 = Agent(1500.0f, 1500.0f);
    a1.object.SetPosition({ 100.0f, 100.0f });
    Agent a2 = Agent(1250.0f, 1250.0f);
    a2.object.SetPosition({ 200.0f, 200.0f });
    Agent a3 = Agent(1000.0f, 1000.0f);
    a3.object.SetPosition({ 300.0f, 300.0f });

    std::vector<Agent> agents;
    
    agents.push_back(a1);
    agents.push_back(a2);
    agents.push_back(a3);

    std::vector<Rigidbody> pObjects;

    pObjects.push_back(a1.object);
    pObjects.push_back(a2.object);
    pObjects.push_back(a3.object);

    Rigidbody o1 = Rigidbody({ 800.0f, 800.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f });
    Rigidbody o2 = Rigidbody({ 1000.0f, 800.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f });
    Rigidbody o3 = Rigidbody({ 1500.0f, 800.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f });

    std::vector<Rigidbody> obstacles;

    obstacles.push_back(o1);
    obstacles.push_back(o2);
    obstacles.push_back(o3);


    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        Vector2 playerPos = GetMousePosition();

        for (int i = 0; i < pObjects.size(); i++)
        {
         pObjects[i].objectAccel = Flee(obstacles[i].GetPosition(), pObjects[i].GetPosition(), pObjects[i].GetVelocity(), agents[i].maxSpeed * 0.5f, agents[i].maxAccel * 0.5f);
         if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
         {
             //Seek(playerPos, pObjects[i].GetPosition(), pObjects[i].GetVelocity(), agents[i].maxSpeed, agents[i].maxAccel);
             pObjects[i].objectAccel = pObjects[i].objectAccel + Seek(playerPos, pObjects[i].GetPosition(), pObjects[i].GetVelocity(), agents[i].maxSpeed, agents[i].maxAccel);
         }
        }
        

        for (int i = 0; i < pObjects.size(); i++)
        {
            pObjects[i].Update(deltaTime);
            WraparoundScreen(pObjects[i].objectPos);
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
            DrawCircleV(WraparoundScreen(pObjects[i].GetPosition()), pObjects[i].GetCircleRadius(), RED);
            DrawCircleV(obstacles[i].GetPosition(), obstacles[i].GetCircleRadius(), YELLOW);
        }

        DrawCircleV(playerPos, 50, BLUE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    //rlImGuiShutdown();
    CloseWindow();
    return 0;
}
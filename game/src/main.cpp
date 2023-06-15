#include "rlImGui.h"
#include "Math.h"
#include "Collision.h"
#include <vector>
#include <time.h>

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

class Rigidbody
{
public:
    Vector2 objectPos{ 0.0f, 0.0f };
    Vector2 objectVel{ 50.0f, 0.0f };
    Vector2 objectAccel{ 0.0f, 100.0f };
    const float kCircleRadius { 50.0f };
    float rotation{ 0.0f };
    float rotationSpeed = 100.0f;
    static const int s_kWhiskerCount = 4;
    bool detection[s_kWhiskerCount] {};
    float whiskerLength = 250.0f;
    Vector2 whiskers[s_kWhiskerCount] = { {0,0},{0,0} ,{0,0} ,{0,0} };


public:
    Rigidbody() {};

    Rigidbody(Vector2 position, Vector2 velocity, Vector2 acceleration) 
        : objectPos(position)
        , objectVel(velocity)
        , objectAccel(acceleration)
    {

    }
};

class Agent
{
public:
    float maxSpeed = 1000.0f;
    float maxAccel = 1000.0f;


public:
    Rigidbody object;

    Agent() {};

    Agent(float speed, float acceleration)
        : maxSpeed(speed)
        , maxAccel(acceleration)
    {
    }

    void Update(const float deltaTime)
    {

        object.rotation = AngleFromVector();

        UpdateWhiskers();

        if (Length(object.objectVel) > this->maxSpeed)
            object.objectVel = object.objectVel * (this->maxSpeed / Length(object.objectVel));


        object.objectVel = object.objectVel + (object.objectAccel * deltaTime);
        
        object.objectPos = object.objectPos + (object.objectVel * deltaTime) + (object.objectAccel * 0.5f * deltaTime * deltaTime);

        object.objectPos = WraparoundScreen(object.objectPos);


        object.objectAccel = { 0,0 };
    }

    float AngleFromVector()
    {
        Vector2 normVector = Normalize(object.objectVel);
        float angleInDegrees = atan2f(normVector.y, normVector.x) * RAD2DEG;
        angleInDegrees = fmodf(angleInDegrees + 360, 360);

        return angleInDegrees;
    }

    Vector2 VectorFromAngleDegrees(float angleInDegrees)
    {
        return Vector2{ cosf(angleInDegrees * DEG2RAD), sinf(angleInDegrees * DEG2RAD)};
    }

    void UpdateWhiskers()
    {
        float whiskerFL = fmodf(object.rotation - 30.0f + 360.0f, 360.0f);
        float whiskerL = fmodf(object.rotation - 15.0f + 360.0f, 360.0f);
        float whiskerR = fmodf(object.rotation + 15.0f + 360.0f, 360.0f);
        float whiskerFR = fmodf(object.rotation + 30.0f + 360.0f, 360.0f);


        object.whiskers[0] = VectorFromAngleDegrees(whiskerFL) * object.whiskerLength;
        object.whiskers[1] = VectorFromAngleDegrees(whiskerL) * object.whiskerLength;
        object.whiskers[2] = VectorFromAngleDegrees(whiskerR) * object.whiskerLength;
        object.whiskers[3] = VectorFromAngleDegrees(whiskerFR) * object.whiskerLength;
    }

    bool IsWhiskerColliding(int index, Vector2 circlePosition, float circleRadius)
    {
        Vector2 nearest = NearestPoint(object.objectPos, object.objectPos + object.whiskers[index], circlePosition);
        object.detection[index] = DistanceSqr(nearest, circlePosition) <= circleRadius * circleRadius;
        
        return object.detection[index];
    }

    void RotateAgent(Vector2 targetPos, float deltaTime)
    {
        for (int i = 0; i < object.s_kWhiskerCount; i++)
        {
            this->IsWhiskerColliding(i, targetPos, object.kCircleRadius);
        }

        if (object.detection[0] || object.detection[1])
        {
            object.objectVel = Rotate(object.objectVel, object.rotationSpeed * deltaTime * DEG2RAD);
        }
        if (object.detection[2] || object.detection[3])
        {
            object.objectVel = Rotate(object.objectVel, -object.rotationSpeed * deltaTime * DEG2RAD);
        }
    }
};

Vector2 Seek(const Vector2& targetPos, const Vector2& agentPos, Vector2 agentVel, float maxSpeed, float maxAccel)
{
    Vector2 toPlayer = targetPos - agentPos;
    Vector2 normToPlayer = Normalize(toPlayer);
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

    srand((unsigned)time(NULL));

    std::vector<Agent*> agents;
    std::vector<Rigidbody*> obstacles;
    
    for (int i = 0; i < 3; i++)
    {
        agents.push_back(new Agent(static_cast<float>(GetRandomValue(500, 750)), static_cast<float>(GetRandomValue(500, 750))));
        agents[i]->object.objectPos = { static_cast<float>(GetRandomValue(500, 1000)) , static_cast<float>(GetRandomValue(500, 1000)) };
    }



    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        Vector2 playerPos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            obstacles.push_back(new Rigidbody({ playerPos.x, playerPos.y }, { 0,0 }, { 0,0 }));
        }
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            for (Agent* agent : agents)
                agent->object.objectAccel = Seek(playerPos, agent->object.objectPos, agent->object.objectVel, agent->maxSpeed, agent->maxAccel);
        }

        for (Agent* agent : agents)
        {
            agent->Update(deltaTime);
        }
       
        if (obstacles.capacity() != 0)
        {
            for (Agent* agent : agents)
            {
                for (Rigidbody* obstacle : obstacles)
                {
                    agent->RotateAgent(obstacle->objectPos, deltaTime);
                }
            }
        }



        BeginDrawing();
        ClearBackground(RAYWHITE);

        //rlImGuiBegin();
        //ImGui::DragFloat2("position", &(a1.GetPosition().x), 1, 0, SCREEN_WIDTH);
        //ImGui::DragFloat2("velocity", &(agentVel.x), 1, -maxSpeed, maxSpeed);
        //ImGui::DragFloat2("acceleration", &(agentAccel.x), 1, -maxAccel, maxAccel);
        //rlImGuiEnd();

        for (Agent* agent : agents)
        {
            DrawCircleV(agent->object.objectPos, agent->object.kCircleRadius, RED);
        }

        if (obstacles.capacity() != 0)
        {
            for (Rigidbody* obstacle : obstacles)
            {
                DrawCircleV(obstacle->objectPos, obstacle->kCircleRadius, YELLOW);
            }
        }

        for (Agent* agent : agents)
        {
            for (int x = 0; x < agent->object.s_kWhiskerCount; x++)
            {
                DrawLineV(agent->object.objectPos, agent->object.objectPos + agent->object.whiskers[x], (agent->object.detection[x]) ? RED : GREEN);
            }
        }

        DrawCircleV(playerPos, 50, BLUE);

        DrawFPS(10, 10);

        EndDrawing();
    }

    //rlImGuiShutdown();
    CloseWindow();
    return 0;
}
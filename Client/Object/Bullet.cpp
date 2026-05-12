#include "pch.h"
#include "Bullet.h"
#include "Component/MeshComponent.h"
#include "Component/MovementComponent.h"

#include "Core/TimeManager.h"
#include "World/Level.h"

bool Bullet::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    //Ptr<MeshComponent> meshComp = CreateSceneComponent<MeshComponent>("Mesh");
    //meshComp->SetMesh("Triangle");
    //meshComp->SetShader("ColorMeshShader");

    //SetRootComponent(meshComp);

    ////생기고나서 10초 후에 자동으로 삭제
    //_timerID = TimeManager::Instance().SetTimer(10.f, false, this, &Bullet::RemoveBullet);

    //Ptr<MovementComponent> movement = CreateActorComponent<MovementComponent>("Movement");
    //
    //movement->SetUpdateComponent(_root);

    return true;
}

void Bullet::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Bullet::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void Bullet::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Bullet::Destroy()
{
    TimeManager::Instance().RemoveTimer(_timerID);
}

void Bullet::SetDir(const FVector3D& dir)
{
    //
    Ptr<MovementComponent> foundComp = FindComponent<MovementComponent>("Movement");
    if (nullptr == foundComp)
        return;

    foundComp->SetMoveAxis(dir);
}

void Bullet::SetSpeed(const float speed)
{
    Ptr<MovementComponent> foundComp = FindComponent<MovementComponent>("Movement");
    if (nullptr == foundComp)
        return;

    foundComp->SetSpeed(speed);
}

void Bullet::RemoveBullet()
{
    Ptr<Level> level = GetLevel();
    if (level)
        level->RemoveActor(GetActorID());
}

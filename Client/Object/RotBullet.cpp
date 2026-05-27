#include "pch.h"
#include "RotBullet.h"
#include "Component/MeshComponent.h"
#include "Component/MovementComponent.h"

#include "Core/TimeManager.h"
#include "World/Level.h"

bool RotBullet::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

      //생기고나서 10초 후에 자동으로 삭제
    _timerID = TimeManager::Instance().SetTimer(10.f, false, this, &RotBullet::RemoveBullet);

    Ptr<MovementComponent> movement = CreateActorComponent<MovementComponent>("Movement");

    movement->SetUpdateComponent(_root);

    return true;
}

void RotBullet::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);

    FRotator rot = GetRelativeRotation();
    _root->SetRelativeRotation(rot._x, rot._y, rot._z + (90.f * deltaTime));
}


void RotBullet::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void RotBullet::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void RotBullet::Destroy()
{
    TimeManager::Instance().RemoveTimer(_timerID);
}

void RotBullet::SetDir(const FVector3D& dir)
{
    //
    Ptr<MovementComponent> foundComp = FindComponent<MovementComponent>("Movement");
    if (nullptr == foundComp)
        return;

    foundComp->SetMoveAxis(dir);
}

void RotBullet::SetSpeed(const float speed)
{
    Ptr<MovementComponent> foundComp = FindComponent<MovementComponent>("Movement");
    if (nullptr == foundComp)
        return;

    foundComp->SetSpeed(speed);
}

void RotBullet::RemoveBullet()
{
}

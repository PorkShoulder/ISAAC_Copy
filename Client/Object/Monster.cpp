#include "pch.h"
#include "Monster.h"
//#include "Bullet.h"
//#include "RotBullet.h"
#include "Player.h"

#include "../Core/TimeManager.h"
#include "../Component/MeshComponent.h"
//#include "Component/AABBCollisionComponent.h"
//#include "Component/SphereCollisionComponent.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/AIComponent.h"
//#include "Component/ApeachMachine.h"
#include "../Component/WidgetComponent.h"

#include "World/Level.h"

#include "Common/LogManager.h"

#include "Controller/AIController.h"


bool Monster::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    // SpriteComponent를 생성해서 RootComponent로 설정함 

    Pawn::Init(id, pos, scale, rot);

    Ptr<SpriteComponent> monster = CreateSceneComponent<SpriteComponent>("Monster_1");
    SetRootComponent(monster);
    
    return true;


}

void Monster::Tick(float deltaTime)
{
    Pawn::Tick(deltaTime);
}

void Monster::Collision(float deltaTime)
{
    Pawn::Collision(deltaTime);
}

void Monster::Render(float deltaTime)
{
    Pawn::Render(deltaTime);
}

void Monster::Destroy()
{
    Pawn::Destroy();

    TimeManager::Instance().RemoveTimer(_bulletTimer);
    TimeManager::Instance().RemoveTimer(_rotBulletTimer);
}

void Monster::SetTarget(Ptr<class Player> player)
{
}

void Monster::Fire()
{
}

void Monster::RotBulletFire()
{
}



void Monster::BlockCallback(Weak<class CollisionComponent> comp)
{
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    level->RemoveActor(_id);
}

bool Monster::IsCheck()
{
    Ptr<Actor> player = Lock<Player>(_target);
    if (!player)
        return false;

    float dist = player->GetWorldPosition().Distance(GetWorldPosition());
    if (dist <= 5)
        return true;
    else
        return false;
}

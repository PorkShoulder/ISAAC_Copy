#include "pch.h"
#include "Monster.h"
#include "Bullet.h"
#include "RotBullet.h"
#include "Player.h"

#include "Core/TimeManager.h"
#include "Component/MeshComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SphereCollisionComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/AIComponent.h"

#include "Component/WidgetComponent.h"

#include "World/Level.h"

#include "Common/LogManager.h"

#include "Controller/AIController.h"

//#include "World/Level.h"


bool Monster::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Pawn::Init(id, pos, scale, rot, name);

    //AI State Machine 사용방법
    // 
    //1. AI컨트롤러를 생성(PlayerController로)
    _controller = GetLevel()->SpawnActor<AIController>("AIController", pos, scale, rot);

    //2. AI컨트롤러가 조종하는 대상을 지정
    _controller->SetPawn(This<Monster>());

    //3. AI컨트롤러로부터 AIComponent를 가져온다.(AIComponent는 AIController를 생성하면 자동으로 생성)
    Ptr<AIController> ctrl = Cast<Controller, AIController>(_controller);
    Ptr<AIComponent> aicomp = ctrl->GetAI();

    //4. MachineBase를 상속받은 스테이트머신 자식클래스로 스테이트머신을 생성
    
    Ptr<SpriteComponent> meshComp = CreateSceneComponent<SpriteComponent>("Mesh");

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
    _target = player;
}

void Monster::Fire()
{
    //총알 생성 및 총알 나가야 할 위치를 지정해준다.
    //속도랑
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    LogManager::Instance().Debug("Monster Fire..!");

    Ptr<Bullet> bullet = level->SpawnActor<Bullet>("bullet", GetWorldPosition(), GetWorldScale() / 2, GetWorldRotation());

    //Ptr<Player> player = level->FindActor()
    FVector3D targetPos = Lock<Player>(_target)->GetWorldPosition();
    FVector3D pos = GetWorldPosition();

    bullet->SetDir(targetPos - pos);
    bullet->SetSpeed(15);
}

void Monster::RotBulletFire()
{
    //총알 생성 및 총알 나가야 할 위치를 지정해준다.
   //속도랑
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    //LogManager::Instance().Debug("Monster Fire..!");

    Ptr<RotBullet> bullet = level->SpawnActor<RotBullet>("bullet", GetWorldPosition(), GetWorldScale() / 2, GetWorldRotation());

    //Ptr<Player> player = level->FindActor()
    FVector3D targetPos = Lock<Player>(_target)->GetWorldPosition();
    FVector3D pos = GetWorldPosition();

    bullet->SetDir(targetPos - pos);
    bullet->SetSpeed(10);
}


void Monster::BlockCallback(Weak<CollisionComponent> comp)
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


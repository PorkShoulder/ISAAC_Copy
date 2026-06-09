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
    _type = eActorType::Monster;

    

    //AI State Machine 사용방법
    //1. AI컨트롤러를 생성(PlayerController로)
    _controller = GetLevel()->SpawnActor<AIController>("AIController", pos, scale, rot);

    //2. AI컨트롤러가 조종하는 대상을 지정
    _controller->SetPawn(This<Monster>());

    //3. AI컨트롤러로부터 AIComponent를 가져온다.(AIComponent는 AIController를 생성하면 자동으로 생성)
    Ptr<AIController> ctrl = Cast<Controller, AIController>(_controller);
    Ptr<AIComponent> aicomp = ctrl->GetAI();

    //4. MachineBase를 상속받은 스테이트머신 자식클래스로 스테이트머신을 생성
    _monsterMesh= CreateSceneComponent<SpriteComponent>("Mesh");
    SetRootComponent(_monsterMesh);

    // 1. 피격/접촉용 충돌체
    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(32.f, 32.f);   // 에디터에서 추가 설정변경 -> 동적(RoomObjectUI)
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Monster");
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Monster::OnHit); 
    
    // 2. 감지용 충돌체 
    _detectCol = CreateSceneComponent<SphereCollisionComponent>("Detect");
    _detectCol->SetRadius(_monsterData.detectRange);
    _detectCol->AttachToComponent(_root);
    _detectCol->SetCollisionProfile("Monster");
    _detectCol->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Monster::OnDetectPlayer);
    return true;
}

void Monster::Tick(float deltaTime)
{
    Pawn::Tick(deltaTime);
    UpdataeAnimation();
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

void Monster::SetMonsterData(const FMonsterData& data)
{
    
    _monsterData = data;
    SetMaxHp((int32)data.hp);

    if (_col)
        _col->SetBoxSize(data.collisionSize._x, data.collisionSize._y);

    if (_detectCol)
        _detectCol->SetRadius(data.detectRange);

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

void Monster::UpdataeAnimation()
{
    // 스프라이트가 없으면 리턴함.
    if (!_monsterMesh)
        return;
    // 타겟(플레이어) 가져오기 
    Ptr<Player> player = Lock<Player>(_target);
    if (!player)
    {
        // 플레이어가 없ㅇ므ㅕㄴ 대기 애니메이션
        _monsterMesh->ChangeAnimation("IDLE");
        return;
    }

    //플레이어 위치 - 몬스터 위치 = 방향 벡터.
    FVector3D dir = player->GetWorldPosition() - GetWorldPosition();
    
    // X차이가 Y보다 크다면 좌우 이동 
    if (abs(dir._x) > abs(dir._y))
    {
        _monsterMesh->ChangeAnimation("MOVE_SIDE");
        // dir._x < 0 -> 플레이어가 왼쪽 -> 스프라이트 좌우 반전.
        _monsterMesh->SetAnimFilp(dir._x < 0.f);
    }
    // y차이가 더 크면 정면/뒷면 이동 애니메이션
    else
    {
        _monsterMesh->ChangeAnimation("MOVE_FRONT");
        _monsterMesh->SetAnimFilp(dir._y > 0.f);
    }
}

void Monster::OnDetectPlayer(Weak<class CollisionComponent> dest)
{
    Ptr<CollisionComponent> col = Lock<CollisionComponent>(dest);
    if (!col) return;

    Ptr<Actor> owner = col->GetOwner();
    if (!owner) return;

    Ptr<Player> player = Cast<Actor, Player>(owner);
    if (!player) return;

    _target = player;
    _isChasing = true;
}

void Monster::OnHit(Weak<class CollisionComponent> dest)
{
    Ptr<CollisionComponent> col = Lock<CollisionComponent>(dest);
    if (!col) return;

    Ptr<Actor> owner = col->GetOwner();
    if (!owner) return;

    // 플레이어 총알에 맞았을 때
    Ptr<Bullet> bullet = Cast<Actor, Bullet>(owner);
    if (bullet)
    {
        TakeDamage(1);
        if (IsDead())
            OnDeath();
        
    }
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


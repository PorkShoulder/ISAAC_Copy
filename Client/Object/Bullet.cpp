#include "pch.h"
#include "Bullet.h"
#include "../Component/SpriteComponent.h"
#include "../Component/AABBCollisionComponent.h"
#include "../Component/TileComponent.h"
#include "Component/MeshComponent.h"
#include "Component/MovementComponent.h"

#include "../Common/LogManager.h"
#include "TileMap.h"

#include "../Object/Pawn.h"

#include "Core/TimeManager.h"
#include "World/Level.h"

bool Bullet::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);
    
   
    // 스프라이트 (멤버 _mesh에 저장)
    _mesh = CreateSceneComponent<SpriteComponent>("Mesh");
    _mesh->AddAnimSequence("Tears", true); 
    SetRootComponent(_mesh);
    _mesh->SetRelativeScale(64.f, 64.f, 1.f);

    // 무브먼트 (SetDir/SetSpeed가 찾는 컴포넌트)
    Ptr<MovementComponent> movement = CreateActorComponent<MovementComponent>("Movement");
    movement->SetUpdateComponent(_root);

    // 충돌체 + 프로파일 + 피격 콜백
    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(16.f, 16.f);
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("MonsterBullet");
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Bullet::OnHit);

    SetBulletType(eBulletType::Normal);   // 기본 외형

    _timerID = TimeManager::Instance().SetTimer(10.f, false, this, &Bullet::RemoveBullet);

    return true;
}

void Bullet::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
    if (_bulletImpacted)
        return;

    Ptr<MovementComponent> mv = FindComponent<MovementComponent>("Movement");
    if (mv && mv->IsBlockedThisFrame())
    {
        BulletImpact();
        return;
    }
    
    Ptr<Level> level = GetLevel();
    if (!level) return;                 // ← 이 줄은 유지 (안전)

    Ptr<TileMap> tileMap = level->GetTileMap();
    if (!tileMap) { LogManager::Instance().Debug("BULLET: tileMap NULL"); return; }

    Ptr<TileComponent> tileComp = tileMap->GetTileComponent();
    if (tileComp && tileComp->IsBlocked(GetWorldPosition()))
    {
        BulletImpact();
    }

    if (_homing)
        UpdateHoming(deltaTime);
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
    Actor::Destroy();
    TimeManager::Instance().RemoveTimer(_timerID);
}

void Bullet::SetDir(const FVector3D& dir)
{
    _moveDir = dir;
    Ptr<MovementComponent> foundComp = FindComponent<MovementComponent>("Movement");
    if (nullptr == foundComp)
        return;

    foundComp->SetMoveAxis(dir);
}

void Bullet::SetSpeed(const float speed)
{
    _maxSpeed = speed;
    _currentSpeed = speed;

    Ptr<MovementComponent> foundComp = FindComponent<MovementComponent>("Movement");
    if (nullptr == foundComp)
        return;

    foundComp->SetSpeed(speed);
}

void Bullet::UpdateHoming(float deltaTime)
{
    Ptr<Level> level = GetLevel();
    if (!level) return;

    float nearestDist = 999999.f;
    FVector3D nearestPos;
    bool found = false;

    for (auto& pair : level->GetActors())
    {
        Ptr<Actor> actor = pair.second;
        if (!actor || !actor->IsActive()) continue;
        if (actor->GetActorType() != eActorType::Monster) continue;

        FVector3D diff = actor->GetWorldPosition() - GetWorldPosition();
        float dist = diff._x * diff._x + diff._y * diff._y;
        if (dist < nearestDist)
        {
            nearestDist = dist;
            nearestPos = actor->GetWorldPosition();
            found = true;
        }
    }
    
    if (found)
    {
        FVector3D toTarget = nearestPos - GetWorldPosition();
        toTarget.Normalize();

        // 방향 차이 → 급커브일수록 감속
        float dot = _moveDir._x * toTarget._x + _moveDir._y * toTarget._y;
        float turnFactor = (dot + 1.f) * 0.5f;
        _currentSpeed = _maxSpeed * (0.4f + 0.6f * turnFactor);

        // 느리게 회전 (관성 — 값 낮을수록 드리프트 강함)
        _moveDir = _moveDir + (toTarget - _moveDir) * deltaTime * 2.f;
    }

    _moveDir.Normalize();

    Ptr<MovementComponent> mv = FindComponent<MovementComponent>("Movement");
    if (mv)
    {
        mv->SetMoveAxis(_moveDir);
        mv->SetSpeed(_currentSpeed);
    }
}

void Bullet::SetAnimation(const std::string& animName)
{
    Ptr<SpriteComponent> sprite = FindSceneComponent<SpriteComponent>("Mesh");
    if (sprite)
    {
        sprite->ChangeAnimation(animName);
        sprite->SetPlay(animName, true);
    }
}

void Bullet::SetBulletType(eBulletType type)
{
    _type = type;

    if (!_mesh)
        return;

    switch (type)
    {
    case eBulletType::Normal:
        _mesh->ChangeAnimation("Tears");
        _mesh->SetPlay("Tears", false);
        break;
        // case eBulletType::Fire: ... (추가될 2종)
    default: break;
    }

}

void Bullet::SetProfile(const std::string& name)
{
    if (_col) _col->SetCollisionProfile(name);
}

void Bullet::RemoveBullet()
{
    if (_removeRequested)
        return;
    _removeRequested = true;

    Ptr<Level> level = GetLevel();
    if (level)
        level->RemoveActor(GetActorID());
}

void Bullet::BulletImpact()
{
    if (_bulletImpacted) // 한 번만
        return;     

    _bulletImpacted = true;

    // 이동 정지
    Ptr<MovementComponent> mv = FindComponent<MovementComponent>("Movement");
    if (mv) 
    {
        mv->SetMoveAxis(FVector3D::Zero);
        mv->SetSpeed(0.f);
        mv->SetEnable(false);
    }

    if (_mesh)
    {
        _mesh->SetPlay("Tears", true);
    }

    // 애니 끝날 즈음 제거 (시간은 스플래시 길이에 맞게 조정)
    TimeManager::Instance().RemoveTimer(_timerID);
    _timerID = TimeManager::Instance().SetTimer(0.3f, false, this, &Bullet::RemoveBullet);
}

void Bullet::OnHit(Weak<class CollisionComponent> dest)
{
    Ptr<CollisionComponent> destCol = Lock<CollisionComponent>(dest);
    if (!destCol) return;

    Ptr<Pawn> pawn = Cast<Actor, Pawn>(destCol->GetOwner());
    if (pawn)
        pawn->TakeDamage(_damage);   // 1단계 HP 시스템

    BulletImpact();
}

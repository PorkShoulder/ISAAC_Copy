#include "pch.h"
#include "MovementComponent.h"
#include "SceneComponent.h"

//#include "../Object/Actor.h"
#include "../Component/TileComponent.h"
#include "../Component/AABBCollisionComponent.h"

#include "../World/Level.h"

#include "../Controller/PlayerController.h"

#include "../Object/Pawn.h"
#include "../Object/TileMap.h"

#include "../Common/LogManager.h"




bool MovementComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(id, name, owner);

    _speed = 1.f;

    return true;
}

void MovementComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    //움직일 대상이 없다면
    if (nullptr == _updateComponent)
        return;

    // 가속 / 감속 부분
    if (_moveAxis.Length() > 0)
    {
        // 방향 전환 체크
        float dot = _lastMoveAxis._x * _moveAxis._x + _lastMoveAxis._y * _moveAxis._y;
        if (dot < 0.f)
            _speed *= 0.5f;  // 반대 방향이면 속도 절반
        _speed += _accel * deltaTime;
        if (_speed > _maxSpeed)
            _speed = _maxSpeed;
    }
    else
    {
        _speed -= _friction * deltaTime;
        if (_speed <= 0.f)
        {
            _speed = 0.f;
            return;
        }
    }

    FVector3D curPos = _updateComponent->GetWorldPosition();
    FVector3D moveDir = (_moveAxis.Length() > 0) ? _moveAxis : _lastMoveAxis;
    FVector3D delta = moveDir * _speed * deltaTime;
    FVector3D result = curPos;

    Ptr<TileComponent> tileComp = nullptr;
    if (Ptr<Level> level = GetLevel())
        if (Ptr<TileMap> tileMap = level->GetTileMap())
            tileComp = tileMap->GetTileComponent();

    // AABB 에서 절반 크기 가져오기
    float halW = 0.f;
    float halH = 0.f;
    Ptr<Actor> owner = Lock<Actor>(_owner);
    if (owner) 
    {
        Ptr<AABBCollisionComponent> col = owner->FindSceneComponent<AABBCollisionComponent>("AABB");
        if (col)
        {
            halW = col->GetBoxSize()._x * 0.5f;
            halH = col->GetBoxSize()._y * 0.5f;
        }
    
    }


    //X 축 이동 체크
    float nextX = curPos._x + delta._x;
    if (tileComp && halW > 0.f)
    {
        bool blockedX = false;
        if (delta._x > 0)

            blockedX = tileComp->IsBlocked(FVector3D(nextX + halW, curPos._y + halH, 0))
                    || tileComp->IsBlocked(FVector3D(nextX + halW, curPos._y - halH, 0));
        else if (delta._x < 0)
            blockedX = tileComp->IsBlocked(FVector3D(nextX - halW, curPos._y + halH, 0))
                    || tileComp->IsBlocked(FVector3D(nextX - halW, curPos._y - halH, 0));
        if (!blockedX)
            result._x = nextX;
    }
    else
        result._x = nextX;
    
    //Y 축 이동 체크
    float nextY = curPos._y + delta._y;
    if (tileComp && halH > 0.f)
    {
        bool blockedY = false;
        if (delta._y > 0)
            blockedY = tileComp->IsBlocked(FVector3D(result._x + halW, nextY + halH, 0))
                    || tileComp->IsBlocked(FVector3D(result._x - halW, nextY + halH, 0));
        else if (delta._y < 0)
            blockedY = tileComp->IsBlocked(FVector3D(result._x + halW, nextY - halH, 0))
                    || tileComp->IsBlocked(FVector3D(result._x - halW, nextY - halH, 0));
        if (!blockedY)
            result._y = nextY;
    }
    else
        result._y = nextY;
    

    //이동량을 통해 도달할 위치
    _nextPosition = result;
    _updateComponent->SetWorldPosition(_nextPosition);


}

void MovementComponent::Destroy()
{
    ActorComponent::Destroy();
}

void MovementComponent::SetUpdateComponent(Ptr<class SceneComponent> updateComp)
{
    _updateComponent = updateComp;
}

Ptr<class SceneComponent> MovementComponent::GetUpdateComponent() const
{
    return _updateComponent;
}

void MovementComponent::SetMoveAxis(const FVector3D& moveAxis)
{
    _moveAxis = moveAxis;
    _moveAxis.Normalize();
    if (_moveAxis.Length() > 0)
        _lastMoveAxis = _moveAxis; // 마지막 이동방향 기억


}
void MovementComponent::AddMoveAxis(const FVector3D& moveAxis)
{
    _moveAxis += moveAxis;
    _moveAxis.Normalize();
}

void MovementComponent::Stop()
{
    _moveAxis = FVector3D::Zero;
}


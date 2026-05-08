#pragma once
#include "ActorComponent.h"

//==================================
// 오브젝트의 이동을 담당하는 컴포넌트.
// 다른 컴포넌트를 대신 움직여준다.
//==================================

class MovementComponent : public ActorComponent
{
public:
    MovementComponent() = default;
    virtual ~MovementComponent() = default;

protected:
    float _speed = 0.f;                         //움직이는 속도
    FVector3D _moveAxis = FVector3D::Zero;      //움직이는 방향
    FVector3D _nextPosition = FVector3D::Zero;  //이동 후에 도착할 위치

    // 자기는 위치가 없음 대신 지정된 SceneComopnent의 위치 변경.
    Ptr<class SceneComponent> _updateComponent = nullptr; 

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Destroy() override;

public:
    void SetSpeed(float speed) { _speed = speed; }
    const float GetSpeed() const { return _speed; }
    const FVector3D& GetMoveAxis() const { return _moveAxis; }
    void SetUpdateComponent(Ptr<class SceneComponent> updateComp);
    Ptr<class SceneComponent> GetUpdateComponent() const;
    void SetMoveAxis(const FVector3D& moveAxis);
    void AddMoveAxis(const FVector3D& moveAxis);
    void Stop();
};


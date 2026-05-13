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
    float _maxSpeed = 200.f;                    //최대 속도
    float _accel = 300.f;                       //가속도
    bool _isAccel = false;                      //가속 상태확인
    float _friction = 500.f;                    //마찰력
    
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
    void SetMaxSpeed(float max) { _maxSpeed = max; }
    void SetAccel(float accel) { _accel = accel; }
    void SetFriction(float friction) { _friction = friction; }
    const float GetSpeed() const { return _speed; }
    const FVector3D& GetMoveAxis() const { return _moveAxis; }
    void SetUpdateComponent(Ptr<class SceneComponent> updateComp);
    Ptr<class SceneComponent> GetUpdateComponent() const;
    void SetMoveAxis(const FVector3D& moveAxis);
    void AddMoveAxis(const FVector3D& moveAxis);
    void Stop();


};


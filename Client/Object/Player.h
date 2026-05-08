#pragma once
#include "Pawn.h"

//====================================
// Pawn을 상속받은 플레이어 캐릭터, 
// 컴포넌트를 조립해서
// 실제 게임에서 동작하는 캐릭터를 만드는곳.
//====================================


class Player : public Pawn
{
public:
    Player() = default;
    virtual ~Player() = default;

private:
    Ptr<class MovementComponent> _movement;     // 이동처리 
    Ptr<class CameraComponent> _camera;         // 카메라가 플레이어를 따라다님.
    Ptr<class AABBCollisionComponent> _col;     // 충돌판정 
    Ptr<class SphereCollisionComponent> _colSphere; 
    //Ptr<class SoundComponent> _sound;
    int32 _timerID = -1;

    bool reverse = false;
    float _opacity = 0.f;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot);
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);

    virtual void Destroy() override;

private:
    void MoveRight(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveUp(float deltaTime);
    void MoveDown(float deltaTime);

    void MoveStop(float deltaTime);

    void OnDance(float deltaTime);

    void mouseDown(float deltaTime);

    void onDanceNotifyCallBack();


private:
    void BlockCallBack(Weak<class CollisionComponent> dest);
    void OverlapCallBack(Weak<class CollisionComponent> dest);
    void ReleaseCallBack(Weak<class CollisionComponent> dest);
};

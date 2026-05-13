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
    Ptr<class SoundComponent> _sound;
    int32 _timerID = -1;

    bool reverse = false;
    bool _headKeyActive = false;
    float _opacity = 0.f;

    //이동조작 플래그
    bool _moveRight = false;
    bool _moveLeft = false;
    bool _moveUp = false;
    bool _moveDown = false;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);

    virtual void Destroy() override;

private:
    void SetHeadDirection(const std::string& anim, bool flip);
    void UpdateMovement();
    void MoveRight(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveUp(float deltaTime);
    void MoveDown(float deltaTime);
    void MoveRightStop(float deltaTime);
    void MoveLeftStop(float deltaTime);
    void MoveUpStop(float deltaTime);
    void MoveDownStop(float deltaTime);

    void MoveStop(float deltaTime);

    //아이작 머리 조작
    void HeadRight(float deltaTime);
    void HeadLeft(float deltaTime);
    void HeadUp(float deltaTime);
    void HeadDown(float deltaTime);
    void HeadRelease(float deltaTime);

    /*void OnDance(float deltaTime);*/

    void mouseDown(float deltaTime);

    


private:
    void BlockCallBack(Weak<class CollisionComponent> dest);
    void OverlapCallBack(Weak<class CollisionComponent> dest);
    void ReleaseCallBack(Weak<class CollisionComponent> dest);
};

#pragma once
#include "Pawn.h"

class Monster : public Pawn
{
public:
    Monster() = default;
    virtual ~Monster() = default;

private:
    int32 _bulletTimer = -1;
    int32 _rotBulletTimer = -1;
    Weak<class Player> _target;
    Ptr<class AABBCollisionComponent> _col;
    Ptr<class SphereCollisionComponent> _colSphere;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot);
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);

    virtual void Destroy() override;

    void SetTarget(Ptr<class Player> player);

private:
    void Fire();
    void RotBulletFire();
    void BlockCallback(Weak<class CollisionComponent> comp);

private:
    bool IsCheck();

};


#pragma once
#include "Pawn.h"
#include "../Common/Info.h"

class Monster : public Pawn
{
public:
    Monster() = default;
    virtual ~Monster() = default;

private:
    int32 _bulletTimer = -1;
    int32 _rotBulletTimer = -1;
    Weak<class Player> _target;

    // 충돌체 
    Ptr<class AABBCollisionComponent> _col;
    Ptr<class SphereCollisionComponent> _detectCol;
    bool _isChasing = false;

    Ptr<class SpriteComponent> _monsterMesh = nullptr;
    FMonsterData _monsterData;
    

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;
    void SetTarget(Ptr<class Player> player);
    void SetMonsterData(const FMonsterData& data);
private:
    void Fire();
    void RotBulletFire();
    void BlockCallback(Weak<class CollisionComponent> comp);
    void UpdataeAnimation();
    void OnDetectPlayer(Weak<class CollisionComponent> dest);
    void OnHit(Weak<class CollisionComponent> dest);

    bool IsCheck();

};


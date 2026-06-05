#pragma once
#include "Pawn.h"

enum class eMonsterType
{
    host,
    fatty,
    END
};


class Monster : public Pawn
{
public:
    Monster() = default;
    virtual ~Monster() = default;

private:
    int32 _bulletTimer = -1;
    int32 _rotBulletTimer = -1;
    Weak<class Player> _target;
    eMonsterType _monsterType = eMonsterType::fatty;

    // 충돌체 
    Ptr<class AABBCollisionComponent> _col;
    Ptr<class SphereCollisionComponent> _colSphere;

    Ptr <class SpriteComponent> _mesh = nullptr;


public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;
public:
    void SetTarget(Ptr<class Player> player);
    
    void SetMonsterType(eMonsterType type) { _monsterType = type; }

private:
    void Fire();
    void RotBulletFire();
    void BlockCallback(Weak<class CollisionComponent> comp);

private:
    bool IsCheck();

};


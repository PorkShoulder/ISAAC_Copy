#pragma once
#include "Actor.h"

enum class eBulletType
{
    Normal,
    End
};


class Bullet : public Actor
{
public:
    Bullet() = default;
    virtual ~Bullet() = default;

private:
    int32 _timerID = -1;

private:
    // 총알 계산
    int32 _damage = 1;
    bool _bulletImpacted = false;
    eBulletType _type = eBulletType::Normal;
    Ptr<class AABBCollisionComponent> _col;
    Ptr<class SpriteComponent> _mesh = nullptr;
    bool _removeRequested = false;



public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);

    virtual void Destroy() override;

    void SetDir(const FVector3D& dir);
    void SetSpeed(const float speed);
public: 
    void SetDamage(int32 dmg) { _damage = dmg; }
    void SetAnimation(const std::string& animName); // 외형 교체
    void SetBulletType(eBulletType type);       //타입에 따라 외형 및 스탯 세팅.
    void SetProfile(const std::string& name);
    

private:
    void RemoveBullet();
    void BulletImpact(); // 총알 충돌시 처리 
    void OnHit(Weak<class CollisionComponent> dest); // 피격 콜백
};


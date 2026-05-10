#pragma once
#include "CollisionComponent.h"

class SphereCollisionComponent : public CollisionComponent
{
public:
    SphereCollisionComponent();
    virtual ~SphereCollisionComponent();

private:
    //원의 반지름
    float _radius = 0.f;

public:
    float GetRadius() const { return _radius; }
    void SetRedius(float r) { _radius = r; }

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTiem) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;

    //다른 충돌체랑 충돌 계산 함수
    //가상함수
    virtual bool Collision(Weak<CollisionComponent> dest);
};


#pragma once
#include "CollisionComponent.h"

class AABBCollisionComponent : public CollisionComponent
{
public:
    AABBCollisionComponent();
    virtual ~AABBCollisionComponent();

protected:
    FVector2D _boxSize;
    FAABB2D _box;

public:
    const FVector2D& GetBoxSize() const { return _boxSize; }
    const FAABB2D& GetBox() const { return _box; }
    void SetBoxSize(const FVector2D& size)
    {
        SetBoxSize(size._x, size._y);
    }

    void SetBoxSize(float x, float y)
    {
        _boxSize._x = x;
        _boxSize._y = y;
    }

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTiem) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;

    //다른 충돌체랑 충돌 계산 함수
    virtual bool Collision(Weak<CollisionComponent> dest) override; 
};


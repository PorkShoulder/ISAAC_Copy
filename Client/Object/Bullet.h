#pragma once
#include "Actor.h"

class Bullet : public Actor
{
public:
    Bullet() = default;
    virtual ~Bullet() = default;

private:
    int32 _timerID = -1;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);

    virtual void Destroy() override;

    void SetDir(const FVector3D& dir);
    void SetSpeed(const float speed);

private:
    void RemoveBullet();
};


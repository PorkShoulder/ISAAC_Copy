#pragma once
#include "Actor.h"


class TestActor : public Actor
{
public:
    TestActor();
    virtual ~TestActor();

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;

private:
    void MoveRight(float deltaTime);
    void MoveLeft(float deltaTime);
    void ScaleRight(float deltaTime);
    void ScaleLeft(float deltaTime);
};


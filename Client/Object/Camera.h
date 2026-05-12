#pragma once
#include "Actor.h"

class Camera : public Actor
{
public:
    Camera();
    virtual ~Camera();

private:
    Ptr<class CameraComponent> _cam;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
    virtual void Tick(float deltaTime);

    virtual void Destroy() override;

};


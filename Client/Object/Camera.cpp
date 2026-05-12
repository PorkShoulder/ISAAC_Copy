#include "pch.h"
#include "Camera.h"

#include "Component/CameraComponent.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

bool Camera::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    _cam = CreateSceneComponent<CameraComponent>("Cam");

    return true;
}


void Camera::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Camera::Destroy()
{
}

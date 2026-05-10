#include "pch.h"
#include "CameraManager.h"
#include "../Component/CameraComponent.h"

#include "../Core/Device.h"

#include "../World/Level.h"

bool CameraManager::Init(Ptr<Level> level)
{
    //todo create Main Camera?
    _level = level;

    FResolution rs = Device::Instance().GetRS();

    CreateUIProj(static_cast<float>(rs._width), static_cast<float>(rs._height), 2000);

    return true;
}

void CameraManager::SetMainCamera(Ptr<class CameraComponent> camera)
{
    _mainCamera = camera;

    _cameras[_mainCamera->GetName()] = _mainCamera;
}

void CameraManager::ChangeMainCamera(const std::string& name)
{
    auto foundCam = FindCamera(name);
    if (!foundCam)
        return;

    _mainCamera = foundCam;
}

Ptr<class CameraComponent> CameraManager::GetMainCamera() const
{
    return _mainCamera;
}

Ptr<class CameraComponent> CameraManager::FindCamera(const std::string& name) const
{
    auto it = _cameras.find(name);
    if (it == _cameras.end())
        return nullptr;

    return it->second;
}

const FMatrix& CameraManager::GetViewMatrix() const
{
    return _mainCamera->GetViewMatrix();
}

const FMatrix& CameraManager::GetProjMatrix() const
{
    return _mainCamera->GetProjMatrix();
}

const FVector3D& CameraManager::GetCameraWorldPos() const
{
    return _mainCamera->GetWorldPosition();
}

const FMatrix& CameraManager::GetUIProjMatrix() const
{
    return _uiProj;
}

void CameraManager::Destroy()
{
    //
}

void CameraManager::CreateUIProj(float x, float y, float dist)
{
    _uiProj = DirectX::XMMatrixOrthographicOffCenterLH(0.f, x, 0.f, y, 0, dist);
}

#pragma once
#include "../Core/Object.h"

#include <unordered_map>

class CameraManager : public Object
{
public:
    CameraManager() = default;
    virtual ~CameraManager() = default;
    CameraManager(const CameraManager&) = delete;
    CameraManager(CameraManager&&) = delete;
    CameraManager operator = (const CameraManager&) = delete;
    CameraManager operator = (CameraManager&&) = delete;

private:
    Weak<class Level> _level;
    Ptr<class CameraComponent> _mainCamera; //화면을 비출 메인 카메라
    std::unordered_map<std::string, Ptr<class CameraComponent>> _cameras;

    FMatrix _uiProj;

public:
    bool Init(Ptr<class Level> level);
    void SetMainCamera(Ptr<class CameraComponent> camera);
    void ChangeMainCamera(const std::string& name);
    Ptr<class CameraComponent> GetMainCamera() const;
    Ptr<class CameraComponent> FindCamera(const std::string& name) const;
    const FMatrix& GetViewMatrix() const;
    const FMatrix& GetProjMatrix() const;
    const FVector3D& GetCameraWorldPos() const;

    const FMatrix& GetUIProjMatrix() const;


public:
    virtual void Destroy() override;

private:
    void CreateUIProj(float x, float y, float dist);

};


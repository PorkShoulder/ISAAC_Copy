#pragma once
#include "SceneComponent.h"

//=======================================
// 뷰 행렬과 투영 행렬을 만듬.
//=======================================



enum eCameraType
{
    Perspective,        //3D        (원근 O)
    Ortho,              //2D 카메라 (원근 X)
};


//게임 화면을 비추는 카메라
class CameraComponent : public SceneComponent
{
public:
    CameraComponent();
    virtual ~CameraComponent();

protected:
    FMatrix _matView = {};  // 뷰 행렬 -> 카메라 위치/방향 기준으로 월드를 다시 계산.
    FMatrix _matProj = {};  // 투영 행렬 -> 3D좌표를 2D화면에 투영하는 행렬.
    eCameraType _type = eCameraType::Ortho; // 카메라 종류 
    float _viewAngle = 90.f;
    float _width = 0.f;             // 카메라가 비추는 영역 크기
    float _height = 0.f;            
    float _viewDistance = 1000.f;   // 카메라가 볼 수 있는 최대 거리 

public:
    void SetProjectionType(eCameraType type);

    const FMatrix& GetViewMatrix() const { return _matView; }
    const FMatrix& GetProjMatrix() const { return _matProj; }

    eCameraType GetProjectionType() const { return _type; }
    float GetViewAnglg() const { return _viewAngle; }
    void SetViewAngle(float angle) { _viewAngle = angle; }

    float GetWidth() const { return _width; }
    void SetWidht(float width) { _width = width; }

    float GetHeight() const { return _height; }
    void SetHeight(float height) { _height = height; }

    void SetViewResolution(float w, float h) { _width = w; _height = h; }

    float GetViewDistance() const { return _viewDistance; }
    void SetViewDistance(float distance) { _viewDistance = distance; }


public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTiem) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;

    void OnWindowSizeChanged(int, int);
};


#include "pch.h"
#include "CameraComponent.h"
#include "../World/Level.h"
#include "../World/CameraManager.h"
#include "../Core/Device.h"

#include "../Shader/TranformCBuffer.h"


CameraComponent::CameraComponent()
{
	FResolution rs = Device::Instance().GetRS();
	_width = static_cast<float>(rs._width);
	_height = static_cast<float>(rs._height);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::SetProjectionType(eCameraType type)
{
	_type = type;

	switch (_type)
	{
	case Perspective:
		break;
	case Ortho:
		_matProj = DirectX::XMMatrixOrthographicOffCenterLH(_width / -2, _width / 2, _height / -2, _height / 2, 0.f, _viewDistance);
		break;
	default:
		break;
	}
}

bool CameraComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
	SceneComponent::Init(id, name, owner);

	SetProjectionType(_type);

	Ptr<Level> level = owner->GetLevel();
	if (nullptr == level)
		return false;

	level->SetMainCamera(This<CameraComponent>());

	return true;
}

void CameraComponent::Tick(float deltaTime)
{
	SceneComponent::Tick(deltaTime);

	_matView.Indentity();

	for (int i = 0; i < AXIS_TYPE::END; ++i)
	{
		FVector3D axis = _axis[i];
		memcpy(&_matView[i][0], &axis, sizeof(FVector3D));
	}

	_matView.Transpose();

	_matView._41 = -_axis[AXIS_TYPE::X].Dot(_world._position);
	_matView._42 = -_axis[AXIS_TYPE::Y].Dot(_world._position);
	_matView._43 = -_axis[AXIS_TYPE::Z].Dot(_world._position);

}

void CameraComponent::Collision(float deltaTiem)
{
	SceneComponent::Collision(deltaTiem);
}

void CameraComponent::Render(float deltaTime)
{
	SceneComponent::Render(deltaTime);
}

void CameraComponent::Destroy()
{

}

void CameraComponent::OnWindowSizeChanged(int width, int height)
{
	// 
}

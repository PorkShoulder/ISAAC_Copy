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

	//카메라 행렬 계산

	//뷰변환이 일어나게 되면, 카메라는 Z+ 방향을 바라보게 됩니다
	//카메라를 구성하는 축은 정확히 월드의 축과 일치하게 됩니다.

	//카메라 축을 이용해서 행렬을 구성하고, 해당 행렬에 뷰행렬을 곱하게 되면, 결과로 항등행렬
	//A * B 항등행렬이 나오면, B는 A의 역행렬
	//카메라 축을 이용해서 행렬 <-- 뷰행렬의 역행렬

	//행렬을 구성하는 각 축이 서로에 90도인 행렬을 직교행렬이라고 합니다.
	//직교 행렬은 역행렬과 전치행렬이 동일하다.

	// 카메라 X 축 * 뷰행렬 = 1, 0, 0
	// 카메라 Y 축 * 뷰행렬 = 0, 1, 0
	// 카메라 Z 축 * 뷰행렬 = 0, 0, 1

	//카메라를 원점으로 땡겨야 해서
	//월드에 축에 카메라의 위치를 내적하게 되면
	//A벡터에 투영된 길이가 나오고, 해당 길이만큼 위치를 빼주면
	//카메라의 위치는 원점으로

	//이거 전부 다해서 카메라 행렬 구성

	_matView.Indentity();

	for (int i = 0; i < AXIS_TYPE::END; ++i)
	{
		FVector3D axis = _axis[i];
		memcpy(&_matView[i][0], &axis, sizeof(FVector3D));
	}

	//Xx Xy Xz 0
	//Yx Yy Yz 0
	//Zx Zy Zz 0
	//0  0  0  1

	_matView.Transpose();

	//Xx Yx Zx 0
	//Xy Yy Zy 0
	//Xz Yz Zz 0
	//0  0  0  1

	_matView._41 = -_axis[AXIS_TYPE::X].Dot(_world._position);
	_matView._42 = -_axis[AXIS_TYPE::Y].Dot(_world._position);
	_matView._43 = -_axis[AXIS_TYPE::Z].Dot(_world._position);

	//이렇게 뷰행렬 구성
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

#pragma once
#include "SceneComponent.h"

#include <vector>

//=========================================
// 메쉬를 그리기 위하 재질 슬롯을 관리
// SceneComponent를 상속 위치,재질 정보를 합침.
//==========================================

class MeshComponent : public SceneComponent
{
public:
	MeshComponent();
	virtual ~MeshComponent();
protected:
	std::vector<Ptr<class Material>> _materialSlots;

public:
	virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
	virtual void Tick(float deltaTime) override;
	virtual void Collision(float deltaTime) override;
	virtual void Render(float deltaTime) override;

	virtual void Destroy() override;
};


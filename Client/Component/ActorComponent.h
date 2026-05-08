#pragma once
#include "Component.h"

//======================================
// 기능 전용 컴포넌트 부모의 클래스.
//======================================


class ActorComponent : public Component
{
public:
	ActorComponent();
	virtual ~ActorComponent();

public:
	virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
	virtual void Tick(float deltaTime) override;
	virtual void Destroy() override;

};


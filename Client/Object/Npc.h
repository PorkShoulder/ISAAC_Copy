#pragma once
#include "Actor.h"
class Npc :public Actor
{
public:
	Npc();
	virtual ~Npc();
public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);
	virtual void Destroy() override;

};


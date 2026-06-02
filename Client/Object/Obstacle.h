#pragma once
#include "Actor.h"

class Obstacle : public Actor
{
public:
	Obstacle();
	virtual ~Obstacle();
public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Tick(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);
	virtual void Destroy() override;
	


};


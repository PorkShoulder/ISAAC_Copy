#pragma once
#include "Actor.h"

enum class eObstacleType
{
	PIT,        // 구덩이
	FIRE,       // 모닥불
	IRON_WALL,  // 철벽
	ROCK_WALL,  // 돌벽
	SPIKE,      // 가시밭
	WEB,        // 거미줄
	END
};

struct FObstacleData
{
	bool canflyPass = false;	// 비행 통과
	bool canWalkPass = false;	// 걷기 통과
	bool breakable = false;		// 파괴 가능
	bool damage = false;		// 닿으면 피해
};


class Obstacle : public Actor
{
public:
	Obstacle();
	virtual ~Obstacle();
protected:


public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Tick(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);
	virtual void Destroy() override;
	


};


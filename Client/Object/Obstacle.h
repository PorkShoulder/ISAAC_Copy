#pragma once
#include "Actor.h"
class Obstacle : public Actor
{
public:
	Obstacle();
	virtual ~Obstacle();
private:
	eObstacleType _obstacleType = eObstacleType::ROCK_WALL;
	std::string _obstacleName;
	
	int32 _hp = 1;
	
	Ptr<class AABBCollisionComponent> _col;

	FObstacleData _data;
	Ptr<class SpriteComponent> _obstacleMesh = nullptr;

	
	
	
	


public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Tick(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);
	virtual void Destroy() override;
	virtual void Save(std::ofstream& file) override;
	virtual void Load(std::ifstream& file) override;
public:
	
	void SetObstacleData(const FObstacleData& data);
	eObstacleType GetObstacleType() const { return _obstacleType; }
	void OnOverlap(Weak<class CollisionComponent> dest);
	void OnHitByBullet(Weak<class CollisionComponent> dest);
	float GetDamageAmount() const { return _data.damage ? _data.damageAmount : 0.f; }
};


#pragma once

#include"Actor.h"

enum class eDoorType
{
	NORMAL,	// 일반 방
	LOCKED,	// 열쇠 소모 방
	HIDDEN,	// 숨겨진 방
	BOSS,	// 보스 방
	END		
};
inline const char* DoorTypeName[] = { "NORMAL", "LOCKED", "HIDDEN","BOSS" };




class Door : public Actor
{
public:
	Door();
	virtual ~Door();
private:
	eDoorType _doorType = eDoorType::NORMAL;
	std::string _doorName;

	Ptr<class AABBCollisionComponent> _col;
	Ptr<class StaticMeshComponent> _mesh = nullptr;
	


public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Tick(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);
	virtual void Destroy() override;
public:
	void SetTexture(const std::string& name);
	void SetDoorType(eDoorType type) { _doorType = type; }
	eDoorType GetDoorType() const { return _doorType; }

	

};


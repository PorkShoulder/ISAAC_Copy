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
enum  class eDoorDir
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	END
};
inline const char* DoorDirName[] = { "UP", "DOWN", "LEFT", "RIGHT" };

enum class eDoorState
{
	OPEN,           // 열린 문
	NORMAL_CLOSED,  // 전투 중 닫힌 문
	LOCKED_CLOSED,  // 열쇠문 잠김 상태
	HIDDEN_CLOSED,  // 히든문 닫힘/숨김
	OPENING,        // 열리는 애니메이션 중
	CLOSING,        // 닫히는 애니메이션 중
	END
};


class Door : public Actor
{
public:
	Door();
	virtual ~Door();
private:
	eDoorType _doorType = eDoorType::NORMAL;
	eDoorDir _doorDir = eDoorDir::UP;
	eDoorState _doorState = eDoorState::OPEN;
	std::string _doorName;

	Ptr<class AABBCollisionComponent> _col;
	Ptr<class SpriteComponent> _mesh = nullptr;
	Ptr<class SpriteComponent> _frame = nullptr;
	Ptr<class SpriteComponent> _panel = nullptr;
	


public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Tick(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);
	virtual void Destroy() override;
public:
	void SetTexture(const std::string& name);
	void SetDoorType(eDoorType type);
	eDoorType GetDoorType() const { return _doorType; }
	
	void SetDoorDir(eDoorDir dir) { _doorDir = dir; }
	eDoorDir GetDoorDir() const { return _doorDir; }

	void OverlapCallback(Weak<class CollisionComponent> dest);
	void ChangeDoorState(eDoorState state);
	

};


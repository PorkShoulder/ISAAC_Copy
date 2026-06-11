#pragma once
#include "Actor.h"
#include "../Common/Info.h"
#include "../World/World.h"
#include "../World/TitleLevel.h"


class Door : public Actor
{
public:
	Door();
	virtual ~Door();
private:
	FDoorSpawnData _doorData;
	
	Ptr<class SpriteComponent> _doorFrame = nullptr;
	Ptr<class SpriteComponent> _doorLeft = nullptr;
	Ptr<class SpriteComponent> _doorRight = nullptr;
	Ptr<class SpriteComponent> _doorOpen = nullptr;

	Ptr<class AABBCollisionComponent> _col;
	eRoomDir _exitDir = eRoomDir::END;  // 이 문이 어느 방향 출구인지
	

public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, 
								const FRotator& rot, const std::string& name) override;
	virtual void Tick(float deltaTime) override;
	virtual void Collision(float deltaTime) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;

	virtual void Save(std::ofstream& file) override;
	virtual void Load(std::ifstream& file) override;
	const FDoorSpawnData& GetDoorData() const { return _doorData; }
	void SetDoorData(const FDoorSpawnData& data);
	void SetOpen(bool bOpen);
	bool IsOpen()const { return _doorData.bOpen; }
	
	void TryOpen(Ptr<class Player> player);
	eDoorType GetDoorType() const { return _doorData.doorType; }
	bool IsBattleControl() const { return _doorData.bBattle; }
	
	void SetExitDir(eRoomDir dir) { _exitDir = dir; }
	eRoomDir GetExitDir() const { return _exitDir; }

private:
	void UpdateVisibility();
	void RegisterPartAnim(Ptr<class SpriteComponent> sprite,
		const std::string& animName, const FVector4D& rect);
	void OnOverlap(Weak<class CollisionComponent> dest);
	void doorOpen(Ptr<Player> player);
	
	



};


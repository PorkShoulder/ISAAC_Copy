#pragma once
#include "Actor.h"
#include "../Common/Info.h"



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


public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, 
								const FRotator& rot, const std::string& name) override;
	virtual void Tick(float deltaTime) override;
	virtual void Collision(float deltaTime) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;

	void SetDoorData(const FDoorSpawnData& data);
	void SetOpen(bool bOpen);
	bool IsOpen()const { return _doorData.bOpen; }
	
	void TryOpen(Ptr<class Player> player);
	eDoorType GetDoorType() const { return _doorData.doorType; }
	bool IsBattleControl() const { return _doorData.bBattle; }
	static void RenderDoorUI();


private:
	void UpdateVisibility();
	void RegisterPartAnim(Ptr<class SpriteComponent> sprite,
		const std::string& animName, const FVector4D& rect);
	void OnOverlap(Weak<class CollisionComponent> dest);
	void doorOpen(Ptr<Player> player);
	
	



};


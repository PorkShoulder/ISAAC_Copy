#pragma once

#include "Actor.h"


enum class eItemType
{
	PASSIVE,	// 
	ACTIVE,		//
	CONSUMABLE,	//
	END
};
inline const char* ItemTypeName[] = { "PASSIVE", "ACTIVE", "CONSUMANLE" };

class Item :public Actor
{
public:
	Item();
	virtual ~Item();
protected:
	eItemType _itemType = eItemType::PASSIVE;
	std::string _itemName;
	bool _isPickedUp = false;

	
	Ptr<class AABBCollisionComponent> _col;			// 충돌 컴포넌트 (플레이어 접촉 감지)
	Ptr<class StaticMeshComponent> _mesh = nullptr;

public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Tick(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);
	virtual void Destroy() override;

public:
	void SetTexture(const std::string& name);
	void SetItemType(eItemType type) { _itemType = type; }
	eItemType GetItemType() const { return _itemType; }
	
	void SetPickedUp(bool picked) { _isPickedUp = picked; }
	bool IspickedUp() const { return _isPickedUp; }



};


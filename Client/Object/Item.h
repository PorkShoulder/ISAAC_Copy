#pragma once
#include "Actor.h"
#include "../Common/Info.h"



class Item :public Actor
{
public:
	Item();
	virtual ~Item();
protected:
	FItemData _data;
	bool _isPickedUp = false;
	
	
	Ptr<class AABBCollisionComponent> _col;				// 충돌 컴포넌트 (플레이어 접촉 감지)
	Ptr<class SpriteComponent>	_itemMesh = nullptr;	// 아이템 렌더용 

public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
	virtual void Tick(float deltaTime) override;
	virtual void Collision(float deltaTime) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;

	virtual void Save(std::ofstream& file) override;
	virtual void Load(std::ifstream& file) override;

public:
	void SetItemData(const FItemData& data);
	eItemType GetItemType() const { return _data.itemType; }
	const FItemData& GetItemData() const { return _data; }

	void SetPickedUp(bool picked) { _isPickedUp = picked; }
	bool IspickedUp() const { return _isPickedUp; }

	void OnOverlap(Weak<class CollisionComponent> dest);


};


#pragma once

#include "Actor.h"

enum class eItemType
{
	PASSIVE,
	ACTIVE,
	CINSUMABLE,
	END
};

// 패시브 적용 스탯
struct FItemStat
{
	float attackPower = 0.f;
	float speed = 0.f;
	float hp = 0.f;
	float range = 0.f;
	float fireRate = 0.f;
};


class Item :public Actor
{
	Item();
	virtual ~Item();
protected:
	eItemType _itemType = eItemType::PASSIVE;
	std::string _itemName;
	bool _isPickedUp = false;
	// 충돌 컴포넌트 (플레이어 접촉 감지)
	Ptr<class AABBCollisionComponent> _col;
	// 렌더링용
	Ptr<class MeshComponent> _mesh;
public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Tick(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);

	virtual void DrawInspector() override;

	virtual void Destroy() override;




};


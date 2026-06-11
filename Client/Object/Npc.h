#pragma once
#include "Actor.h"
class Npc :public Actor
{
public:
	Npc();
	virtual ~Npc();
private:
	FNpcData _data;
	bool _hasInteracted = false;
	
	Ptr<class AABBCollisionComponent> _col;
	Ptr<class SpriteComponent> _npcMesh = nullptr;

public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
	virtual void Tick(float deltaTime) override;
	virtual void Collision(float deltaTime) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;

	virtual void Save(std::ofstream& file) override;
	virtual void Load(std::ifstream& file) override;
public:
	void SetNpcData(const FNpcData& data);
	void SetNpcType(eNpcType type) { _data.npcType = type; }
	eNpcType GetNpcType() const { return _data.npcType; }
	const FNpcData& GetNpcData() const { return _data; }

	void OnOverlap(Weak<class CollisionComponent> dest);
	

};


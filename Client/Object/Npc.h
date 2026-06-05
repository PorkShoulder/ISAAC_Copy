#pragma once
#include "Actor.h"

enum class eNpcType
{
	SHOP,
	COINSLOT,
	END
};
inline const char* NpcTypeName[] = { "SHOP", "COINSLOT" };



class Npc :public Actor
{
public:
	Npc();
	virtual ~Npc();
private:
	eNpcType _npcType = eNpcType::SHOP;
	Ptr<class SpriteComponent> _mesh;

public:
	virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
	virtual void Collision(float deltaTime);
	virtual void Render(float deltaTime);
	virtual void Destroy() override;
public:
	void SetTexture(const std::string& name);
	void SetNpcType(eNpcType type) { _npcType = type; }
	eNpcType GetNpcType() const { return _npcType; }

};


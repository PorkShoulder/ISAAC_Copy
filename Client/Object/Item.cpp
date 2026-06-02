#include "pch.h"
#include "Item.h"
#include"../Component/SpriteComponent.h"


Item::Item()
{
}

Item::~Item()
{
}

bool Item::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
	Actor::Init(id, pos, scale, rot, name);
    Ptr<SpriteComponent> meshComp = CreateSceneComponent<SpriteComponent>("Mesh");
    _type = eActorType::Item;
    return true;
}

void Item::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Item::Collision(float deltaTime)
{
}

void Item::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Item::DrawInspector()
{
}

void Item::Destroy()
{
    Actor::Destroy();
}

#include "pch.h"
#include "Npc.h"

#include "../Component/SpriteComponent.h"

Npc::Npc()
{
}

Npc::~Npc()
{
}

bool Npc::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    _mesh = CreateSceneComponent<SpriteComponent>("Mesh");


    SetRootComponent(_mesh);

    _type = eActorType::Npc;


    return true;
}

void Npc::Collision(float deltaTime)
{
}

void Npc::Render(float deltaTime)
{
}

void Npc::Destroy()
{
}

void Npc::SetTexture(const std::string& name)
{
}

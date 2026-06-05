#include "pch.h"
#include "Npc.h"

#include "../Component/SpriteComponent.h"
#include "../Component/AABBCollisionComponent.h"

Npc::Npc()
{
}

Npc::~Npc()
{
}

bool Npc::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);
    _type = eActorType::Npc;
    // 생성 
    _mesh = CreateSceneComponent<SpriteComponent>("Mesh");
    SetRootComponent(_mesh);
    
    // 충돌체
    _col = CreateSceneComponent<AABBCollisionComponent>("Mesh");
    _col->SetBoxSize(32.f, 32.f);
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Npc");


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


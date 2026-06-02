#include "pch.h"
#include "Obstacle.h"
#include"../Component/SpriteComponent.h"
#include"../Component/StaticMeshComponent.h"

Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
}

bool Obstacle::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);
    
    auto meshComp = CreateSceneComponent<StaticMeshComponent>("Mesh");
    meshComp->SetMesh("TexRect");
    SetRootComponent(meshComp);

    _type = eActorType::Obstacle;

    return true;
}

void Obstacle::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Obstacle::Collision(float deltaTime)
{
    
}

void Obstacle::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Obstacle::Destroy()
{
    Actor::Destroy();
}

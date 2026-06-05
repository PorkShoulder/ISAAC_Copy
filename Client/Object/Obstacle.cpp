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
    
    _mesh = CreateSceneComponent<StaticMeshComponent>("Mesh");
    _mesh->SetMesh("TexRect");
    SetRootComponent(_mesh);

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

void Obstacle::SetTexture(const std::string& name)
{
    if (_mesh)
        _mesh->AddTexture(0, name, 0);
}

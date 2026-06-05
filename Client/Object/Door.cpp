#include "pch.h"
#include "Door.h"


#include "../Component/StaticMeshComponent.h"


Door::Door()
{
}

Door::~Door()
{
}

bool Door::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    _mesh = CreateSceneComponent<StaticMeshComponent>("Mesh");
    _mesh->SetMesh("TexRect");
    SetRootComponent(_mesh);

    _type = eActorType::Door;

    return true;
}

void Door::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Door::Collision(float deltaTime)
{
}

void Door::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Door::Destroy()
{
    Actor::Destroy();
}

void Door::SetTexture(const std::string& name)
{
    if (_mesh)
        _mesh->AddTexture(0, name, 0);
}

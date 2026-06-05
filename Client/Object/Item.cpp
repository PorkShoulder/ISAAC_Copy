#include "pch.h"
#include "Item.h"

#include "Component/StaticMeshComponent.h"

Item::Item()
{
}

Item::~Item()
{
}

bool Item::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
	Actor::Init(id, pos, scale, rot, name);

    _mesh = CreateSceneComponent<StaticMeshComponent>("Mesh");
    _mesh->SetMesh("TexRect");
    SetRootComponent(_mesh);
    
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


void Item::Destroy()
{
    Actor::Destroy();
}

void Item::SetTexture(const std::string& name)
{
    if(_mesh)
        _mesh->AddTexture(0, name, 0);
}


#include "pch.h"
#include "TestActor.h"
#include "Component/MeshComponent.h"
#include "Component/InputComponent.h"
#include "Component/StaticMeshComponent.h"

TestActor::TestActor()
{}

TestActor::~TestActor()
{}

bool TestActor::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    Ptr<StaticMeshComponent> meshComp = CreateSceneComponent<StaticMeshComponent>("Mesh");
    meshComp->SetMesh("TexRect");
    meshComp->AddTexture(0, "apeach", 0);

    meshComp->SetMesh("Triangle");
    meshComp->SetShader("ColorMeshShader");

    SetRootComponent(meshComp);

   




    return true;
}

void TestActor::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void TestActor::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void TestActor::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void TestActor::Destroy()
{
    Actor::Destroy();
}

void TestActor::MoveRight(float deltaTime)
{
    FVector3D pos = _root->GetWorldPosition();
    _root->SetWorldPosition(pos._x + (1.f * deltaTime), pos._y, pos._z);
}

void TestActor::MoveLeft(float deltaTime)
{
    FVector3D pos = _root->GetWorldPosition();
    _root->SetWorldPosition(pos._x - (1.f * deltaTime), pos._y, pos._z);
}

void TestActor::ScaleRight(float deltaTime)
{
    FVector3D scale = _root->GetWorldScale();
    _root->SetWorldScale(scale._x + (1.f * deltaTime), scale._y + (1.f * deltaTime), scale._z);
}

void TestActor::ScaleLeft(float deltaTime)
{
    FVector3D scale = _root->GetWorldScale();
    _root->SetWorldScale(scale._x -(1.f * deltaTime), scale._y - (1.f * deltaTime), scale._z);
}

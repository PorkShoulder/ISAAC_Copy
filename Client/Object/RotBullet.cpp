#include "pch.h"
#include "RotBullet.h"
#include "Component/MeshComponent.h"
#include "Component/MovementComponent.h"

#include "Core/TimeManager.h"
#include "World/Level.h"

bool RotBullet::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    /* for (int i = -5; i < 5; ++i)
     {
         Ptr<MeshComponent> meshComp = CreateSceneComponent<MeshComponent>("Mesh");
         meshComp->SetMesh("Triangle");
         meshComp->SetShader("ColorMeshShader");
         meshComp->SetRelativePosition(i * 2, 0, 0);
         meshComp->SetRelativeScale(GetWorldScale());
         meshComp->SetRelativeRotation(0.f, 0.f, 0.f);

         meshComp->AttachToComponent(_root);
     }*/

     /* Ptr<MeshComponent> meshComp = CreateSceneComponent<MeshComponent>("Mesh");
      meshComp->SetMesh("Triangle");
      meshComp->SetShader("ColorMeshShader");
      meshComp->SetRelativePosition(-5.f, 0, 0);
      meshComp->SetRelativeScale(GetWorldScale());
      meshComp->SetRelativeRotation(0.f, 0.f, 0.f);

      meshComp->AttachToComponent(_root);

      Ptr<MeshComponent> meshComp1 = CreateSceneComponent<MeshComponent>("Mesh");
      meshComp1->SetMesh("Triangle");
      meshComp1->SetShader("ColorMeshShader");
      meshComp1->SetRelativePosition(5.f, 0, 0);
      meshComp1->SetRelativeScale(GetWorldScale());
      meshComp1->SetRelativeRotation(0.f, 0.f, 0.f);

      meshComp1->AttachToComponent(_root);

      Ptr<MeshComponent> meshComp2 = CreateSceneComponent<MeshComponent>("Mesh");
      meshComp2->SetMesh("Triangle");
      meshComp2->SetShader("ColorMeshShader");
      meshComp2->SetRelativePosition(0.f, 5.f, 0);
      meshComp2->SetRelativeScale(GetWorldScale());
      meshComp2->SetRelativeRotation(0.f, 0.f, 0.f);

      meshComp2->AttachToComponent(_root);

      Ptr<MeshComponent> meshComp2 = CreateSceneComponent<MeshComponent>("Mesh");
      meshComp2->SetMesh("Triangle");
      meshComp2->SetShader("ColorMeshShader");
      meshComp2->SetRelativePosition(0.f, 5.f, 0);
      meshComp2->SetRelativeScale(GetWorldScale());
      meshComp2->SetRelativeRotation(0.f, 0.f, 0.f);

      meshComp2->AttachToComponent(_root);*/


      //생기고나서 10초 후에 자동으로 삭제
    _timerID = TimeManager::Instance().SetTimer(10.f, false, this, &RotBullet::RemoveBullet);

    Ptr<MovementComponent> movement = CreateActorComponent<MovementComponent>("Movement");

    movement->SetUpdateComponent(_root);

    return true;
}

void RotBullet::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);

    FRotator rot = GetRelativeRotation();
    _root->SetRelativeRotation(rot._x, rot._y, rot._z + (90.f * deltaTime));
}


void RotBullet::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void RotBullet::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void RotBullet::Destroy()
{
    TimeManager::Instance().RemoveTimer(_timerID);
}

void RotBullet::SetDir(const FVector3D& dir)
{
    //
    Ptr<MovementComponent> foundComp = FindComponent<MovementComponent>("Movement");
    if (nullptr == foundComp)
        return;

    foundComp->SetMoveAxis(dir);
}

void RotBullet::SetSpeed(const float speed)
{
    Ptr<MovementComponent> foundComp = FindComponent<MovementComponent>("Movement");
    if (nullptr == foundComp)
        return;

    foundComp->SetSpeed(speed);
}

void RotBullet::RemoveBullet()
{
}

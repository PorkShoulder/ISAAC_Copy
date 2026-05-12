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

    /*meshComp->SetMesh("Triangle");
    meshComp->SetShader("ColorMeshShader");*/

    SetRootComponent(meshComp);

    //Ptr<InputComponent> inputComp = CreateActorComponent<InputComponent>("Input");

    ////switch 
    //auto switchAction = InputSystem::Instance().FindOrAddInputAction("switch");

    ////이런거 사용하는거는
    ////예를 들어 공격상태일때는 F키가 공격인데
    ////NPC랑 붙어있을때는 F키가 상호작용키


    ////게임
    ////서있을때, 말 탔을때


    ////moveContext
    //auto moveContext = InputSystem::Instance().FindOrAddInputContext("MOVE_CONTEXT");
    //auto moveRight = InputSystem::Instance().FindOrAddInputAction("MOVE_RIGHT");
    //auto moveLeft = InputSystem::Instance().FindOrAddInputAction("MOVE_Left");
    //moveContext->BindInputAction(moveRight, 'D');
    //moveContext->BindInputAction(moveLeft, 'A');
    //moveContext->BindInputAction(switchAction, VK_TAB);

    ////scaleContext
    //auto scaleContext = InputSystem::Instance().FindOrAddInputContext("SCALE_CONTEXT");
    //auto scaleRight = InputSystem::Instance().FindOrAddInputAction("SCALE_RIGHT");
    //auto scaleLeft = InputSystem::Instance().FindOrAddInputAction("SCALE_Left");
    //scaleContext->BindInputAction(scaleRight, 'D');
    //scaleContext->BindInputAction(scaleLeft, 'A');
    //scaleContext->BindInputAction(switchAction, VK_TAB);

    //inputComp->AddInputContext(moveContext->GetName());
    //inputComp->AddInputContext(scaleContext->GetName());

    //inputComp->BindAction(moveContext->GetName(), moveRight->GetName(), INPUT_TYPE::HOLD, this, &TestActor::MoveRight);
    //inputComp->BindAction(moveContext->GetName(), moveLeft->GetName(), INPUT_TYPE::HOLD, this, &TestActor::MoveLeft);

    //inputComp->BindAction(scaleContext->GetName(), scaleRight->GetName(), INPUT_TYPE::HOLD, this, &TestActor::ScaleRight);
    //inputComp->BindAction(scaleContext->GetName(), scaleLeft->GetName(), INPUT_TYPE::HOLD, this, &TestActor::ScaleLeft);

    //Ptr<TestActor> mine = This<TestActor>();
    //inputComp->BindAction(moveContext->GetName(), switchAction->GetName(), INPUT_TYPE::DOWN, [mine](float deltaTime) 
    //    {
    //        Ptr<InputComponent> inputComp = mine->FindComponent<InputComponent>("Input");
    //        inputComp->SetActiveContext("SCALE_CONTEXT");
    //    });

    //inputComp->BindAction(scaleContext->GetName(), switchAction->GetName(), INPUT_TYPE::DOWN, [mine](float deltaTime)
    //    {
    //        Ptr<InputComponent> inputComp = mine->FindComponent<InputComponent>("Input");
    //        inputComp->SetActiveContext("MOVE_CONTEXT");
    //    });




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

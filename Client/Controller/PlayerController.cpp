#include "pch.h"
#include "PlayerController.h"
#include "Object/Player.h"
#include "Input/InputSystem.h"

bool PlayerController::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Controller::Init(id, pos, scale, rot, name);

    //디폴트로 InputComponent를 생성하고 가지고 있는다.
    _input = CreateActorComponent<InputComponent>("Input");

    //생성한 InputComponent를 inputSystem에 등록해준다.
    InputSystem::Instance().RegisterInputComponent(GetActorID(), _input);

    return true;
}

void PlayerController::Tick(float deltaTime)
{
    Controller::Tick(deltaTime);
}

void PlayerController::Collision(float deltaTime)
{
    Controller::Collision(deltaTime);
}

void PlayerController::Render(float deltaTime)
{
    Controller::Render(deltaTime);
}

void PlayerController::Destroy()
{
    Controller::Destroy();

    InputSystem::Instance().RemoveInputComponent(GetActorID(), _input->GetComponentID());
}

Ptr<InputComponent> PlayerController::GetInputComponent() const
{
    return _input;
}


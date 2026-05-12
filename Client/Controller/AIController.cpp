#include "pch.h"
#include "AIController.h"
#include "Component/AIComponent.h"

bool AIController::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Controller::Init(id, pos, scale, rot, name);

    _aiComponent = CreateActorComponent<AIComponent>("AI");
    _aiComponent->SetController(This<AIController>());

    return true;
}

void AIController::Tick(float deltaTime)
{
    Controller::Tick(deltaTime);

    _aiComponent->Tick(deltaTime);
}

void AIController::Collision(float deltaTime)
{
}

void AIController::Render(float deltaTime)
{
}

void AIController::Destroy()
{
    DESTROY(_aiComponent);
}

Ptr<class AIComponent> AIController::GetAI() const
{
    return _aiComponent;
}

Ptr<class MachineBase> AIController::GetAIStateMachine() const
{
    if (!_aiComponent)
        return nullptr;

    return _aiComponent->GetAIStateMachine();
}

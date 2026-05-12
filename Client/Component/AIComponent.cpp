#include "pch.h"
#include "AIComponent.h"
#include "MachineBase.h"
#include "Controller/AIController.h"


AIComponent::AIComponent()
{
}

AIComponent::~AIComponent()
{
}

bool AIComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(id, name, owner);

    return true;
}

void AIComponent::Tick(float deltaTime)
{
    ActorComponent::Tick(deltaTime);

    if (!_machine)
        return;

    _machine->Tick(deltaTime);
}

void AIComponent::Destroy()
{
    DESTROY(_machine);
}

void AIComponent::SetController(Ptr<class AIController> controller)
{
    _controller = controller;
}

Ptr<class AIController> AIComponent::GetController() const
{
    return Lock<AIController>(_controller);
}

Ptr<class MachineBase> AIComponent::GetAIStateMachine()
{
    return _machine;
}



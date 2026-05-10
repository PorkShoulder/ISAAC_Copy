#include "pch.h"
#include "ActorComponent.h"

ActorComponent::ActorComponent()
{
}

ActorComponent::~ActorComponent()
{
}

bool ActorComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    Component::Init(id, name, owner);
    return true;
}

void ActorComponent::Tick(float deltaTime)
{
    Component::Tick(deltaTime);
}

void ActorComponent::Destroy()
{
}

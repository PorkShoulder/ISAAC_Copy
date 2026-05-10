#include "pch.h"
#include "AIState.h"
#include "AIComponent.h"
#include "AITransition.h"

AIState::AIState()
{}

AIState::~AIState()
{}

bool AIState::Init(const std::string& name)
{
    _name = name;
    
    return true;
}

Ptr<AIState> AIState::Tick(float deltaTime)
{
    for (auto& it : _transitions)
    {
        if (it->CheckCondition(deltaTime))
            return it->GetJumpState();
    }

    return This<AIState>();
}

void AIState::SetName(const std::string& name)
{
    _name = name;
}

const std::string& AIState::GetName() const
{
    return _name;
}

void AIState::OnExit()
{
    if (_callback[AI_EVENT_STATE::EXIT])
        _callback[AI_EVENT_STATE::EXIT]();
}

void AIState::OnEnter()
{
    if (_callback[AI_EVENT_STATE::ENTER])
        _callback[AI_EVENT_STATE::ENTER]();
}

void AIState::OnTick()
{
    if (_callback[AI_EVENT_STATE::TICK])
        _callback[AI_EVENT_STATE::TICK]();
}

void AIState::Destroy()
{
    for (auto& it : _transitions)
        DESTROY(it);
}

#include "pch.h"
#include "AITransition.h"
#include "AIComponent.h"
#include "AIState.h"

AITransition::AITransition()
{}

AITransition::~AITransition()
{}

bool AITransition::CheckCondition(float deltaTime)
{
    if (_conditions.size() < 1)
        return false;

    if (_rule == eTransitionRule::AND)
    {
        //전부 성공해야 true이기 때문에
        //하나라도 실패하면 false를 리턴한다.
        for (auto& it : _conditions)
        {
            if (!it())
                return false;
        }

        return true;
    }
    else
    {
        //하나라도 성공한다면
        for (auto& it : _conditions)
        {
            if (it())
                return true;
        }

        return false;
    }

    return true;
}

bool AITransition::Init(const std::string& name, Ptr<AIState> jumpState, eTransitionRule rule)
{
    _name = name;
    _jumpState = jumpState;
    _rule = rule;

    return true;
}

void AITransition::Destroy()
{

}

void AITransition::SetName(const std::string& name)
{
    _name = name;
}

const std::string& AITransition::GetName() const
{
    return _name;
}

Ptr<class AIState> AITransition::GetJumpState() const
{
    return _jumpState;
}

void AITransition::SetJumpState(Ptr<class AIState> state)
{
    _jumpState = state;
}

void AITransition::SetTransitionRule(eTransitionRule rule)
{
    _rule = rule;
}

#include "pch.h"
#include "MachineBase.h"

#include "Component/AIComponent.h"

MachineBase::MachineBase()
{}

MachineBase::~MachineBase()
{}

void MachineBase::Init(Ptr<class AIComponent> owner)
{
    _owner = owner;
}

void MachineBase::Tick(float deltaTime)
{
    if (!_curState) return;

    _curInterval += deltaTime;

    if (_curInterval < _interval)
        return;

    Ptr<AIState> state = _curState->Tick(deltaTime);
    if (state)
    {
        if (state->GetName() == _curState->GetName())
        {
            _curState->OnTick();
        }
        else
        {
            _curState->OnExit();
            state->OnEnter();
            _curState = state;

            OnChangedState();
        }
    }

    _curInterval -= _interval;
}

void MachineBase::Destroy()
{
    DESTROY(_board);

    for (auto& it : _stateFinder)
        DESTROY(it.second);

    _stateFinder.clear();
}

Ptr<class AIComponent> MachineBase::GetAIComponent() const
{
    return Lock<AIComponent>(_owner);
}

void MachineBase::OnChangedState()
{}

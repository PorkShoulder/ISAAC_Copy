#pragma once
#include "ActorComponent.h"

#include "AIState.h"
#include "AITransition.h"
#include "MachineBase.h"

#include <unordered_map>

class AIComponent : public ActorComponent
{
public:
    AIComponent();
    virtual ~AIComponent();

protected:
    Weak<class AIController> _controller;
    Ptr<class MachineBase> _machine;

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Destroy() override;

    void SetController(Ptr<class AIController> controller);
    Ptr<class AIController> GetController() const;

    Ptr<class MachineBase> GetAIStateMachine();

    template<typename T>
    void CreateAIStateMachine()
    {
        if (_machine)
            return;

        _machine = New<T>();
        _machine->Init(This<AIComponent>());
    }
};


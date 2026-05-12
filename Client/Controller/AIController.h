#pragma once
#include "Controller.h"

class AIController : public Controller
{
public:
    AIController() = default;
    virtual ~AIController() = default;
    AIController(const AIController&) = delete;
    AIController(AIController&&) = delete;
    AIController& operator = (const AIController&) = delete;
    AIController& operator = (AIController&&) = delete;

private:
    Ptr<class AIComponent> _aiComponent;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;

    Ptr<class AIComponent> GetAI() const;
    Ptr<class MachineBase> GetAIStateMachine() const;
};


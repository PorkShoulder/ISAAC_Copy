#pragma once
#include "Controller.h"
#include "Component/InputComponent.h"

//Controller를 상속받고
//Ptr<InputComponent> _input을 생성하고 관리해주는 컨트롤러
//
class PlayerController : public Controller
{
public:
    PlayerController() = default;
    virtual ~PlayerController() = default;
    PlayerController(const PlayerController&) = delete;
    PlayerController(PlayerController&&) = delete;
    PlayerController& operator = (const PlayerController&) = delete;
    PlayerController& operator = (PlayerController&&) = delete;

public:
    Ptr<InputComponent> _input;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;

    Ptr<InputComponent> GetInputComponent() const;
};


#pragma once
#include "../Core/Object.h"


//입력의 행위를 구분
//움직이기, 공격하기
class InputAction : public Object
{
    friend class InputSystem;

public:
    InputAction() = default;
    virtual ~InputAction() = default;
    InputAction(const InputAction&) = delete;
    InputAction(InputAction&&) = delete;
    InputAction& operator= (const InputAction&) = delete;
    InputAction& operator= (InputAction&&) = delete;

private:
    std::string _name;

public:
    void SetName(const std::string& name);
    const std::string& GetName() const;

    virtual void Destroy() override;
};


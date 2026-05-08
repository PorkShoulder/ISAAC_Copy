#pragma once
#include "Core/Object.h"
#include "InputAction.h"

#include <unordered_map>

//이전에 입력 버그가 뭐였냐면
//A InputContext <--- B InputAction을 바인딩하고 함수도 다 바인딩 잘 하고나서
//다른 클래스에서 A InputContext <--- C InputAction을 바인딩하면 바인딩이 안되는 버그가
//있었어요...

//InputContext --- FInputMapping(InputAction + key)
//FInputMapping <--- 얘는 InputComponent 에서 포인터로 가지고 있으면서,
//확인하고 키입력에 따른 함수를 호출해줘야 한다면 호출해주는 구조를 가지고 있었다.


//입력의 행위 와 실제물리적인 키를 엮어주는 구조체
struct FInputMapping
{
    Ptr<InputAction> _action;               //행위
    bool _state[INPUT_TYPE::END] = {};      //키가 어떤 상태인지
    uint8 _key = 0;                             //실제 하드웨어 키(물리적인 키)
};

class InputContext : public Object
{
    friend class InputSystem;
    friend class InputComponent;

public:
    InputContext() = default;
    virtual ~InputContext() = default;
    InputContext(const InputContext&) = delete;
    InputContext(InputContext&&) = delete;
    InputContext& operator= (const InputContext&) = delete;
    InputContext& operator= (InputContext&&) = delete;

private:
    std::string _name;
    std::unordered_map<std::string, FInputMapping> _mappings;

public:
    const std::string& GetName() const;
    void SetName(const std::string& name);
    void BindInputAction(Ptr<InputAction> action, uint8 key);
    void ChangeInputActionKey(const std::string& actionName, uint8 key);
    FInputMapping* FindMapping(const std::string& name);

    virtual void Destroy() override;
};


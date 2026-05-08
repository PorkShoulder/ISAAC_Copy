#include "pch.h"
#include "InputContext.h"
#include "InputSystem.h"

const std::string& InputContext::GetName() const
{
    return _name;
}

void InputContext::SetName(const std::string& name)
{
    _name = name;
}


//새로운 액션이 바인딩 될때마다
//InputComponent 중에 해당 컨텍스트를 사용하는 얘가 있다면
//InputComponent 의 FBindAction목록 도 갱신해준다.
//FBindAction <---여기에 FInputMapping이 있으니까
void InputContext::BindInputAction(Ptr<InputAction> action, uint8 key)
{
    auto it = _mappings.find(action->GetName());
    if (_mappings.end() != it)
        return;

    FInputMapping mapping;
    mapping._action = action;
    mapping._key = InputSystem::Instance().ConvertKey(key);

    _mappings[action->GetName()] = mapping;
    InputSystem::Instance().RefreshInputComponent(_name, _mappings[action->GetName()]);
}

void InputContext::ChangeInputActionKey(const std::string& actionName, uint8 key)
{
    auto it = _mappings.find(actionName);
    if (_mappings.end() != it)
        _mappings[actionName]._key = InputSystem::Instance().ConvertKey(key);
}

FInputMapping* InputContext::FindMapping(const std::string& name)
{
    auto it = _mappings.find(name);
    if (_mappings.end() == it)
        return nullptr;

    return &(it->second);
}

void InputContext::Destroy()
{
    _mappings.clear();
}

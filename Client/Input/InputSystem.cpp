#include "pch.h"
#include "InputSystem.h"
#include "InputAction.h"
#include "InputContext.h"
#include "Input.h"
#include "Component/InputComponent.h"

bool InputSystem::Init(Ptr<class Input> input)
{
    _input = input;

    //원한다면 미리 컨텐스트 나 액션을 만들어도 됩니다.

    return true;
}

void InputSystem::Tick(float deltaTime)
{
    for (auto& it : _activeContexts)
    {
        for (auto& subIt : it.second->_mappings)
        {
            FInputMapping& mapping = subIt.second;
            uint8 key = mapping._key;

            if (key == DIK_MOUSELBUTTON)
            {
                mapping._state[INPUT_TYPE::DOWN] = _input->GetMouseDown(MOUSE_BUTTON_TYPE::LButton);
                mapping._state[INPUT_TYPE::HOLD] = _input->GetMouseHold(MOUSE_BUTTON_TYPE::LButton);
                mapping._state[INPUT_TYPE::UP] = _input->GetMouseUp(MOUSE_BUTTON_TYPE::LButton);
            }
            else if (key == DIK_MOUSERBUTTON)
            {
                mapping._state[INPUT_TYPE::DOWN] = _input->GetMouseDown(MOUSE_BUTTON_TYPE::RButton);
                mapping._state[INPUT_TYPE::HOLD] = _input->GetMouseHold(MOUSE_BUTTON_TYPE::RButton);
                mapping._state[INPUT_TYPE::UP] = _input->GetMouseUp(MOUSE_BUTTON_TYPE::RButton);
            }
            else if (key == DIK_MOUSEWHEEL)
            {
                mapping._state[INPUT_TYPE::DOWN] = _input->GetMouseDown(MOUSE_BUTTON_TYPE::Wheel);
                mapping._state[INPUT_TYPE::HOLD] = _input->GetMouseHold(MOUSE_BUTTON_TYPE::Wheel);
                mapping._state[INPUT_TYPE::UP] = _input->GetMouseUp(MOUSE_BUTTON_TYPE::Wheel);
            }
            else
            {
                bool keyPush = _input->GetKeyState(key);

                if (keyPush)
                {
                    if (!mapping._state[INPUT_TYPE::DOWN] && !mapping._state[INPUT_TYPE::HOLD])
                    {
                        mapping._state[INPUT_TYPE::DOWN] = true;
                        mapping._state[INPUT_TYPE::HOLD] = true;
                    }
                    else
                    {
                        mapping._state[INPUT_TYPE::DOWN] = false;
                    }
                }
                else
                {
                    if (mapping._state[INPUT_TYPE::HOLD])
                    {
                        mapping._state[INPUT_TYPE::DOWN] = false;
                        mapping._state[INPUT_TYPE::HOLD] = false;
                        mapping._state[INPUT_TYPE::UP] = true;
                    }
                    else
                    {
                        mapping._state[INPUT_TYPE::UP] = false;
                    }
                }
            }
        }
    }
}

void InputSystem::AddActiveInputContext(Ptr<class InputContext> context)
{
    _activeContexts[context->_name] = context;
}

void InputSystem::RemoveActiveContext(const std::string& name)
{
    _activeContexts.erase(name);
}

Ptr<class InputContext> InputSystem::FindOrAddInputContext(const std::string& name)
{
    auto it = _allContexts.find(name);
    if (_allContexts.end() != it)
        return it->second;

    Ptr<InputContext> context = New<InputContext>();
    context->_name = name;
    _allContexts[name] = context;

    return context;
}

Ptr<class InputAction> InputSystem::FindOrAddInputAction(const std::string& name)
{
    auto it = _allActions.find(name);
    if (_allActions.end() != it)
        return it->second;

    Ptr<InputAction> context = New<InputAction>();
    context->_name = name;
    _allActions[name] = context;

    return context;
}

bool InputSystem::IsActiveContext(const std::string& name)
{
    return _activeContexts.contains(name);
}

uint8 InputSystem::ConvertKey(uint8 key)
{
    return _input->ConvertKey(key);
}

void InputSystem::RegisterInputComponent(int32 actorID, Ptr<class InputComponent> inputComp)
{
    //InputComponent가 결국 컴포넌트니까
    //하나의 InputComponent가 고유한 키를 가지려면
    //InputComponent --> ID, 해당 ActorID : 두 ID를 조합하면 새로운 고유키를 만들수 
    //있으니 그걸 고유키로 해서 inputSystem에서 관리한다.
    std::pair<int32, int32> key(actorID, inputComp->GetComponentID());

    _inputComponents[key] = inputComp;
}

void InputSystem::RemoveInputComponent(int32 actorID, int32 componentID)
{
    std::pair<int32, int32> key(actorID, componentID);
    _inputComponents.erase(key);
}

void InputSystem::RefreshInputComponent(const std::string& contextName, const FInputMapping& mapping)
{
    for (auto it : _inputComponents)
    {
        Ptr<InputComponent> comp = Lock<InputComponent>(it.second);
        if (nullptr == comp)
            continue;

        comp->RefreshInputMapping(contextName, mapping);
    }
}

const FVector2D& InputSystem::GetMouseWorldPos() const
{
    return _input->GetMouseWorldPos();
}

const FVector2D& InputSystem::GetMousePos() const
{
    return _input->GetMousePos();
}

const FVector2D& InputSystem::GetMouseMove() const
{
    return _input->GetMouseMove();
}

bool InputSystem::GetMouseDown(MOUSE_BUTTON_TYPE::Type type)
{
    return _input->GetMouseDown(type);
}

bool InputSystem::GetMouseHold(MOUSE_BUTTON_TYPE::Type type)
{
    return _input->GetMouseHold(type);
}

bool InputSystem::GetMouseUp(MOUSE_BUTTON_TYPE::Type type)
{
    return _input->GetMouseUp(type);
}

void InputSystem::Destroy()
{
    _activeContexts.clear();

    for (auto& it : _allContexts)
        DESTROY(it.second);

    for (auto& it : _allActions)
        DESTROY(it.second);

    _allContexts.clear();
    _allActions.clear();
}

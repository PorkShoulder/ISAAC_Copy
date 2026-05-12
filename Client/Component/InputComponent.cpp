#include "pch.h"
#include "InputComponent.h"

bool InputComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    ActorComponent::Init(id, name, owner);

    return true;
}

void InputComponent::Tick(float deltaTime)
{
    //(키 입력(down, hold, up)에 따른 함수를 호출한다.)

    ActorComponent::Tick(deltaTime);

    auto it = _binds.find(_activeContext);
    if (it == _binds.end())
        return;

    for (auto& bindIt : it->second)
    {
        FInputMapping* mapping = bindIt.second._mapping;
        if (!mapping)
            continue;

        for (int i = 0; i < INPUT_TYPE::END; ++i)
        {
            if (mapping->_state[i] && bindIt.second._callBack[i])
                bindIt.second._callBack[i](deltaTime);
        }
    }

}

void InputComponent::Destroy()
{
    _binds.clear();
}

void InputComponent::SetActiveContext(const std::string & name)
{
    _activeContext = name;
}

const std::string& InputComponent::GetActiveContext() const
{
    return _activeContext;
}

void InputComponent::AddInputContext(const std::string& name)
{
    if (_binds.find(name) != _binds.end())
        return;

    Ptr<InputContext> foundCont = InputSystem::Instance().FindOrAddInputContext(name);
    if (!foundCont)
        return;

    for (auto& it : foundCont->_mappings)
    {
        FBindAction bind;
        bind._mapping = &it.second;
        bind._name = bind._mapping->_action->GetName();

        _binds[name][bind._name] = bind;
    }

    if (_activeContext.empty())
        _activeContext = name;

    InputSystem::Instance().AddActiveInputContext(foundCont);
}

void InputComponent::RemoveInputContext(const std::string & name)
{
    _binds.erase(name);

    InputSystem::Instance().RemoveActiveContext(name);
}

void InputComponent::RefreshInputMapping(const std::string& contextName, const FInputMapping& mapping)
{
    auto it = _binds.find(contextName);
    if (_binds.end() == it)
        return;
    
    auto& itArr = it->second;
    auto foundItArr = itArr.find(mapping._action->GetName());
    if (itArr.end() != foundItArr)
    {
        FBindAction& bindAction = foundItArr->second;
        bindAction._mapping = const_cast<FInputMapping*>(&mapping);
    }
    else
    {
        FBindAction bind;
        bind._mapping = const_cast<FInputMapping*>(&mapping);
        bind._name = bind._mapping->_action->GetName();

        _binds[contextName][bind._name] = bind;
    }
}



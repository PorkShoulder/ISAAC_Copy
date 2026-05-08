#pragma once
#include "ActorComponent.h"
#include "Input/InputContext.h"
#include "Input/InputSystem.h"

#include <functional>

//키를 눌렀을때 호출해줄 함수와 입력 액션을 묶는 구조체
struct FBindAction
{
    std::string _name;
    FInputMapping* _mapping = nullptr;
    std::function<void(float)> _callBack[INPUT_TYPE::END];
};

class InputComponent : public ActorComponent
{
public:
    InputComponent() = default;
    virtual ~InputComponent() = default;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, FBindAction>> _binds;
    std::string _activeContext;

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;

    virtual void Destroy() override;

    void SetActiveContext(const std::string& name);
    const std::string& GetActiveContext() const;
    void AddInputContext(const std::string& name);
    void RemoveInputContext(const std::string& name);
    void RefreshInputMapping(const std::string& contextName, const FInputMapping& mapping);



    //람다 혹은 객체가 필요없는 함수 받기
    template<typename T>
    void BindAction(const std::string& contextName, const std::string& actionName, INPUT_TYPE::eType type, T&& func)
    {
        auto it = _binds.find(contextName);
        if (_binds.end() == it)
            return;

        _binds[contextName][actionName]._callBack[type] = std::forward<T>(func);

        //for (auto& bindIt : it->second)
        //{
        //    if (bindIt._mapping->_action->GetName() == actionName)
        //    {
        //        bindIt._callBack[type] = std::forward<T>(func);
        //        break;
        //    }
        //}
    }

    //멤버함수 포인터 바인딩
    template<typename T>
    void BindAction(const std::string& contextName, const std::string& actionName, INPUT_TYPE
        ::eType type, T* obj, void (T::* memFunc)(float))
    {
        auto it = _binds.find(contextName);
        if (_binds.end() == it)
            return;

        _binds[contextName][actionName]._callBack[type] = std::bind(memFunc, obj, std::placeholders::_1);
        /*for (auto& bindIt : it->second)
        {
            if (bindIt._mapping->_action->GetName() == actionName)
            {
                bindIt._callBack[type] = std::bind(memFunc, obj, std::placeholders::_1);
                break;
            }
        }*/
    }
};


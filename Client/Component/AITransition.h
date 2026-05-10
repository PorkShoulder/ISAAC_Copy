#pragma once
#include "../Core/Object.h"

#include <functional>

//AI 다른 상태로 넘어가기 위한 조건을 관리하는 클래스
class AITransition : public Object
{
    friend class AIState;
    friend class AIComponent;
    
public:
    AITransition();
    virtual ~AITransition();

protected:
    //조건의 이름(ex IDLE_MOVE, MOVE_IDLE, IDLE_ATTACK..)
    std::string _name;

    //조건을 어떤 방식으로 체크할지 규칙, OR AND
    eTransitionRule _rule = eTransitionRule::OR;
    
    //다른 상태로 넘어가기 위한 "조건들" 을 체크
    //몬스터 IDLE --> ATTACK
    //몬스터가 선공 몬스터, 플레이어가 일정 범위 이내, 몬스터가 근접인지? 원거리인지?, 마법쓸껀지? 평타쓸껀지? 
    std::vector<std::function<bool()>> _conditions;

    //_conditions 충족을 한다면 전환 될 상태
    Ptr<class AIState> _jumpState;

protected:
    virtual bool CheckCondition(float deltaTime);

public:
    bool Init(const std::string& name, Ptr<AIState> jumpState, eTransitionRule rule);
    virtual void Destroy() override;

public:
    void SetName(const std::string& name);
    const std::string& GetName() const;

    Ptr<class AIState> GetJumpState() const;
    void SetJumpState(Ptr<class AIState> state);

    void SetTransitionRule(eTransitionRule rule);

public:
    template<typename T>
    void AddCondition(T&& func)
    {
        _conditions.push_back(std::forward<T>(func));
    }

    template<typename T>
    void AddCondition(T* obj, bool (T::* memFunc)())
    {
        _conditions.push_back(std::bind(memFunc, obj));
    }
};


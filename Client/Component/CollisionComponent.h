#pragma once
#include "SceneComponent.h"

#include <functional>
#include <map>

class CollisionComponent : public SceneComponent
{
    friend class CollisionManager;

public:
    CollisionComponent();
    virtual ~CollisionComponent();

protected:
    //충돌체 모양
    eCollisionShape _shape = eCollisionShape::End;
    FVector3D _min;
    FVector3D _max;

    //충돌체가 사용할 고유ID
    //액터 + 컴포넌트ID
    std::pair<int32, int32> _colliderID;

    //이 충돌컴포넌트가 사용할 프로파일
    Ptr<class CollisionProfile> _profile;

    //충돌했을 때 호출해줄 함수들
    std::function<void(Weak<CollisionComponent>)> _collisionCallBack[eCollisionState::COLLISION_STATE_END];

    //다른 충돌컴포넌트와의 충돌상태를 저장
    std::map<std::pair<int32, int32>, eCollisionState> _colliderReactions;

#if _DEBUG
    //todo : debug collision
    Ptr<class TranformCBuffer> _transformCBuffer = nullptr;
    Ptr<class ColorCBuffer> _colorCBuffer = nullptr;
    Ptr<class Mesh>         _mesh = nullptr;
    Ptr<class Shader>       _shader = nullptr;
#endif // _DEBUG

public:
    eCollisionShape GetShape() const { return _shape; }

    //다른 충돌체ID를 넣어주면 해당 충돌체랑 나의 충돌상태를 알려준다.
    eCollisionState CheckState(std::pair<int32, int32>& destKey);

    //충돌체ID를 반환
    const std::pair<int32, int32>& GetColliderID() const;

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTiem) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;

    //다른 충돌체랑 충돌 계산 함수
    //가상함수
    virtual bool Collision(Weak<CollisionComponent> dest);

    //사용할 충돌 프로파일을 지정
    void SetCollisionProfile(const std::string& name);
    const Ptr<class CollisionProfile> GetProfile() const;

private:
    //충돌 상태에 따라서 함수를 호출해준다.
    void Invoke(eCollisionState state, Weak<CollisionComponent> dest, const std::pair<int32, int32>& destKey);

public:
    template<typename T>
    void SetCollisionCallBack(eCollisionState state, T* obj, void (T::* memFunc)(Weak<CollisionComponent>))
    {
        _collisionCallBack[state] = std::bind(memFunc, obj, std::placeholders::_1);
    }

    template<typename T>
    void SetCollisionCallBack(eCollisionState state, T&& func)
    {
        _collisionCallBack[state] = std::forward<T>(func);
    }
};


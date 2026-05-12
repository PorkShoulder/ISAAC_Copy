#include "pch.h"
#include "Component.h"
#include "../Object/Actor.h"
#include "../World/Level.h"


Component::Component()
{
}

Component::~Component()
{
}

const Ptr<class Actor> Component::GetOwner() const
{
    // 약참조로 저장한 변수를 shared_ptr로 확인할때는 lock() 함수를 사용하여 접근.
    return Lock<Actor>(_owner);
}

void Component::SetOwner(Ptr<class Actor> owner)
{
    // weak_ptr로 선언되어 있는 변수는 그냥 shared_ptr로 대입해도 된다.
    // 이러헤 그냥 대입해도 shared_ptr의 refCount는 증가하지 않음.
    _owner = owner;
}

Ptr<class Level> Component::GetLevel() const
{
    return Lock<Level>(_level);
}

bool Component::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    _id = id;
    _name = name;
    _owner = owner;

    _level = owner->GetLevel();

    SetEnable(true);
    SetActive(true);

    return true;

}

void Component::Tick(float deltaTime)
{
}

void Component::Collision(float deltaTiem)
{
}

void Component::Render(float deltaTime)
{
}

void Component::Destroy()
{
    SetEnable(false);
    SetActive(false);
}

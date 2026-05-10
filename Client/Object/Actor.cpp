#include "pch.h"
#include "Actor.h"

#include "../World/Level.h"
#include "../Component/SceneComponent.h"
#include "../Component/ActorComponent.h"

// #include "Editor/EditorEngine.h"


bool Actor::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    _id = id;

    _root = CreateSceneComponent<SceneComponent>("DefaultRoot");

    SetRelativePosition(pos);
    SetRelativeScale(scale);
    SetRelativeRotation(rot);

    SetEnable(true);
    SetActive(true);

    return true;
}

void Actor::Tick(float deltaTime)
{
    _root->Tick(deltaTime);

    //액터컴포넌트의 tick은 씬컴포넌트들의 tick보다 늦게 돌린다.
    //이유는 씬컴포넌트에서 충돌이라던가 다른 반응이 일어날껀데
    //그거 결과 받아서 처리할 일이 있을수도 있어서
    for (auto& it : _actorComponents)
    {
        Ptr<ActorComponent> comp = it.second;
        if (nullptr == comp)
            continue;

        if (!comp->IsActive() || !comp->IsEnable())
            continue;

        comp->Tick(deltaTime);
    }
}

void Actor::Collision(float deltaTime)
{

}

void Actor::Render(float deltaTime)
{
    //_root->Render(deltaTime);
}

//void Actor::DrawInspector()
//{
//    ImGui::SeparatorText("World Transform");
//
//    FVector3D worldPos = GetWorldPosition();
//    if (ImGui::DragFloat3("World Position", &worldPos._x, 0.5f))
//        SetWorldPosition(worldPos);
//
//    FVector3D worldRot = GetWorldRotation();
//    if (ImGui::DragFloat3("World Rotation", &worldRot._x, 0.5f))
//        SetWorldPosition(worldRot);
//
//    FVector3D worldScale = GetWorldScale();
//    if (ImGui::DragFloat3("World Scale", &worldScale._x, 0.5f))
//        SetWorldPosition(worldScale);
//
//    ImGui::SeparatorText("Relative Transform");
//
//    FVector3D relativePos = GetWorldPosition();
//    if (ImGui::DragFloat3("Relative Position", &relativePos._x, 0.1f))
//        SetWorldPosition(relativePos);
//
//    FVector3D relativeRot = GetWorldRotation();
//    if (ImGui::DragFloat3("Relative Rotation", &relativeRot._x, 0.1f))
//        SetWorldPosition(relativeRot);
//
//    FVector3D relativeScale = GetWorldScale();
//    if (ImGui::DragFloat3("Relative Scale", &relativeScale._x, 0.1f))
//        SetWorldPosition(relativeScale);
//
//    bool active = IsActive();
//    if (ImGui::Checkbox("Active", &active))
//        SetActive(active);
//
//    bool enable = IsEnable();
//    if (ImGui::Checkbox("Enable", &enable))
//        SetEnable(enable);
//}

Ptr<class SceneComponent> Actor::GetRoot() const
{
    return _root;
}

void Actor::SetRootComponent(const Ptr<class SceneComponent> comp)
{
    if (_root) //루트컴포넌트로 다른 애가 있을때
    {
        //기존 루트컴포넌트의 정보를 새로 들어온 컴포넌트에 옮겨준다.

        comp->SetRelativeTransform(_root->GetRelativeTransform());

        for (auto& it : _root->_childs)
        {
            Ptr<SceneComponent> child = it.second;
            if (child)
            {
                child->AttachToComponent(comp);
            }
        }

        _root->_childs.clear();

        Delete(_root);

        _root = comp;
    }
    else // _root == nullptr
    {
        _root = comp;
    }
}

Ptr<class SceneComponent> Actor::FindSceneComponent(const std::string& name) const
{
    if (_root->GetName() == name)
        return _root;

    auto it = _componentFinder.find(name);
    if (_componentFinder.end() == it)
        return nullptr;

    return FindSceneComponent(it->second);
}

Ptr<class ActorComponent> Actor::FindActorComponent(const std::string& name) const
{
    auto it = _componentFinder.find(name);
    if (_componentFinder.end() == it)
        return nullptr;

    return FindActorComponent(it->second);
}

Ptr<class SceneComponent> Actor::FindSceneComponent(int32 id) const
{
    if (_root->GetComponentID() == id)
        return _root;

    return _root->FindComponent(id);
}

Ptr<class ActorComponent> Actor::FindActorComponent(int32 id) const
{
    auto compIt = _actorComponents.find(id);
    if (_actorComponents.end() == compIt)
        return nullptr;

    return compIt->second;
}

Ptr<class Level> Actor::GetLevel() const
{
    return Lock<Level>(_level);
}

void Actor::SetLevel(Ptr<class Level> level)
{
    _level = level;
}

void Actor::Remove()
{
    Ptr<Level> level = Lock<Level>(_level);
    if (level)
        level->RemoveActor(_id);
}

bool Actor::IsTag(const std::string& tag)
{
    return _tags.contains(tag);
}

void Actor::AddTag(const std::string& tag)
{
    _tags.insert(tag);
    //태그 매니저에서도 넣어줘야 함.

    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    level->AddTag(tag, _id);
}

//액터의 트랜스폼 정보는 root(sceneComponent)의 트랜스폼 정보
const FVector3D& Actor::GetWorldPosition() const
{
    return _root->GetWorldPosition();
}

const FVector3D& Actor::GetWorldScale() const
{
    return _root->GetWorldScale();
}

const FRotator& Actor::GetWorldRotation() const
{
    return _root->GetWorldRotation();
}

void Actor::SetWorldRotation(const FRotator& rot)
{
    _root->SetWorldRotation(rot);
}

void Actor::SetWorldRotation(float x, float y, float z)
{
    _root->SetWorldRotation(x, y, z);
}

void Actor::SetWorldRotation(const FVector2D& rot)
{
    _root->SetWorldRotation(rot);
}

void Actor::SetWorldRotation(float x, float y)
{
    _root->SetWorldRotation(x, y);
}

const FTransform& Actor::GetWorldTransform() const
{
    return _root->GetWorldTransform();
}

void Actor::SetWorldPosition(const FVector3D& pos)
{
    _root->SetWorldPosition(pos);
}

void Actor::SetWorldPosition(float x, float y, float z)
{
    _root->SetWorldPosition(x, y, z);
}

void Actor::SetWorldPosition(const FVector2D& pos)
{
    _root->SetWorldPosition(pos);
}

void Actor::SetWorldPosition(float x, float y)
{
    _root->SetWorldPosition(x, y);
}

const FVector3D& Actor::AddWorldPosition(const FVector3D& pos)
{
    _root->AddWorldPosition(pos);

    return _root->_world._position;
}

const FVector3D& Actor::AddWorldPosition(float x, float y, float z)
{
    _root->AddWorldPosition(x, y, z);

    return _root->_world._position;
}

const FVector3D& Actor::AddWorldPosition(const FVector2D& pos)
{
    _root->AddWorldPosition(pos);

    return _root->_world._position;
}

const FVector3D& Actor::AddWorldPosition(float x, float y)
{
    _root->AddWorldPosition(x, y);

    return _root->_world._position;
}

const FVector3D& Actor::AddWorldPosition(float value)
{
    _root->AddWorldPosition(value);

    return _root->_world._position;
}

const FVector3D& Actor::AddWorldPositionX(float x)
{
    _root->AddWorldPositionX(x);

    return _root->_world._position;
}

const FVector3D& Actor::AddWorldPositionY(float y)
{
    _root->AddWorldPositionY(y);

    return _root->_world._position;
}

void Actor::SetWorldScale(const FVector3D& scale)
{
    _root->SetWorldScale(scale);
}

void Actor::SetWorldScale(float x, float y, float z)
{
    _root->SetWorldScale(x, y, z);
}

void Actor::SetWorldScale(const FVector2D& scale)
{
    _root->SetWorldScale(scale);
}

void Actor::SetWorldScale(float x, float y)
{
    _root->SetWorldScale(x, y);
}

const FVector3D& Actor::AddWorldScale(const FVector3D& scale)
{
    return _root->AddWorldScale(scale);
}

const FVector3D& Actor::AddWorldScale(float x, float y, float z)
{
    return _root->AddWorldScale(x, y, z);
}

const FVector3D& Actor::AddWorldScale(const FVector2D& scale)
{
    return _root->AddWorldScale(scale);
}

const FVector3D& Actor::AddWorldScale(float x, float y)
{
    return _root->AddWorldScale(x, y);
}

const FVector3D& Actor::AddWorldScale(float value)
{
    return _root->AddWorldScale(value);
}

const FVector3D& Actor::AddWorldScaleX(float x)
{
    return _root->AddWorldScaleX(x);
}

const FVector3D& Actor::AddWorldScaleY(float y)
{
    return _root->AddWorldScaleY(y);
}

void Actor::SetWorldTransform(const FTransform& tran)
{
    _root->SetWorldTransform(tran);
}

void Actor::SetWorldTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    _root->SetWorldTransform(pos, scale, rot);
}

const FVector3D& Actor::GetRelativePosition() const
{
    return _root->GetRelativePosition();
}

const FVector3D& Actor::GetRelativeScale() const
{
    return _root->GetRelativeScale();
}

const FRotator& Actor::GetRelativeRotation() const
{
    return _root->GetRelativeRotation();
}

void Actor::SetRelativeRotation(const FRotator& rot)
{
    _root->SetRelativeRotation(rot);
}

void Actor::SetRelativeRotation(float x, float y, float z)
{
    _root->SetRelativeRotation(x, y, z);
}

void Actor::SetRelativeRotation(const FVector2D& rot)
{
    _root->SetRelativeRotation(rot);
}

void Actor::SetRelativeRotation(float x, float y)
{
    _root->SetRelativeRotation(x, y);
}

const FRotator& Actor::AddRelativeRotation(const FVector3D& scale)
{
    return _root->AddRelativeRotation(scale);
}

const FRotator& Actor::AddRelativeRotation(float x, float y, float z)
{
    return _root->AddRelativeRotation(x, y, z);
}

const FRotator& Actor::AddRelativeRotation(const FVector2D& scale)
{
    return _root->AddRelativeRotation(scale);
}

const FRotator& Actor::AddRelativeRotation(float x, float y)
{
    return _root->AddRelativeRotation(x, y);
}

const FRotator& Actor::AddRelativeRotation(float value)
{
    return _root->AddRelativeRotation(value);
}

const FRotator& Actor::AddRelativeRotationX(float x)
{
    return _root->AddRelativeRotationX(x);
}

const FRotator& Actor::AddRelativeRotationY(float y)
{
    return _root->AddRelativeRotationY(y);
}

const FTransform& Actor::GetRelativeTransform() const
{
    return _root->GetRelativeTransform();
}

void Actor::SetRelativePosition(const FVector3D& pos)
{
    _root->SetRelativePosition(pos);
}

void Actor::SetRelativePosition(float x, float y, float z)
{
    _root->SetRelativePosition(x, y, z);
}

void Actor::SetRelativePosition(const FVector2D& pos)
{
    _root->SetRelativePosition(pos);
}

void Actor::SetRelativePosition(float x, float y)
{
    _root->SetRelativePosition(x, y);
}

const FVector3D& Actor::AddRelativePosition(const FVector3D& scale)
{
    return _root->AddRelativePosition(scale);
}

const FVector3D& Actor::AddRelativePosition(float x, float y, float z)
{
    return _root->AddRelativePosition(x, y, z);
}

const FVector3D& Actor::AddRelativePosition(const FVector2D& scale)
{
    return _root->AddRelativePosition(scale);
}

const FVector3D& Actor::AddRelativePosition(float x, float y)
{
    return _root->AddRelativePosition(x, y);
}

const FVector3D& Actor::AddRelativePosition(float value)
{
    return _root->AddRelativePosition(value);
}

const FVector3D& Actor::AddRelativePositionX(float x)
{
    return _root->AddRelativePositionX(x);
}

const FVector3D& Actor::AddRelativePositionY(float y)
{
    return _root->AddRelativePositionY(y);
}

void Actor::SetRelativeScale(const FVector3D& scale)
{
    _root->SetRelativeScale(scale);
}

void Actor::SetRelativeScale(float x, float y, float z)
{
    _root->SetRelativeScale(x, y, z);
}

void Actor::SetRelativeScale(const FVector2D& scale)
{
    _root->SetRelativeScale(scale);
}

void Actor::SetRelativeScale(float x, float y)
{
    _root->SetRelativeScale(x, y);
}

const FVector3D& Actor::AddRelativeScale(const FVector3D& scale)
{
    return _root->AddRelativeScale(scale);
}

const FVector3D& Actor::AddRelativeScale(float x, float y, float z)
{
    return _root->AddRelativeScale(x, y, z);
}

const FVector3D& Actor::AddRelativeScale(const FVector2D& scale)
{
    return _root->AddRelativeScale(scale);
}

const FVector3D& Actor::AddRelativeScale(float x, float y)
{
    return _root->AddRelativeScale(x, y);
}

const FVector3D& Actor::AddRelativeScale(float value)
{
    return _root->AddRelativeScale(value);
}

const FVector3D& Actor::AddRelativeScaleX(float x)
{
    return _root->AddRelativeScaleX(x);
}

const FVector3D& Actor::AddRelativeScaleY(float y)
{
    return _root->AddRelativeScaleY(y);
}

void Actor::SetRelativeTransform(const FTransform& tran)
{
    _root->SetRelativeTransform(tran);
}

void Actor::SetRelativeTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    _root->SetRelativeTransform(pos, scale, rot);
}

void Actor::Destroy()
{
    DESTROY(_root)

        for (auto& it : _actorComponents)
            DESTROY(it.second)

            _actorComponents.clear();
    _componentFinder.clear();
}


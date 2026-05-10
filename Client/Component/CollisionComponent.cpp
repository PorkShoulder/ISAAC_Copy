#include "pch.h"
#include "CollisionComponent.h"
#include "Collision/CollisionProfileManager.h"

#include "Object/Actor.h"

#include "World/Level.h"

#include "Shader/ShaderManager.h"
#include "Shader/TranformCBuffer.h"
#include "Shader/ColorCBuffer.h"
#include "Core/Mesh.h"

CollisionComponent::CollisionComponent()
{
#ifdef _DEBUG
    _isRender = true;
    _renderLayerName = "Collider";
#endif // _DEBUG

}

CollisionComponent::~CollisionComponent()
{}

bool CollisionComponent::Collision(Weak<CollisionComponent> dest)
{
    return false;
}

void CollisionComponent::SetCollisionProfile(const std::string& name)
{
    Ptr<CollisionProfile> foundVal = CollisionProfileManager::Instance().FindProfile(name);
    if (nullptr == foundVal)
        return;

    _profile = foundVal;
}

const Ptr<class CollisionProfile> CollisionComponent::GetProfile() const
{
    return _profile;
}

void CollisionComponent::Invoke(eCollisionState state, Weak<CollisionComponent> dest, const std::pair<int32, int32>& destKey)
{
    Ptr<CollisionComponent> destCol = Lock<CollisionComponent>(dest);
    if (nullptr == destCol)
        return;

    //여기 들어왔다는것은 뭔가와 충돌을 했거나 혹은 충돌 중
    if (state == eCollisionState::COLLISION_STATE_BLOCK || state == eCollisionState::COLLISION_STATE_OVERLAP)
        _colliderReactions.emplace(std::make_pair(destKey, state));
    else
        _colliderReactions.erase(destKey);

    //충돌 반응에 따라서 맞는 함수를 호출
    if (_collisionCallBack[state])
        _collisionCallBack[state](dest);
}


//다른 충돌체 키를 넣어주면
//나와 다른 충돌체의 현재 충돌반응 상태를 가지고 온다.
eCollisionState CollisionComponent::CheckState(std::pair<int32, int32>& destKey)
{
    auto it = _colliderReactions.find(destKey);
    if (it == _colliderReactions.end())
        return eCollisionState::COLLISION_STATE_RELEASE;

    return it->second;
}

const std::pair<int32, int32>& CollisionComponent::GetColliderID() const
{
    return _colliderID;
}

bool CollisionComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(id, name, owner);

    _colliderID = std::make_pair(owner->GetActorID(), GetComponentID());

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return false;

    level->AddCollision(_colliderID, This<CollisionComponent>());

#if _DEBUG
    _shader = ShaderManager::Instance().FindShader("FrameMeshShader");
    _transformCBuffer = ShaderManager::Instance().FindCBuffer<TranformCBuffer>("Transform");
    _colorCBuffer = ShaderManager::Instance().FindCBuffer<ColorCBuffer>("Color");
#endif // _DEBUG

    return true;
}

void CollisionComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void CollisionComponent::Collision(float deltaTiem)
{
    SceneComponent::Collision(deltaTiem);
}

void CollisionComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    //todo : debug render
#if _DEBUG
    if (!_mesh || !_shader)
        return;

    _transformCBuffer->SetWorldMatrix(_matrix._world);

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    _transformCBuffer->SetViewMatrix(level->GetViewMatrix());
    _transformCBuffer->SetProjMatrix(level->GetProjMatrix());

    _transformCBuffer->Update();

    if (_colliderReactions.size() > 0)
    {
        _colorCBuffer->SetColor(1.f, 0.f, 0.f, 1.f);
    }
    else
    {
        _colorCBuffer->SetColor(0.f, 1.f, 0.f, 1.f);
    }

    _colorCBuffer->Update();

    _shader->SetShader();
    _mesh->Render();

#endif // _DEBUG
}

void CollisionComponent::Destroy()
{
    SceneComponent::Destroy();

    //todo : ??
    //내가 지워질때 RELASE함수가 바인딩 되어 있다면
    //이걸 호출해줘야 하나??? ---> 정답은 없음 하고 싶은대로

    //todo : 
    //내가 지워질때...
    //나를 리액션목록에 가지고 있는 애들을 찾아서
    //나를 전부 지워줘야 한다.

    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    //레벨에 존재하는 충돌관리자에서 나를 지운다.
    level->RemoveCollision(_colliderID);

    //나의 충돌 반응 목록을 순회하면서
    //나와 지금 부딪힌 반응을 가진 다른 충돌체의
    //반응목록에서 나를 지워줘야 한다.
    for (auto& it : _colliderReactions)
    {
        //나의 반응목록에 있는 키를 통해
        //다른 충돌체를 찾아온다.
        std::pair<int32, int32> key = it.first;
        Ptr<CollisionComponent> foundCol = level->FindCollider(key);
        if (nullptr == foundCol)
            continue;

        //찾아온 충돌체의 반응목록에서 나를 지운다.
        foundCol->_colliderReactions.erase(_colliderID);
    }
}

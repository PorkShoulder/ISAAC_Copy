#include "pch.h"
#include "AABBCollisionComponent.h"
#include "SphereCollisionComponent.h"
#include "OBBCollisionComponent.h"

#include "Collision/CollisionSystem.h"


#include "Core/AssetManager.h"
#include "Core/MeshManager.h"
#include "Core/Mesh.h"

AABBCollisionComponent::AABBCollisionComponent()
{
    _shape = eCollisionShape::AABB;
}

AABBCollisionComponent::~AABBCollisionComponent()
{}

bool AABBCollisionComponent::Init(int32 id, const std::string & name, Ptr<class Actor> owner)
{
    CollisionComponent::Init(id, name, owner);


#if _DEBUG
    _mesh = MESH_MANAGER->FindMesh("frameRect");
#endif // _DEBUG

    return true;
}

void AABBCollisionComponent::Tick(float deltaTime)
{
    CollisionComponent::Tick(deltaTime);

    //박스의 크기를 계산한다.
    _box._min._x = _world._position._x - _boxSize._x * 0.5f;
    _box._max._x = _world._position._x + _boxSize._x * 0.5f;

    _box._min._y = _world._position._y - _boxSize._y * 0.5f;
    _box._max._y = _world._position._y + _boxSize._y * 0.5f;

    _min._x = _box._min._x;
    _min._y = _box._min._y;
    _max._x = _box._max._x;
    _max._y = _box._max._y;

    SetWorldScale(_boxSize);
}

void AABBCollisionComponent::Collision(float deltaTiem)
{
    CollisionComponent::Collision(deltaTiem);
}

void AABBCollisionComponent::Render(float deltaTime)
{
    CollisionComponent::Render(deltaTime);
}

void AABBCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}

bool AABBCollisionComponent::Collision(Weak<CollisionComponent> dest)
{
    //충돌 연산을 진행한다.
    Ptr<CollisionComponent> destCol = Lock<CollisionComponent>(dest);
    if (nullptr == destCol)
        return false;

    switch (destCol->GetShape())
    {
    case eCollisionShape::AABB:
        return CollisionSystem::AABBToAABB(This<AABBCollisionComponent>(), Cast<CollisionComponent, AABBCollisionComponent>(destCol));
    case eCollisionShape::Sphere:
        return CollisionSystem::AABBToSphere(This<AABBCollisionComponent>(), Cast<CollisionComponent, SphereCollisionComponent>(destCol));
    case eCollisionShape::OBB:
        return CollisionSystem::AAABBToOBB(This<AABBCollisionComponent>(), Cast<CollisionComponent, OBBCollisionComponent>(destCol));
    default:
        break;
    }

    return false;
}

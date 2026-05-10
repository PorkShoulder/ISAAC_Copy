#include "pch.h"
#include "SphereCollisionComponent.h"
#include "AABBCollisionComponent.h"
#include "OBBCollisionComponent.h"

#include "Collision/CollisionSystem.h"


#include "Core/AssetManager.h"
#include "Core/MeshManager.h"
#include "Core/Mesh.h"

#include "Shader/TranformCBuffer.h"


SphereCollisionComponent::SphereCollisionComponent()
{
    _shape = eCollisionShape::Sphere;
}

SphereCollisionComponent::~SphereCollisionComponent()
{}

bool SphereCollisionComponent::Init(int32 id, const std::string & name, Ptr<class Actor> owner)
{
    CollisionComponent::Init(id, name, owner);

#if _DEBUG
    _mesh = MESH_MANAGER->FindMesh("FrameSphere");
#endif // _DEBUG

    return true;
}

void SphereCollisionComponent::Tick(float deltaTime)
{
    CollisionComponent::Tick(deltaTime);

    _min._x = _world._position._x - _radius;
    _max._x = _world._position._x + _radius;

    _min._y = _world._position._y - _radius;
    _max._y = _world._position._y + _radius;

    SetWorldScale(_radius * 2.f, _radius * 2.f, 1.f);
}

void SphereCollisionComponent::Collision(float deltaTiem)
{
    CollisionComponent::Collision(deltaTiem);
}

void SphereCollisionComponent::Render(float deltaTime)
{
    CollisionComponent::Render(deltaTime);
}

void SphereCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}

bool SphereCollisionComponent::Collision(Weak<CollisionComponent> dest)
{
    Ptr<CollisionComponent> destCol = Lock<CollisionComponent>(dest);
    if (nullptr == destCol)
        return false;

    switch (destCol->GetShape())
    {
    case eCollisionShape::Sphere:
        return CollisionSystem::SphereToSphere(This<SphereCollisionComponent>(), Cast<CollisionComponent, SphereCollisionComponent>(destCol));
    case eCollisionShape::AABB:
        return CollisionSystem::AABBToSphere(Cast<CollisionComponent, AABBCollisionComponent>(destCol), This<SphereCollisionComponent>());
    case eCollisionShape::OBB:
        return CollisionSystem::SphereToOBB(This<SphereCollisionComponent>(), Cast<CollisionComponent, OBBCollisionComponent>(destCol));
    default:
        break;
    }

    return false;
}

#include "pch.h"
#include "CollisionSystem.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SphereCollisionComponent.h"
#include "Component/OBBCollisionComponent.h"

bool CollisionSystem::AABBToAABB(Weak<AABBCollisionComponent> src, Weak<AABBCollisionComponent> dest)
{
    Ptr<AABBCollisionComponent> srcCol = Lock<AABBCollisionComponent>(src);
    Ptr<AABBCollisionComponent> destCol = Lock<AABBCollisionComponent>(dest);
    
    if (!srcCol || !destCol)
        return false;
    
    return AABBToAABB(srcCol->GetBox(), destCol->GetBox());
}

bool CollisionSystem::AABBToAABB(const FAABB2D& src, const FAABB2D& dest)
{
    //실제 AABB 충돌 연산
    if (src._min._x > dest._max._x)
        return false;
    else if (dest._min._x > src._max._x)
        return false;
    else if (src._min._y > dest._max._y)
        return false;
    else if (dest._min._y > src._max._y)
        return false;
    
    return true;
}

bool CollisionSystem::AABBToSphere(Weak<class AABBCollisionComponent> src, Weak<class SphereCollisionComponent> dest)
{
    Ptr<AABBCollisionComponent> srcCol = Lock<AABBCollisionComponent>(src);
    Ptr<SphereCollisionComponent> destCol = Lock<SphereCollisionComponent>(dest);

    if (!srcCol || !destCol)
        return false;

    return AABBToSphere(srcCol->GetBox(), destCol->GetWorldPosition(), destCol->GetRadius());
}

bool CollisionSystem::AABBToSphere(const FAABB2D& src, const FVector3D& destCenter, float destRadius)
{
    //중점과 가까운 점을 찾는다.
    
    float foundX = Utility::Clamp<float>(destCenter._x, src._min._x, src._max._x);
    float foundY = Utility::Clamp<float>(destCenter._y, src._min._y, src._max._y);

    //원의 중점으로 부터 거리
    float distX = destCenter._x - foundX;
    float distY = destCenter._y - foundY;

    float foundDistSquare = (distX * distX) + (distY * distY);
    float radiuseSquare = (destRadius * destRadius);

    if (foundDistSquare > radiuseSquare)
        return false;

    return true;
}

bool CollisionSystem::AAABBToOBB(Weak<class AABBCollisionComponent> src, Weak<class OBBCollisionComponent> dest)
{
    Ptr<AABBCollisionComponent> srcCol = Lock<AABBCollisionComponent>(src);
    Ptr<OBBCollisionComponent> destCol = Lock<OBBCollisionComponent>(dest);

    if (!srcCol || !destCol)
        return false;

    return AABBToOBB(srcCol->GetBox(), destCol->GetBox());
}

bool CollisionSystem::AABBToOBB(const FAABB2D& srcAABB, const FOBB2D& destOBB)
{
    //AABB의 상자를 OBB상자로 변경하여 OBB 충돌을 진행한다.
    FOBB2D obb = CreateOBB(srcAABB);

    return OBBToOBB(obb, destOBB);
}

bool CollisionSystem::SphereToSphere(Weak<class SphereCollisionComponent> src, Weak<class SphereCollisionComponent> dest)
{
    Ptr<SphereCollisionComponent> srcCol = Lock<SphereCollisionComponent>(src);
    Ptr<SphereCollisionComponent> destCol = Lock<SphereCollisionComponent>(dest);

    if (!srcCol || !destCol)
        return false;

    return SphereToSphere(srcCol->GetWorldPosition(), srcCol->GetRadius(), destCol->GetWorldPosition(), destCol->GetRadius());
}

bool CollisionSystem::SphereToSphere(const FVector3D& srcCenter, float srcRadiuse, const FVector3D& destCenter, float destRadius)
{
    //두 중점 사이의 거리를 구한다.
    float dist = srcCenter.Distance(destCenter);

    //두 중점 사이의 거리가 두 원의 반지름 합보다 크다면
    //두 원은 충돌하지 않은것이다.
    if (dist > srcRadiuse + destRadius)
        return false;

    return true;
}

bool CollisionSystem::SphereToOBB(Weak<class SphereCollisionComponent> src, Weak<class OBBCollisionComponent> dest)
{
    Ptr<SphereCollisionComponent> srcCol = Lock<SphereCollisionComponent>(src);
    Ptr<OBBCollisionComponent> destCol = Lock<OBBCollisionComponent>(dest);

    if (!srcCol || !destCol)
        return false;

    return SphereToOBB(srcCol->GetWorldPosition(), srcCol->GetRadius(), destCol->GetBox());
}

bool CollisionSystem::SphereToOBB(const FVector3D& srcCenter, float srcRadius, const FOBB2D& destOBB)
{
    //구는 따로 축이 없어서
    //우리가 축을 만들어야 합니다.
    //중심을 연결한 축을 구 특유의 축으로 사용할꺼에요.

    FVector2D convertCenter;
    convertCenter._x = srcCenter._x;
    convertCenter._y = srcCenter._y;

    FVector2D centerLine = convertCenter - destOBB._center;

    FVector2D axis = centerLine;
    axis.Normalize();

    //중심 연결 축 (구 특유의 분리 축)
    float centerProj = std::abs(centerLine.Dot(axis));
    float obbProjOnAxis = std::abs(axis.Dot(destOBB._axis[AXIS_TYPE::X])) * destOBB._halfSize._x + std::abs(axis.Dot(destOBB._axis[AXIS_TYPE::Y])) * destOBB._halfSize._y;

    if (centerProj > srcRadius + obbProjOnAxis)
        return false;

    //OBB x축
    float projX = std::abs(centerLine.Dot(destOBB._axis[AXIS_TYPE::X]));
    if (projX > srcRadius + destOBB._halfSize._x)
        return false;

    //OBB y축
    float projY = std::abs(centerLine.Dot(destOBB._axis[AXIS_TYPE::Y]));
    if (projY > srcRadius + destOBB._halfSize._y)
        return false;

    return true;
}

bool CollisionSystem::OBBToOBB(Weak<class OBBCollisionComponent> src, Weak<class OBBCollisionComponent> dest)
{
    Ptr<OBBCollisionComponent> srcCol = Lock<OBBCollisionComponent>(src);
    Ptr<OBBCollisionComponent> destCol = Lock<OBBCollisionComponent>(dest);

    if (!srcCol || !destCol)
        return false;


    return OBBToOBB(srcCol->GetBox(), destCol->GetBox());
}

bool CollisionSystem::OBBToOBB(const FOBB2D& src, const FOBB2D& dest)
{
    //OBB - OBB 충돌 --> 분리축 이론

    FVector2D centerLine = src._center - dest._center;

    if (!ComputeAxisProjection(centerLine, src._axis[AXIS_TYPE::X], src._halfSize._x, dest._axis, dest._halfSize))
        return false;

    if (!ComputeAxisProjection(centerLine, src._axis[AXIS_TYPE::Y], src._halfSize._y, dest._axis, dest._halfSize))
        return false;

    if (!ComputeAxisProjection(centerLine, dest._axis[AXIS_TYPE::X], dest._halfSize._x, src._axis, src._halfSize))
        return false;

    if (!ComputeAxisProjection(centerLine, dest._axis[AXIS_TYPE::Y], dest._halfSize._y, src._axis, src._halfSize))
        return false;

    return true;
}

bool CollisionSystem::ComputeAxisProjection(const FVector2D& centerLine, const FVector2D& srcAxis, float axisHalfSize, const FVector2D* destAxis, const FVector2D& destHalfSize)
{
    //센터를 연결하는 벡터를 분리축 후보에 투영해서
    //구간의 길이를 구한다.
    //음수 값은 필요가 없어서, 절대값으로 구한다.
    float centerProjectionDist = std::abs(centerLine.Dot(srcAxis));

    //src와 dest를 분리축에 투영한 구간을 구한다.
    //src의 x축에 투영하면 src의 투영한 구간의 길이는
    //scr의 x절반 길이와 같다.
    float destProjectionDist = std::abs(srcAxis.Dot(destAxis[AXIS_TYPE::X])) * destHalfSize._x + std::abs(srcAxis.Dot(destAxis[AXIS_TYPE::Y])) * destHalfSize._y;

    float overlap = axisHalfSize + destProjectionDist - centerProjectionDist;
    if (overlap <= 0)
        return false;

    return true;
}

FOBB2D CollisionSystem::CreateOBB(const FAABB2D& aabb)
{
    FOBB2D ret;

    ret._center = (aabb._min + aabb._max) * 0.5f;
    ret._axis[AXIS_TYPE::X]._x = 1.f;
    ret._axis[AXIS_TYPE::X]._y = 0.f;

    ret._axis[AXIS_TYPE::Y]._x = 0.f;
    ret._axis[AXIS_TYPE::Y]._y = 1.f;

    ret._halfSize = (aabb._max - aabb._min) * 0.5f;

    return ret;
}

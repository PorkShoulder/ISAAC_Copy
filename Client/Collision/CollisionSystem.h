#pragma once
#include "../Common/Info.h"
#include "../Core/Defines.h"

class CollisionSystem
{
public:
    //= AABB =
    static bool AABBToAABB(Weak<class AABBCollisionComponent> src, Weak<class AABBCollisionComponent> dest);
    static bool AABBToAABB(const FAABB2D& src, const FAABB2D& dest);

    static bool AABBToSphere(Weak<class AABBCollisionComponent> src, Weak<class SphereCollisionComponent> dest);
    static bool AABBToSphere(const FAABB2D& src, const FVector3D& destCenter, float destRadius);

    static bool AAABBToOBB(Weak<class AABBCollisionComponent> src, Weak<class OBBCollisionComponent> dest);
    static bool AABBToOBB(const FAABB2D& srcAABB, const FOBB2D& destOBB);

    //= Sphere =
    static bool SphereToSphere(Weak<class SphereCollisionComponent> src, Weak<class SphereCollisionComponent> dest);
    static bool SphereToSphere(const FVector3D& srcCenter, float srcRadiuse, const FVector3D& destCenter, float destRadius);

    static bool SphereToOBB(Weak<class SphereCollisionComponent> src, Weak<class OBBCollisionComponent> dest);
    static bool SphereToOBB(const FVector3D& srcCenter, float srcRadius, const FOBB2D& destOBB);


    //= OBB =
    static bool OBBToOBB(Weak<class OBBCollisionComponent> src, Weak<class OBBCollisionComponent> dest);
    static bool OBBToOBB(const FOBB2D& src, const FOBB2D& dest);


private:
    //static float Clamp(float value, float minValue, float maxValue);

    static bool ComputeAxisProjection(const FVector2D& centerLine, const FVector2D& srcAxis, float axisHalfSize, const FVector2D* destAxis, const FVector2D& destHalfSize);

    static FOBB2D CreateOBB(const FAABB2D& aabb);
};


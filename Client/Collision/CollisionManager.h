#pragma once
#include "./Core/Object.h"

#include <map>

class CollisionManager : public Object
{
public:
    CollisionManager() {}
    virtual ~CollisionManager() {}

private:
    //모든 충돌체들을 관리한다.
    std::map<std::pair<int32, int32>, Ptr<class CollisionComponent>> _colliders;

    //지워질 충돌체들
    std::vector<std::pair<int32, int32>> _removeColliders;

public:
    void Init();
    void Collision(float deltaTime);
    void Insert(std::pair<int32, int32>& colID, Ptr<class CollisionComponent> comp);
    void Remove(std::pair<int32, int32>& colID);

    Ptr<class CollisionComponent> FindCollider(std::pair<int32, int32>& colID);

    virtual void Destroy();
};


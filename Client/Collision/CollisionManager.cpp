#include "pch.h"
#include "CollisionManager.h"
#include "Component/CollisionComponent.h"

#include "Collision/CollisionProfileManager.h"

void CollisionManager::Init()
{}

void CollisionManager::Collision(float deltaTime)
{
    //지워져야 될 애들을 먼저 정리
    for (auto it : _removeColliders)
        _colliders.erase(it);

    _removeColliders.clear();

    //전체 충돌컴포넌트들을 순회하며
    //충돌 연산을 진행한다.
    for (auto& it : _colliders)
    {
        std::pair<int32, int32> itKey = it.first; // 충돌체 ID
        Ptr<CollisionComponent> itVal = it.second; //충돌컴포넌트(충돌체)

        if (!itVal->IsEnable())
            continue;

        if (!itVal->IsActive())
            continue;

        Ptr<CollisionProfile> srcProfile = itVal->GetProfile();
        if (nullptr == srcProfile)
            continue;

        for (auto& subIt : _colliders)
        {
            std::pair<int32, int32> subItKey = subIt.first; // 충돌체 ID
            Ptr<CollisionComponent> subItVal = subIt.second; //충돌컴포넌트(충돌체)

            if (!subItVal->IsEnable())
                continue;

            if (!subItVal->IsActive())
                continue;

            Ptr<CollisionProfile> destProfile = subItVal->GetProfile();
            if (nullptr == destProfile)
                continue;

            //src <--> dest 랑 충돌 검사

            //srcProfile에 destProfile의 channel이 무시거나 없다면
            //둘은 충돌 검사를 하지 않는다.
            if (srcProfile->GetResponse(destProfile->GetChannel()) == eCollisionResponse::COLLISION_RESPONSE_IGNORE ||
                srcProfile->GetResponse(destProfile->GetChannel()) == eCollisionResponse::COLLISION_RESPONSE_END)
                continue;

            if (destProfile->GetResponse(srcProfile->GetChannel()) == eCollisionResponse::COLLISION_RESPONSE_IGNORE ||
                destProfile->GetResponse(srcProfile->GetChannel()) == eCollisionResponse::COLLISION_RESPONSE_END)
                continue;

            //충돌 검사
            if (itVal->Collision(subItVal))
            {
                //충돌을 했음

                //이전 상태를 확인
                switch (itVal->CheckState(subItKey))
                {
                case eCollisionState::COLLISION_STATE_BLOCK:
                {
                    //이전 상태가 Block 이였기 때문에
                    //지금도 충돌하고 있는 상태라는 것이므로
                    //겹침으로 판단한다.
                    itVal->Invoke(eCollisionState::COLLISION_STATE_OVERLAP, subItVal, subItKey);
                }
                break;
                case eCollisionState::COLLISION_STATE_OVERLAP:
                {
                    //이전에 겹침상태였고, 지금도 충돌 중이기 때문에
                    //겹침으로 판단한다.
                    itVal->Invoke(eCollisionState::COLLISION_STATE_OVERLAP, subItVal, subItKey);
                }
                break;
                case eCollisionState::COLLISION_STATE_RELEASE:
                {
                    //이전에는 충돌하지 않았고,
                    //지금 충돌했다는 것이므로,
                    //block 상태로 판단한다.
                    itVal->Invoke(eCollisionState::COLLISION_STATE_BLOCK, subItVal, subItKey);
                }
                break;
                default:
                    break;
                }
            }
            else
            {
                //현재 충돌하지 않았고
                //이전에 충돌(블록,겹침) 이라면
                //지금 막 충돌이 해제된것이기 때문에
                //relase로 판단한다.
                switch (itVal->CheckState(subItKey))
                {
                case eCollisionState::COLLISION_STATE_BLOCK:
                case eCollisionState::COLLISION_STATE_OVERLAP:
                {
                    itVal->Invoke(eCollisionState::COLLISION_STATE_RELEASE, subItVal, subItKey);
                }
                break;
                default:
                    break;
                }

            }
        }
    }
}

void CollisionManager::Insert(std::pair<int32, int32>& colID, Ptr<class CollisionComponent> comp)
{
    _colliders[colID] = comp;
}

void CollisionManager::Remove(std::pair<int32, int32>& colID)
{
    _removeColliders.push_back(colID);
}

Ptr<class CollisionComponent> CollisionManager::FindCollider(std::pair<int32, int32>& colID)
{
    auto it = _colliders.find(colID);
    if (_colliders.end() == it)
        return nullptr;

    return it->second;
}

void CollisionManager::Destroy()
{
    _colliders.clear();
}

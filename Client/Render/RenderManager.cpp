#include "pch.h"
#include "RenderManager.h"
#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"

#include "Component/SceneComponent.h"

#include "Object/Actor.h"

#include <algorithm>

bool RenderManager::Init()
{

    CreateRenderLayer("BackGround", 0);
    CreateRenderLayer("Default", 10);
    CreateRenderLayer("Monster", 50);
    CreateRenderLayer("Monster1", 51);
    CreateRenderLayer("UI", 100);
    CreateRenderLayer("Collider", INT_MAX);

    _renderStateManager = New<RenderStateManager>();
    if (!_renderStateManager->Init())
        return false;

    _alphaBlend = _renderStateManager->FindRenderState<BlendState>("AlphaBlend");
    _depthStencilState = _renderStateManager->FindRenderState<DepthStencilState>("DepthStencil");

    return true;
}

void RenderManager::Tick(float deltaTime)
{
    //그려주기 전에 미리 삭제하거나
    //혹은 레이어 층이 변하는 씬 컴포넌트가 있으면
    //해당 컴포넌트의 레이어를 올바른 레이어로 바꿔줄꺼에요.

    //씬컴포넌트 삭제 처리
    for (auto& [layerName, actorID, compID] : _removeRenders)
    {
        auto foundIt = _layerFinder.find(layerName);
        if (_layerFinder.end() == foundIt)
            continue;

        auto itLayer = _layers.find(foundIt->second);
        if (_layers.end() == itLayer)
            continue;

        itLayer->second._finder.erase({ actorID, compID });
        itLayer->second._refreshRenders = true;
    }

    _removeRenders.clear();

    //레이어 변경 처리
    if (_refreshLayer)
    {
        for (auto& [order, layer] : _layers)
        {
            auto subIt = layer._finder.begin();
            while (subIt != layer._finder.end())
            {
                Ptr<SceneComponent> renderComp = Lock<SceneComponent>(subIt->second);
                if (nullptr == renderComp)
                {
                    subIt++;
                    continue;
                }

                FRenderLayer* curLayer = FindLayer(renderComp->GetRenderLayerName());
                if (nullptr == curLayer)
                {
                    subIt++;
                    continue;
                }

                if (curLayer->_order == order)
                {
                    subIt++;
                    continue;
                }

                Ptr<Actor> owner = renderComp->GetOwner();
                if (nullptr == owner)
                {
                    subIt++;
                    continue;
                }

                std::pair<int32, int32> key = { owner->GetActorID(), renderComp->GetComponentID() };

                curLayer->_finder[key] = subIt->second;
                
                //맵에서 erase의 매개변수로 이터레이터를 넣어주면
                //지우기를 성공하면 다음 이터레이터 반환
                subIt = layer._finder.erase(subIt);

                curLayer->_refreshRenders = true;
                layer._refreshRenders = true;
            }
        }
    }
}

void RenderManager::Render(float deltaTime)
{
    _alphaBlend->SetState();
    _depthStencilState->SetState();

    //모든 레이어에 있는 씬컴포넌트들을 그려줄겁니다.

    //renders를 재구성합니다.
    for (auto& [order, layer] : _layers)
    {
        if (layer._refreshRenders)
        {
            layer._renders.clear();

            for (auto& [key, weak] : layer._finder)
                layer._renders.push_back(weak);

            layer._refreshRenders = false;
        }

        //y 소팅
        if (_sortY)
        {
            std::sort(layer._renders.begin(), layer._renders.end(), [](const Weak<SceneComponent>& src, const Weak<SceneComponent>& dest)
                {
                    Ptr<SceneComponent> compSrc = Lock<SceneComponent>(src);
                    Ptr<SceneComponent> compDest = Lock<SceneComponent>(dest);

                    if (!compSrc || !compDest)
                        return false;

                    return compSrc->GetWorldPosition()._y < compDest->GetWorldPosition()._y;
                });
        }


        for (auto& it : layer._renders)
        {
            Ptr<SceneComponent> comp = Lock<SceneComponent>(it);
            if (nullptr == comp)
                continue;

            if (!comp->IsEnable())
                continue;

            if (!comp->IsActive())
                continue;

            comp->Render(deltaTime);
        }
    }

    _alphaBlend->ResetState();
    _depthStencilState->ResetState();
}

void RenderManager::RefreshLayer()
{
    _refreshLayer = true;
}

void RenderManager::SetYSort(bool sort)
{
    _sortY = sort;
}

void RenderManager::AddRenderComponent(int32 actorID, Ptr<class SceneComponent> comp)
{
    FRenderLayer* layer = FindLayer(comp->GetRenderLayerName());
    if (nullptr == layer)
        return;

    layer->_finder[{actorID, comp->GetComponentID()}] = comp;
    layer->_refreshRenders = true;
}

void RenderManager::RemoveRenderComponent(const std::string & layerName, int32 actorID, int32 compID)
{
    std::tuple<std::string, int32, int32> key = { layerName, actorID, compID };
    _removeRenders.push_back(key);
}

FRenderLayer* RenderManager::FindLayer(const std::string & name)
{
    auto it = _layerFinder.find(name);
    if (_layerFinder.end() == it)
        return nullptr;

    auto subIt = _layers.find(it->second);
    if (_layers.end() == subIt)
        return nullptr;

    return &(subIt->second);
}

void RenderManager::Destroy()
{
    DESTROY(_renderStateManager);
}

Ptr<class BlendState> RenderManager::GetAlphaBlend()
{
    return _alphaBlend;
}

Ptr<class DepthStencilState> RenderManager::GetDepthStencilState()
{
    return _depthStencilState;
}

void RenderManager::CreateRenderLayer(const std::string & name, int32 order)
{
    FRenderLayer* foundlayer = FindLayer(name);
    if (foundlayer)
        return;

    FRenderLayer layer;
    layer._name = name;
    layer._order = order;
    
    _layerFinder[name] = order;
    _layers[order] = layer;
}

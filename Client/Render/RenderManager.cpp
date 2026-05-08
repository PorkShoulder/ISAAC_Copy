#include "pch.h"
#include "RenderManager.h"

bool RenderManager::Init()
{
    return false;
}

void RenderManager::Tick(float deltaTime)
{
}

void RenderManager::Render(float deltaTime)
{
}

void RenderManager::RefreshLayer()
{
}

void RenderManager::SetYSort(bool sort)
{
}

void RenderManager::AddRenderComponent(int32 actorID, Ptr<class SceneComponent> comp)
{
}

void RenderManager::RemoveRenderComponent(const std::string& layerName, int32 actorID, int32 compID)
{
}

FRenderLayer* RenderManager::FindLayer(const std::string& name)
{
    return nullptr;
}

void RenderManager::Destroy()
{
}

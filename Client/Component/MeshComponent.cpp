#include "pch.h"
#include "MeshComponent.h"
#include "Core/AssetManager.h"

#include "Shader/ShaderManager.h"
#include "Shader/TranformCBuffer.h"

#include "Core/Mesh.h"
#include "Core/Material.h"

#include "World/Level.h"


MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
}

bool MeshComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(id, name, owner);

    return true;
}

void MeshComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void MeshComponent::Collision(float deltaTime)
{
    SceneComponent::Collision(deltaTime);
}

void MeshComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);
    // 투영 생ㄹ성
    Ptr<Level> level = Lock<Level>(_level);
    if (nullptr == level)
        return;

    FMatrix view = level->GetViewMatrix();
    FMatrix proj = level->GetProjMatrix();

    _transformCBuffer->SetWorldMatrix(_matrix._world);
    _transformCBuffer->SetProjMatrix(proj);
    _transformCBuffer->SetViewMatrix(view);
    _transformCBuffer->Update()

}

void MeshComponent::Destroy()
{
    SceneComponent::Destroy();

    for (auto& it : _materialSlots)
        DESTROY(it);
}

#include "pch.h"
#include "StaticMeshComponent.h"

#include "../Core/AssetManager.h"
#include "../Core/Mesh.h"
#include "../Core/Material.h"

#include "../Shader/ShaderManager.h"
#include "../Shader/TranformCBuffer.h"

StaticMeshComponent::StaticMeshComponent()
{
    _isRender = true;
}

StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::SetMesh(const std::string& name)
{
    Ptr<Mesh> foundMesh = MESH_MANAGER->FindMesh(name);
    if (nullptr == foundMesh)
        return;

    _mesh = foundMesh;
    _materialSlots.clear();

    size_t slotCount = foundMesh->GetSlotCount();

    for (size_t i = 0; i < slotCount; ++i)
    {
        const FMeshSlot* slot = foundMesh->GetSlot(static_cast<int>(i));
        _materialSlots.emplace_back(slot->_material->Clone());
    }
}

void StaticMeshComponent::SetShader(const std::string& name)
{
    _shader = ShaderManager::Instance().FindShader(name);
}

void StaticMeshComponent::SetMaterial(int32 slotIndex, const std::string& name)
{
    _materialSlots[slotIndex] = MATERIAL_MANAGER->CreateMaterialInstance(name);
}

void StaticMeshComponent::AddTexture(int32 slotIndex, const std::string& name, int32 registerNum, int32 shaderBufferType, int32 textureIndex)
{
    _materialSlots[slotIndex]->AddTexture(name, registerNum, shaderBufferType, textureIndex);
}

void StaticMeshComponent::SetBaseColor(int32 slotIndex, float r, float g, float b, float a)
{
    _materialSlots[slotIndex]->SetBaseColor(r, g, b, a);
}

void StaticMeshComponent::SetOpacity(int slotIndex, float opacity)
{
    _materialSlots[slotIndex]->SetOpacity(opacity);
}

bool StaticMeshComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    MeshComponent::Init(id, name, owner);

    SetShader("StaticMeshShader");

    return true;
}

void StaticMeshComponent::Tick(float deltaTime)
{
    MeshComponent::Tick(deltaTime);
}

void StaticMeshComponent::Collision(float deltaTiem)
{
    MeshComponent::Collision(deltaTiem);
}

void StaticMeshComponent::Render(float deltaTime)
{
    MeshComponent::Render(deltaTime);

    Ptr<Shader> shader = Lock<Shader>(_shader);
    if (shader)
        shader->SetShader();

    Ptr<Mesh> mesh = Lock<Mesh>(_mesh);
    if (mesh)
    {
        size_t slotCount = mesh->GetSlotCount();
        for (size_t i = 0; i < slotCount; ++i)
        {
            _materialSlots[i]->SetMaterial();

            mesh->Render();

            _materialSlots[i]->ResetMaterial();
        }
    }
}

void StaticMeshComponent::Destroy()
{
    MeshComponent::Destroy();

   
}

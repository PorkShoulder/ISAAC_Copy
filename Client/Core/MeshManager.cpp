#include "pch.h"
#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

Ptr<class Mesh> MeshManager::FindMesh(const std::string& name)
{
    // 이름으로 메쉬 찾기.
    auto it = _meshes.find(name);
    if (_meshes.end() == it)
        return nullptr;

    return it->second;

}

bool MeshManager::CreateMesh(const std::string& name, void* vertexData, int32 size, int32 count, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* indexData, int32 indexSize, int32 indexCount, DXGI_FORMAT fmt, D3D11_USAGE indexUsage)
{
    // 메쉬 생성 + 등록
    Ptr<Mesh> mesh = FindMesh(name);
    if (mesh)
        return false;

    mesh = New<Mesh>();
    if (false == mesh->CreateMesh(vertexData, size, count, vertexUsage, primitive, indexData, indexSize, indexCount, fmt, indexUsage))
    {
        Delete(mesh);
        return false;
    }
    _meshes[name] = mesh;
    return true;

}

bool MeshManager::Init()
{
    // 여기서 도형 그리기. 초기화임



    return false;
}

void MeshManager::Destroy()
{


}

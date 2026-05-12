#include "pch.h"
#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager()
{}

MeshManager::~MeshManager()
{}

Ptr<class Mesh> MeshManager::FindMesh(const std::string& name)
{
    auto it = _meshes.find(name);
    if (_meshes.end() == it)
        return nullptr;

    return it->second;
}

bool MeshManager::CreateMesh(const std::string& name, void* vertexData, int32 size, int32 count, D3D11_USAGE vertexUsage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* indexData, int32 indexSize, int32 indexCount, DXGI_FORMAT fmt, D3D11_USAGE indexUsage)
{
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
    //================
    //=   TRIANGLE   =
    //================
    //정점 정보 (정점의 위치랑 색깔)
    FVertexColor triangle[3]
    {
        FVertexColor(0.f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
        FVertexColor(0.5f, -0.5f, 0.f, 0.f, 1.f, 0.f, 1.f),
        FVertexColor(-0.5f, -0.5f, 0.f, 0.f, 0.f, 1.f, 1.f)
    };

    //인덱스 정보
    //DXGI_FORMAT_R16_UINT
    //인덱스 하나의 크기를 2바이트로
    uint16 triangleIndex[3] = { 0, 1, 2 };

    if (!CreateMesh("Triangle", triangle, sizeof(FVertexColor), 3, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, triangleIndex, sizeof(uint16), 3, DXGI_FORMAT_R16_UINT))
        return false;

    FVertexColor triangleRed[3]
    {
        FVertexColor(0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
        FVertexColor(0.f, -0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
        FVertexColor(-0.5f, 0.5f, 0.f, 1.f, 0.f, 0.f, 1.f),
    };

    if (!CreateMesh("TriangleRed", triangleRed, sizeof(FVertexColor), 3, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, triangleIndex, sizeof(uint16), 3, DXGI_FORMAT_R16_UINT))
        return false;

    //================
    //=   frameRect  =
    //================
    FVector3D frameRect[4]
    {
        FVector3D(-0.5, 0.5f, 0.f),
        FVector3D(0.5, 0.5f, 0.f),
        FVector3D(-0.5, -0.5f, 0.f),
        FVector3D(0.5, -0.5f, 0.f),
    };

    uint16 frameRectIndex[5] = { 0, 1, 3, 2, 0 };
    if (!CreateMesh("frameRect", frameRect, sizeof(FVector3D), 4, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, frameRectIndex, sizeof(uint16), 5, DXGI_FORMAT_R16_UINT))
        return false;

    //==========
    //= Sphere =
    //==========
    FVector3D spherePoint[37];
    for (int i = 0; i < 37; ++i)
    {
        float angle = DirectX::XMConvertToRadians(i * 10.f);

        spherePoint[i]._x = cosf(angle) * 0.5f;
        spherePoint[i]._y = sinf(angle) * 0.5f;
    }

    uint16 spherIndex[38] = {};
    for (int i = 0; i < 37; ++i)
    {
        spherIndex[i] = i;
    }
    spherIndex[37] = 0;

    if (!CreateMesh("FrameSphere", spherePoint, sizeof(FVector3D), 37, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, spherIndex, sizeof(uint16), 38, DXGI_FORMAT_R16_UINT))
        return false;

    //============
    //= texRect  =
    //============
    FVertexTexture texRect[4]
    {
        FVertexTexture(-0.5f, 0.5f, 0.f, 0.f, 0.f),
        FVertexTexture(0.5f, 0.5f, 0.f, 1.f, 0.f),
        FVertexTexture(-0.5f, -0.5f, 0.f, 0.f, 1.f),
        FVertexTexture(0.5f, -0.5f, 0.f, 1.f, 1.f)
    };

    uint16 rectIndex[6] = { 0, 1, 3, 0, 3, 2 }; 

    if (!CreateMesh("TexRect", texRect, sizeof(FVertexTexture), 4, D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, rectIndex, sizeof(uint16), 6, DXGI_FORMAT_R16_UINT))
        return false;

    return true;
}

void MeshManager::Destroy()
{
    for (auto& it : _meshes)
    {
        if (nullptr == it.second)
            continue;

        DESTROY(it.second)
    }

    _meshes.clear();
}
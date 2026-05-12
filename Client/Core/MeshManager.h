#pragma once
#include "SubManager.h"
#include <unordered_map>

class MeshManager : public SubManager
{
public:
    MeshManager();
    virtual ~MeshManager();
    MeshManager(const MeshManager&) = delete;
    MeshManager(MeshManager&&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;
    MeshManager& operator=(MeshManager&&) = delete;

private:
    std::unordered_map<std::string, Ptr<class Mesh>> _meshes;

public:
    Ptr<class Mesh> FindMesh(const std::string& name);

    bool CreateMesh(const std::string& name, void* vertexData, int32 size, int32 count, D3D11_USAGE vertexUsage,
        D3D11_PRIMITIVE_TOPOLOGY primitive, 
        void* indexData = nullptr, int32 indexSize = 0, int32 indexCount = 0,
        DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE indexUsage = D3D11_USAGE_DEFAULT);

    virtual bool Init() override;
    virtual void Destroy() override;
};


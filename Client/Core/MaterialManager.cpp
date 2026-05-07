#include "pch.h"
#include "MaterialManager.h"
#include "Material.h"

#include "Shader/ShaderManager.h"
#include "Shader/MaterialCBuffer.h"

bool MaterialManager::Init()
{
    if (!CreateMaterial("DefaultMaterial", "MaterialPixelShader", eTextureSampleType::TEXTURE_SAMPLE_LINEAR))
        return false;



    return true;
}

bool MaterialManager::CreateMaterial(const std::string& name, const std::string& pixelShader, eTextureSampleType sampleType)
{
    if (FindMaterial(name))
        return false;

    Ptr<Material> mat = New<Material>();
    mat->SetName(name);
    mat->SetSamplerType(sampleType);
    mat->SetPixelShader(pixelShader);

    //todo : buffer
    mat->_buffer = ShaderManager::Instance().FindCBuffer<MaterialCBuffer>("Material");
    _materials[name] = mat;

    return true;
}

//재질의 경우 -- 메쉬마다 같은 재질이지만 속성이 다를 수 있어서
//각각의 복사본을 가지고 있게끔
Ptr<class Material> MaterialManager::CreateMaterialInstance(const std::string& name)
{
    Ptr<Material> origin = FindMaterial(name);
    if (nullptr == origin)
        return nullptr;

    //복사본을 생성해서 리턴
    return origin->Clone();
}

Ptr<class Material> MaterialManager::FindMaterial(const std::string& name)
{
    auto it = _materials.find(name);
    if (_materials.end() == it)
        return nullptr;

    return it->second;
}

void MaterialManager::Destroy()
{
    for (auto& it : _materials)
        DESTROY(it.second);
}

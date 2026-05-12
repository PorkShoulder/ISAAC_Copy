#include "pch.h"
#include "ShaderManager.h"
#include "ColorMeshShader.h"
#include "Common/LogManager.h"

#include "ColorCBuffer.h"
#include "MaterialCBuffer.h"
#include "SpriteCBuffer.h"
#include "AnimCBuffer.h"
#include "TileMapCBuffer.h"
#include "TranformCBuffer.h"
#include "UICBuffer.h"
#include "ProgressBarCBuffer.h"

#include "FrameMeshShader.h"
#include "MaterialPixelShader.h"
#include "StaticMeshShader.h"
#include "SpriteShader.h"
#include "TileShader.h"
#include "TileInstanceShader.h"
#include "TileLineInstanceShader.h"
#include "UIShader.h"
#include "ProgressBarShader.h"

#include "TileSBuffer.h"
#include "TileLineSBuffer.h"

#include "Core/Device.h"

void ShaderManager::CreateSampler()
{
    D3D11_SAMPLER_DESC desc = {};
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    //포인트 샘플러
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    Device::Instance().GetDevice()->CreateSamplerState(&desc, _samplers[TEXTURE_SAMPLE_POINT].GetAddressOf());

    //
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    Device::Instance().GetDevice()->CreateSamplerState(&desc, _samplers[TEXTURE_SAMPLE_LINEAR].GetAddressOf());

    desc.Filter = D3D11_FILTER_ANISOTROPIC;
    desc.MaxAnisotropy = 16; // 1~16, 높을수록 품질좋고 무겁다. (근데 2d 게임에서는 거의 쓸일이 없다)
    Device::Instance().GetDevice()->CreateSamplerState(&desc, _samplers[TEXTURE_SAMPLE_ANISOTROPIC].GetAddressOf());
}

bool ShaderManager::Init()
{
    if (!CreateShader<ColorMeshShader>("ColorMeshShader"))
        return false;

    if (!CreateShader<FrameMeshShader>("FrameMeshShader"))
        return false;
    
    if (!CreateShader<StaticMeshShader>("StaticMeshShader"))
        return false;

    if (!CreateShader<MaterialPixelShader>("MaterialPixelShader"))
        return false;
    
    if (!CreateShader<SpriteShader>("SpriteShader"))
        return false;
    
    if (!CreateShader<TileShader>("TileShader"))
        return false;

    if (!CreateShader<TileInstanceShader>("TileInstanceShader"))
        return false;

    if (!CreateShader<TileLineInstanceShader>("TileLineInstanceShader"))
        return false;

    if (!CreateShader<UIShader>("UIShader"))
        return false;

    if (!CreateShader<ProgressBarShader>("ProgressBar"))
        return false;

    if (!CreateCBuffer<TranformCBuffer>("Transform", sizeof(FTransformCBufferData), 0, SHADER_TYPE::VERTEX))
        return false;
    if (!CreateCBuffer<AnimCBuffer>("Anim", sizeof(FAnimation2DCBufferData), 1, SHADER_TYPE::VERTEX))
        return false;
    if (!CreateCBuffer<TileMapCBuffer>("TileMap", sizeof(FTileMapCBufferData), 2, SHADER_TYPE::VERTEX))
        return false;
    
    if (!CreateCBuffer<ColorCBuffer>("Color", sizeof(FColorCBufferData), 0, SHADER_TYPE::PIXEL))
        return false;
    if (!CreateCBuffer<MaterialCBuffer>("Material", sizeof(FMaterialCBufferData), 1, SHADER_TYPE::PIXEL))return false;
    if (!CreateCBuffer<SpriteCBuffer>("Sprite", sizeof(FSpriteCBufferData), 2, SHADER_TYPE::PIXEL))
        return false;
    if(!CreateCBuffer<UICBuffer>("UI", sizeof(FUIBrushData), 3, SHADER_TYPE::GRAPHIC))
        return false;
    if (!CreateCBuffer<ProgressBarCBuffer>("ProgressBar", sizeof(FProgressBarData), 4, SHADER_TYPE::PIXEL))
        return false;

    if (!CreateSBuffer<TileSBuffer>("TileInstance", sizeof(FTileInstanceData), 100, 1, SHADER_TYPE::VERTEX))
        return false;
    if (!CreateSBuffer<TileLineSBuffer>("TileLineInstance", sizeof(FTileLineInstanceData), 100, 2, SHADER_TYPE::VERTEX))
        return false;

    CreateSampler();

    return true;
}

void ShaderManager::Destroy()
{
    for (auto& it : _shaders)
    {
        DESTROY(it.second);
    }

    for (auto& it : _cBuffers)
    {
        DESTROY(it.second);
    }

    _shaders.clear();
    _cBuffers.clear();
}

Ptr<Shader> ShaderManager::FindShader(const std::string & name)
{
    auto it = _shaders.find(name);
    if (_shaders.end() == it)
        return nullptr;

    return it->second;
}

void ShaderManager::SetSample(eTextureSampleType type)
{
    Device::Instance().GetContext()->PSSetSamplers(0, 1, _samplers[type].GetAddressOf());
}

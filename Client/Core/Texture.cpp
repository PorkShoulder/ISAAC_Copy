#include "pch.h"
#include "Texture.h"
#include "DirectoryManager.h"
#include "Device.h"


#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#elif _EDITOR
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif // _DEBUG

// 텍스처 정보 가져오기
const FTextureInfo* Texture::GetTexture(int32 idx) const
{
    // idx번째 텍스처 정보를 변환한느 단순한 함수

    if (_textureInfos.size() < idx || idx < 0)
        return nullptr;
    return &_textureInfos[idx];
}

//텍스처 불러오기
bool Texture::LoadTexture(const std::wstring& fileName)
{
    // 경로 조립 
    std::filesystem::path filePath;
    std::optional<std::filesystem::path> resPath = DirectoryManager::Instance().GetCachePath("Resources");

    // 리소스 폴더 아래 텍스처 폴더유무확인
    // 있다면 그 경로를 filePath에 넣어준다.
    if (!DirectoryManager::Instance().GetDirectory(resPath.value(), "Texture", filePath))
        return false;

    // filePath + fileName 두개를 더한 경로에 
    // 파일이 존재한다면 filePath에 그 경로를 넣어줌
    if (!DirectoryManager::Instance().GetFile(filePath, fileName, filePath))
        return false;
    
    // 파일의 확장자에 따라서 로드하는 함수가 달라지므로, 확장자를 확인후 로드함.
    FTextureInfo texture;
    if (DirectoryManager::Instance().IsExtension(filePath, ".DDS"))
    {
        if (FAILED(DirectX::LoadFromDDSFile(filePath.wstring().c_str(), DirectX::DDS_FLAGS_NONE, nullptr, texture._image)))
            return false;
    }
    else if (DirectoryManager::Instance().IsExtension(filePath, ".TGA"))
    {
        if (FAILED(DirectX::LoadFromTGAFile(filePath.wstring().c_str(), nullptr, texture._image)))
            return false;
    }
    else
    {
        if (FAILED(DirectX::LoadFromWICFile(filePath.wstring().c_str(), DirectX::WIC_FLAGS_NONE, nullptr, texture._image)))
            return false;
    }
 
    size_t srcIndex = _textureInfos.size();

    //텍스처의 가로, 세로 크기를 저장한다.
    texture._fileName = fileName;
    texture._width = static_cast<uint32>(texture._image.GetImages()->width);
    texture._height = static_cast<uint32>(texture._image.GetImages()->height);

    _textureInfos.emplace_back(std::move(texture));
    return CreateResourceView(static_cast<int32>(srcIndex));        // cpu에 있는 이미지 데이터를 gpu용 SRV로 만든다.
}

void Texture::SetShader(int32 registerNum, int32 shaderBufferType, int32 textureIndex)
{
    // 어떤 쉐이더를 바인딩할지 판단해서 세팅함.
    if (shaderBufferType & SHADER_TYPE::VERTEX)
        CONTEXT->VSSetShaderResources(registerNum, 1, _textureInfos[textureIndex]._srv.GetAddressOf());
    if (shaderBufferType & SHADER_TYPE::PIXEL)
        CONTEXT->PSSetShaderResources(registerNum, 1, _textureInfos[textureIndex]._srv.GetAddressOf());


}

void Texture::ResetShader(int32 registerNum, int32 shaderBufferType)
{
    // 렌더링 끝나고 슬롯을 비워주는 함수 .
    ID3D11ShaderResourceView* srv = nullptr;

    if (shaderBufferType & SHADER_TYPE::VERTEX)
        CONTEXT->VSSetShaderResources(registerNum, 1, &srv);
    if (shaderBufferType & SHADER_TYPE::PIXEL)
        CONTEXT->VSSetShaderResources(registerNum, 1, &srv);
}

void Texture::Destroy()
{
}

bool Texture::CreateResourceView(int32 idx)
{
    if (FAILED(DirectX::CreateShaderResourceView(Device::Instance().GetDevice().Get(), _textureInfos[idx]._image.GetImages(), _textureInfos[idx]._image.GetImageCount(), _textureInfos[idx]._image.GetMetadata(), _textureInfos[idx]._srv.GetAddressOf())))
        return false;
    return true;
}

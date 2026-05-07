#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

bool TextureManager::Init()
{
    // 사용될 이미지 초기화.

    //if (!LoadTexture("", TEXT(".png")))
    //  return false;
    if (!LoadTexture("apeach", TEXT("apeach.png")))
        return false;

    return true;
}

bool TextureManager::LoadTexture(const std::string& name, const std::wstring& fileName)
{
    // 

    Ptr<Texture> texture = nullptr;
    if (texture = Findtexture(name))
        return texture->LoadTexture(fileName);

    texture = New<Texture>();
    if (!texture->LoadTexture(fileName))
        return false;

    texture->SetName(name);
    _textures[name] = texture;

    return true;
}

Ptr<class Texture> TextureManager::Findtexture(const std::string& name)
{
    auto it = _textures.find(name);
    if (_textures.end() == it)
        return nullptr;

    return it->second;
}

void TextureManager::Destroy()
{
    for (auto& it : _textures)
        DESTROY(it.second);
}

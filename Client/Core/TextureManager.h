#pragma once
#include"SubManager.h"
#include <unordered_map>


class TextureManager: public SubManager
{
public:
    TextureManager() = default;
    virtual ~TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager operator=(const TextureManager&) = delete;
    TextureManager operator=(TextureManager&&) = delete;
private:
    std::unordered_map<std::string, Ptr<class Texture>> _textures;

public:
    bool Init();
    bool LoadTexture(const std::string& name, const std::wstring& fileName);
    Ptr<class Texture> Findtexture(const std::string& name);
    
    virtual void Destroy() override;

};


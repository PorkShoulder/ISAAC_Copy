#pragma once
#include "SubManager.h"

#include <unordered_map>

class AnimationManager : public SubManager
{
public:
    AnimationManager();
    virtual ~AnimationManager();

private:
    std::unordered_map<std::string, Ptr<class Animation2DData>> _anims;

public:
    bool Init();

    Ptr<class Animation2DData> FindAnimation(const std::string& name);

    bool CreateAnimation(const std::string& name, eAnimTextureType type);
    bool SetAnimationTextureType(const std::string& name, eAnimTextureType type);
    void SetTexture(const std::string& name, Ptr<class Texture> texture);
    void SetTexture(const std::string& name, const std::string& textureName);
    void SetTexture(const std::string& name, const std::string& textureName, const std::wstring& fileName);


    void AddFrame(const std::string& name, const FVector2D& start, const FVector2D& size);
    void AddFrame(const std::string& name, float startX, float startY, float sizeX, float sizeY);

    virtual void Destroy() override;

};



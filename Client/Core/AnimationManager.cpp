#include "pch.h"
#include "AnimationManager.h"
#include "Animation2DData.h"
#include "../Core/IsaacAnimationLoader.h"

AnimationManager::AnimationManager()
{}

AnimationManager::~AnimationManager()
{}

bool AnimationManager::Init()
{
    IsaacAnimationLoader::Register(this);   // 이미지 로딩 
    


    return true;
}

Ptr<class Animation2DData> AnimationManager::FindAnimation(const std::string& name)
{
    auto it = _anims.find(name);
    if (_anims.end() == it)
        return nullptr;

    return it->second;
}

bool AnimationManager::CreateAnimation(const std::string& name, eAnimTextureType type)
{
    if (FindAnimation(name))
        return false;

    Ptr<Animation2DData> anim = New<Animation2DData>();
    anim->SetName(name);
    anim->SetAnimationTextureType(type);
    _anims[name] = anim;

    return true;
}

bool AnimationManager::SetAnimationTextureType(const std::string& name, eAnimTextureType type)
{
    Ptr<Animation2DData> anim = FindAnimation(name);
    if (!anim)
        return false;

    anim->SetAnimationTextureType(type);

    return true;
}

void AnimationManager::SetTexture(const std::string& name, Ptr<class Texture> texture)
{
    Ptr<Animation2DData> anim = FindAnimation(name);
    if (!anim)
        return;

    anim->SetTexture(texture);
}

void AnimationManager::SetTexture(const std::string& name, const std::string& textureName)
{
    Ptr<Animation2DData> anim = FindAnimation(name);
    if (!anim)
        return;

    anim->SetTexture(textureName);
}

void AnimationManager::SetTexture(const std::string& name, const std::string& textureName, const std::wstring& fileName)
{
    Ptr<Animation2DData> anim = FindAnimation(name);
    if (!anim)
        return;

    anim->SetTexture(textureName, fileName);
}

void AnimationManager::AddFrame(const std::string& name, const FVector2D& start, const FVector2D& size)
{
    Ptr<Animation2DData> anim = FindAnimation(name);
    if (!anim)
        return;

    anim->AddFrame(start, size);
}

void AnimationManager::AddFrame(const std::string& name, float startX, float startY, float sizeX, float sizeY)
{
    Ptr<Animation2DData> anim = FindAnimation(name);
    if (!anim)
        return;

    anim->AddFrame(startX, startY, sizeX, sizeY);
}

void AnimationManager::Destroy()
{
    for (auto& it : _anims)
        DESTROY(it.second);
}

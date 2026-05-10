#include "pch.h"
#include "AnimationManager.h"
#include "Animation2DData.h"

AnimationManager::AnimationManager()
{}

AnimationManager::~AnimationManager()
{}

bool AnimationManager::Init()
{
    CreateAnimation("LION_IDLE", eAnimTextureType::SPRITE);
    SetTexture("LION_IDLE", "LION_IDLE", TEXT("FrameAnim\\lion_atlas.png"));

    for (int32 i = 0; i < 12; ++i)
        AddFrame("LION_IDLE", i * 420.f, 0, 420.f, 420.f);

    CreateAnimation("LION_DANCE", eAnimTextureType::FRAME);
    SetTexture("LION_DANCE", "LION_DANCE", TEXT("FrameAnim\\frame_000.png"));
    SetTexture("LION_DANCE", "LION_DANCE", TEXT("FrameAnim\\frame_001.png"));
    SetTexture("LION_DANCE", "LION_DANCE", TEXT("FrameAnim\\frame_002.png"));
    SetTexture("LION_DANCE", "LION_DANCE", TEXT("FrameAnim\\frame_003.png"));
    SetTexture("LION_DANCE", "LION_DANCE", TEXT("FrameAnim\\frame_004.png"));
    for (int32 i = 0; i < 5; ++i)
        AddFrame("LION_DANCE", 0.f, 0.f, 1.f, 1.f);

    CreateAnimation("APEACH_IDLE", eAnimTextureType::SPRITE);
    SetTexture("APEACH_IDLE", "APEACH_IDLE", TEXT("FrameAnim\\apeach_atlas.png"));

    for (int32 i = 0; i < 18; ++i)
        AddFrame("APEACH_IDLE", i * 480.f, 0, 480.f, 480.f);



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

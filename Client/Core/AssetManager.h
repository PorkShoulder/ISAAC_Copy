#pragma once
#include "Common/Singleton.h"
#include "Defines.h"
#include "Common/Info.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "AnimationManager.h"
#include "SoundManager.h"
#include "FontManager.h"

#include <map>

//게임에서 사용하는 모든 에셋들의 매니저를 관리하는 클래스
class AssetManager : public Singleton<AssetManager>
{
    SINGLETON(AssetManager)

private:
    std::map<eAssetType, Ptr<class SubManager>> _managers;

public:
    bool Init();

    template<typename T>
    Ptr<T> Get(eAssetType type)
    {
        if (_managers.end() == _managers.find(type))
            return nullptr;

        return Cast<SubManager, T>(_managers[type]);
    }

    template<typename T>
    bool RegisterManager(eAssetType type)
    {
        Ptr<T> manager = New<T>();
        if (false == manager->Init())
        {
            DESTROY(manager);
            //Delete(manager);
            return false;
        }

        _managers[type] = manager;

        return true;
    }

    virtual void Destroy() override;

};

#define MESH_MANAGER AssetManager::Instance().Get<MeshManager>(eAssetType::MESH)
#define TEXTURE_MANAGER AssetManager::Instance().Get<TextureManager>(eAssetType::TEXTURE)
#define MATERIAL_MANAGER AssetManager::Instance().Get<MaterialManager>(eAssetType::MATERIAL)
#define ANIMATION_MANAGER AssetManager::Instance().Get<AnimationManager>(eAssetType::ANIMATION)
#define SOUND_MANAGER AssetManager::Instance().Get<SoundManager>(eAssetType::SOUND)
#define FONT_MANAGER AssetManager::Instance().Get<FontManager>(eAssetType::FONT)


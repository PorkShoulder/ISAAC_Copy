#include "pch.h"
#include "AssetManager.h"

#include "Defines.h"


bool AssetManager::Init()
{
    if (!RegisterManager<TextureManager>(eAssetType::TEXTURE))
        return false;

    if (!RegisterManager<MaterialManager>(eAssetType::MATERIAL))
        return false;

    if (!RegisterManager<MeshManager>(eAssetType::MESH))
        return false;

  /*  if (!RegisterManager<AnimationManager>(eAssetType::ANIMATION))
        return false;

    if (!RegisterManager<SoundManager>(eAssetType::SOUND))
        return false;

    if (!RegisterManager<FontManager>(eAssetType::FONT))*/
        return false;

    return true;
}

void AssetManager::Destroy()
{
    for (auto& it : _managers)
    {
        DESTROY(it.second)
            //it.second->Destroy();
            //Delete(it.second);
    }

    _managers.clear();
}

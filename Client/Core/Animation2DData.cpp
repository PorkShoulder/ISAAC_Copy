#include "pch.h"
#include "Animation2DData.h"

#include "Texture.h"
#include "AssetManager.h"

Animation2DData::Animation2DData()
{}

Animation2DData::~Animation2DData()
{}

eAnimTextureType Animation2DData::GetAnimTextureType() const
{
    return _type;
}

Ptr<Texture> Animation2DData::GetTexture() const
{
    return _texture;
}

const FAnimationFrame& Animation2DData::GetFrame(int32 index) const
{
    if (index < 0)
        return _frames[0];
    else if (index >= _frames.size())
        return _frames[_frames.size() - 1];

    return _frames[index];
}

int32 Animation2DData::GetFrameCount() const
{
    return static_cast<int32>(_frames.size());
}

void Animation2DData::SetAnimationTextureType(eAnimTextureType type)
{
    _type = type;
}

void Animation2DData::SetTexture(const std::string& name)
{
    _texture = TEXTURE_MANAGER->Findtexture(name);
}

void Animation2DData::SetTexture(Ptr<class Texture> texture)
{
    _texture = texture;
}

void Animation2DData::SetTexture(const std::string& name, const std::wstring& fileName)
{
    if (!TEXTURE_MANAGER->LoadTexture(name, fileName))
        return;

    _texture = TEXTURE_MANAGER->Findtexture(name);
}

void Animation2DData::AddFrame(const FVector2D& start, const FVector2D& size)
{
    AddFrame(start._x, start._y, size._x, size._y);
}

void Animation2DData::AddFrame(float startX, float startY, float sizeX, float sizeY)
{
    FAnimationFrame frame;
    frame._start._x = startX;
    frame._start._y = startY;

    frame._size._x = sizeX;
    frame._size._y = sizeY;

    _frames.emplace_back(frame);
}

void Animation2DData::Destroy()
{

}

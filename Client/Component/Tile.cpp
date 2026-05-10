#include "pch.h"
#include "Tile.h"

void Tile::Destroy()
{
}

const eTileType Tile::GetTileType() const
{
    return _type;
}

void Tile::SetTileType(eTileType type)
{
    _type = type;
}

const FVector2D& Tile::GetPos() const
{
    return _pos;
}

void Tile::SetPos(const FVector2D& pos)
{
    _pos = pos;
}

const FVector2D& Tile::GetSize() const
{
    return _size;
}

void Tile::SetSize(const FVector2D& size)
{
    _size = size;
}

const FVector2D& Tile::GetCenter() const
{
    return _center;
}

void Tile::SetCenter(const FVector2D& center)
{
    _center = center;
}

const int32 Tile::GetTextureFrame() const
{
    return _textureFrame;
}

void Tile::SetTextureFrame(int32 frame)
{
    _textureFrame = frame;
}

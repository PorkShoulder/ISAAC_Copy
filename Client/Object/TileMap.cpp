#include "pch.h"
#include "TileMap.h"

#include "Component/TileComponent.h"

#include "Input/InputSystem.h"

TileMap::TileMap()
{}

TileMap::~TileMap()
{}

bool TileMap::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    _tileComponent = CreateSceneComponent<TileComponent>("Tile");
    SetRootComponent(_tileComponent);
    _tileComponent->SetTexture("TileSet", TEXT("ISAAC_Map\\room\\0_library.png"));


    return true;
}

void TileMap::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);

    if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton))
    {
        ChangeTileTexture(InputSystem::Instance().GetMouseWorldPos());
    }

    if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::RButton))
    {
        ChangeTileType(InputSystem::Instance().GetMouseWorldPos());
    }

}

void TileMap::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void TileMap::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void TileMap::Destroy()
{
    Actor::Destroy();
}

//타일 텍스쳐 변경
void TileMap::ChangeTileTexture(const FVector2D& pos)
{
    Ptr<Tile> tile = _tileComponent->GetTile(pos);
    if (nullptr == tile)
        return;

    int32 curFrame = tile->GetTextureFrame();
    int32 maxFrame = _tileComponent->GetTileFrameCount();
    curFrame++;
    if (curFrame >= maxFrame)
        curFrame = 0;

    tile->SetTextureFrame(curFrame);

    _tileComponent->SetTileInstRefresh(true);
}

void TileMap::ChangeTileType(const FVector2D& pos)
{
    Ptr<Tile> tile = _tileComponent->GetTile(pos);
    if (nullptr == tile)
        return;

    eTileType type = tile->GetTileType();
    if (type == eTileType::NORMAL)
        tile->SetTileType(eTileType::BLOCK);
    else
        tile->SetTileType(eTileType::NORMAL);

    _tileComponent->SetTileLineInstRefresh(true);
  
}
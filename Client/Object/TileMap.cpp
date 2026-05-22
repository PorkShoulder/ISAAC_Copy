#include "pch.h"
#include "TileMap.h"

#include "Component/TileComponent.h"

#include "Input/InputSystem.h"
#include "../Editor/imgui.h"


TileMap::TileMap()
{}

TileMap::~TileMap()
{}

bool TileMap::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    _tileComponent = CreateSceneComponent<TileComponent>("Tile");
    SetRootComponent(_tileComponent);
    
    return true;
}

void TileMap::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);

    if (!ImGui::GetIO().WantCaptureMouse)
    {
        FVector2D mousePos = InputSystem::Instance().GetMouseWorldPos();

        // 좌클릭 시작 → 드래그 시작
        if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton))
        {
            int32 idx = _tileComponent->GetTileIndex(mousePos);
            if (idx >= 0)
            {
                _isTileDragging = true;
                _hasTileSelection = false;
                _tileSelectStartIdx = idx;
                _tileSelectEndIdx = idx;
            }
        }

        // 좌클릭 유지 → 끝점 갱신
        if (_isTileDragging && InputSystem::Instance().GetMouseHold(MOUSE_BUTTON_TYPE::LButton))
        {
            int32 idx = _tileComponent->GetTileIndex(mousePos);
            if (idx >= 0)
            {
                _tileSelectEndIdx = idx;

                if (_viewFrameIndex >= 0)
                {
                    Ptr<Tile> tile = _tileComponent->GetTile(idx);
                    if (tile)
                    {
                        tile->SetTextureFrame(_viewFrameIndex);
                        tile->SetFlipX(_viewFlipX);
                        tile->SetFlipY(_viewFlipY);
                        _tileComponent->SetTileInstRefresh(true);
                    }

                }
            }
        }

        // 좌클릭 릴리즈 → 선택 확정
        if (_isTileDragging && InputSystem::Instance().GetMouseUp(MOUSE_BUTTON_TYPE::LButton))
        {
            _isTileDragging = false;

            // 시작과 끝이 같으면 단일 클릭 → 바로 칠하기
            if (_tileSelectStartIdx == _tileSelectEndIdx)
            {
                Ptr<Tile> tile = _tileComponent->GetTile(_tileSelectStartIdx);
                if (tile && _viewFrameIndex >= 0)
                {
                    tile->SetTextureFrame(_viewFrameIndex);
                    tile->SetFlipX(_viewFlipX);
                    tile->SetFlipY(_viewFlipY);
                    _tileComponent->SetTileInstRefresh(true);
                }
                _hasTileSelection = false;
            }
            else
            {
                // 칠하기 모드였으면 이미 Hold에서 전부 칠했으므로 선택 해제
                // 칠하기 모드가 아니면 범위 선택 유지 
                if (_viewFrameIndex >= 0)
                    _hasTileSelection = false;
                else
                    _hasTileSelection = true;

            }
        }

        // 우 클릭
        if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::RButton))
        {
            ChangeTileType(InputSystem::Instance().GetMouseWorldPos());
        }

        //// X키: 좌우 반전, Y키: 상하 반전
        //if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton))
        //{
        //    if (GetAsyncKeyState('R') & 0x8000)
        //    {
        //        FlipTileX(mousePos);
        //    }
        //    if (GetAsyncKeyState('T') & 0x8000)
        //    {
        //        FlipTileY(mousePos);
        //    }
        //}

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
void TileMap::ChangeTileTexture(const FVector2D& pos, int32 frameIndex)
{
    Ptr<Tile> tile = _tileComponent->GetTile(pos);
    if (nullptr == tile)
        return;

    int32 maxFrame = _tileComponent->GetTileFrameCount();
    if (maxFrame <= 0)
        return;
    if(frameIndex >= 0 && frameIndex <maxFrame)
    {
        tile->SetTextureFrame(frameIndex);
    }
    else 
    {
        int32 curFrame = tile->GetTextureFrame();
        int32 maxFrame = _tileComponent->GetTileFrameCount();
        curFrame++;
        if (curFrame >= maxFrame)
            curFrame = 0;
        tile->SetTextureFrame(curFrame);
    }
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

void TileMap::Save(std::ofstream& file)
{
    //Actor 이름 저장
    std::string name = GetName();
    int32 nameLen = (int32)name.size();
    file.write((char*)&nameLen, sizeof(int32));
    file.write(name.c_str(), nameLen);


    // Actor 위치
    FVector3D pos = GetWorldPosition();
    file.write((char*)&pos, sizeof(FVector3D));

    // TileComponent 저장
    _tileComponent->Save(file);
}

void TileMap::Load(std::ifstream& file)
{
    // Actor 이름 읽기
    int32 nameLen = 0;
    file.read((char*)&nameLen, sizeof(int32));
    std::string name(nameLen, '\0');
    file.read(&name[0], nameLen);
    SetName(name);

    // Actor 위치
    FVector3D pos;
    file.read((char*)&pos, sizeof(FVector3D));
    SetWorldPosition(pos);

    // TileComponent 불러오기
    _tileComponent->Load(file);
}

void TileMap::ApplyFrameToSelection(int32 frameIndex, bool flipX, bool flipY)
{
    if (!_hasTileSelection) return;

    int32 maxFrame = _tileComponent->GetTileFrameCount();
    if (frameIndex < 0 || frameIndex >= maxFrame) return;

    // 시작/끝 인덱스 → X, Y 좌표로 변환
    int32 countX = _tileComponent->GetTileCountX();

    int32 startX = _tileSelectStartIdx % countX;
    int32 startY = _tileSelectStartIdx / countX;
    int32 endX = _tileSelectEndIdx % countX;
    int32 endY = _tileSelectEndIdx / countX;

    // min/max 정리
    int32 minX = min(startX, endX);
    int32 maxX = max(startX, endX);
    int32 minY = min(startY, endY);
    int32 maxY = max(startY, endY);

    for (int32 y = minY; y <= maxY; ++y)
    {
        for (int32 x = minX; x <= maxX; ++x)
        {
            int32 idx = y * countX + x;
            Ptr<Tile> tile = _tileComponent->GetTile(idx);
            if (tile)
            {
                tile->SetTextureFrame(frameIndex);
                tile->SetFlipX(flipX);
                tile->SetFlipY(flipY);
            }
            
        }
    }

    _tileComponent->SetTileInstRefresh(true);
    _hasTileSelection = false;
    _tileSelectStartIdx = -1;
    _tileSelectEndIdx = -1;
}

void TileMap::FlipTileX(const FVector2D& pos)
{
    Ptr<Tile> tile = _tileComponent->GetTile(pos);
    if (!tile) return;
    tile->ToggleFlipX();
    _tileComponent->SetTileInstRefresh(true);
}

void TileMap::FlipTileY(const FVector2D& pos)
{
    Ptr<Tile> tile = _tileComponent->GetTile(pos);
    if (!tile) return;
    tile->ToggleFlipY();
    _tileComponent->SetTileInstRefresh(true);
}

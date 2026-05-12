#include "pch.h"
#include "TileComponent.h"

#include "Object/Actor.h"

#include "Core/AssetManager.h"
#include "Core/Texture.h"
#include "Core/Mesh.h"

#include "Shader/ShaderManager.h"
#include "Shader/TileMapCBuffer.h"
#include "Shader/ColorCBuffer.h"
#include "Shader/TranformCBuffer.h"
#include "Shader/TileSBuffer.h"
#include "Shader/TileInstanceShader.h"
#include "Shader/TileLineInstanceShader.h"
#include "Shader/TileLineSBuffer.h"

#include "World/Level.h"

TileComponent::TileComponent()
{
    _isRender = true;
}

TileComponent::~TileComponent()
{}

void TileComponent::RenderTile()
{
    //타일 그리기
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    if (!_tileTexture)
        return;

    if (_useInstancing)
    {
        if (_bTileInstRefresh)
        {
            Ptr<Actor> owner = GetOwner();
            FVector2D ownerPos;
            ownerPos._x = owner->GetWorldPosition()._x;
            ownerPos._y = owner->GetWorldPosition()._y;

            _tileSBuffer->Clear();

            for (int32 i = 0; i < _countY; ++i)
            {
                for (int32 j = 0; j < _countX; ++j)
                {
                    int32 index = i * _countX + j;
                    Ptr<Tile> tile = _tiles[index];

                    int32 frameIndex = tile->GetTextureFrame();
                    if (frameIndex < 0 || frameIndex >= static_cast<int32>(_tileFrames.size()))
                        continue;

                    std::optional<FVector2D> worldPos = GetTileWorldPos(index);

                    FVector2D LTUV, RBUV;
                    LTUV = _tileFrames[frameIndex]._start / _tileTextureSize;
                    RBUV = LTUV + _tileFrames[frameIndex]._size / _tileTextureSize;

                    _tileSBuffer->AddData(worldPos.value(), LTUV, RBUV, _tileSize);
                }
            }

            _tileSBuffer->Update();
            SetTileInstRefresh(false);
        }

        _tileTexture->SetShader(0, SHADER_TYPE::PIXEL, 0);
        ShaderManager::Instance().SetSample(eTextureSampleType::TEXTURE_SAMPLE_LINEAR);

        _transformCBuffer->SetWorldMatrix(FMatrix());
        _transformCBuffer->SetViewMatrix(level->GetViewMatrix());
        _transformCBuffer->SetProjMatrix(level->GetProjMatrix());
        _transformCBuffer->Update();

        _tileSBuffer->Bind();

        _tileInstanceShader->SetShader();

        _tileMesh->RenderInstancing(_tileSBuffer->GetCount());
    }
    else
    {
        _tileTexture->SetShader(0, SHADER_TYPE::PIXEL, 0);
        ShaderManager::Instance().SetSample(eTextureSampleType::TEXTURE_SAMPLE_LINEAR);

        for (int32 i = 0; i < _countY; ++i)
        {
            for (int32 j = 0; j < _countX; ++j)
            {
                int32 index = i * _countX + j;
                Ptr<Tile> tile = _tiles[index];

                int32 frameIndex = tile->GetTextureFrame();
                if (frameIndex < 0 || frameIndex >= static_cast<int32>(_tileFrames.size()))
                    continue;

                FMatrix matScale, matTrans, matWorld;
                matScale.Scaling(tile->GetSize());

                std::optional<FVector2D> worldPos = GetTileWorldPos(index);
                matTrans.Translation(worldPos.value());

                matWorld = matScale * matTrans;

                _transformCBuffer->SetWorldMatrix(matWorld);
                _transformCBuffer->SetViewMatrix(level->GetViewMatrix());
                _transformCBuffer->SetProjMatrix(level->GetProjMatrix());

                FVector2D LTUV, RBUV;
                LTUV = _tileFrames[frameIndex]._start / _tileTextureSize;
                RBUV = LTUV + _tileFrames[frameIndex]._size / _tileTextureSize;

                _tileMapCBuffer->SetUV(LTUV, RBUV);

                _transformCBuffer->Update();
                _tileMapCBuffer->Update();

                _tileShader->SetShader();
                _tileMesh->Render();
            }
        }
    }
}

void TileComponent::RenderOutLine()
{
    //타일의 외곽선 그리기
    if (!_outLineRender)
        return;

    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    if (_useInstancing)
    {
        if (_bTileLineInstRefresh)
        {
            for (int32 i = 0; i < _countY; ++i)
            {
                for (int32 j = 0; j < _countX; ++j)
                {
                    int32 index = i * _countX + j;
                    Ptr<Tile> tile = _tiles[index];

                    int32 frameIndex = tile->GetTextureFrame();
                    if (frameIndex < 0 || frameIndex >= static_cast<int32>(_tileFrames.size()))
                        continue;

                    std::optional<FVector2D> worldPos = GetTileWorldPos(index);

                    FVector4D color;
                    switch (tile->GetTileType())
                    {
                    case eTileType::NORMAL:
                        color = FVector4D(0.f, 1.f, 0.f, 1.f);
                        break;
                    case eTileType::BLOCK:
                        color = FVector4D(1.f, 0.f, 0.f, 1.f);
                        break;
                    default:
                        break;
                    }

                    _tileLineSBuffer->AddData(worldPos.value(), color, _tileSize);
                }
            }

            _tileLineSBuffer->Update();
            SetTileLineInstRefresh(false);
        }

        _transformCBuffer->SetWorldMatrix(FMatrix());
        _transformCBuffer->SetViewMatrix(level->GetViewMatrix());
        _transformCBuffer->SetProjMatrix(level->GetProjMatrix());

        _tileLineSBuffer->Bind();

        _transformCBuffer->Update();

        _tileLineInstanceShader->SetShader();

        _outLineMesh->RenderInstancing(_tileLineSBuffer->GetCount());
    }
    else
    {
        for (int32 i = 0; i < _countY; ++i)
        {
            for (int32 j = 0; j < _countX; ++j)
            {
                int32 index = i * _countX + j;
                Ptr<Tile> tile = _tiles[index];

                int32 frameIndex = tile->GetTextureFrame();
                if (frameIndex < 0 || frameIndex >= static_cast<int32>(_tileFrames.size()))
                    continue;

                FMatrix matScale, matTrans, matWorld;
                matScale.Scaling(tile->GetSize());

                std::optional<FVector2D> worldPos = GetTileWorldPos(index);
                matTrans.Translation(worldPos.value());

                matWorld = matScale * matTrans;

                _transformCBuffer->SetWorldMatrix(matWorld);
                _transformCBuffer->SetViewMatrix(level->GetViewMatrix());
                _transformCBuffer->SetProjMatrix(level->GetProjMatrix());

                switch (tile->GetTileType())
                {
                case eTileType::NORMAL:
                    _outLineCBuffer->SetColor(0.f, 1.f, 0.f, 1.f);
                    break;
                case eTileType::BLOCK:
                    _outLineCBuffer->SetColor(1.f, 0.f, 0.f, 1.f);
                    break;
                default:
                    break;
                }

                _transformCBuffer->Update();
                _outLineCBuffer->Update();

                _outLineShader->SetShader();
                _outLineMesh->Render();
            }
        }
    }
}

bool TileComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(id, name, owner);

    _tileMesh = MESH_MANAGER->FindMesh("TexRect");
    _tileMapCBuffer = FIND_CBUFFER("TileMap", TileMapCBuffer);
    _tileShader = FIND_SHADER("TileShader");

    _outLineMesh = MESH_MANAGER->FindMesh("frameRect");
    _outLineShader = FIND_SHADER("FrameMeshShader");
    _outLineCBuffer = FIND_CBUFFER("Color", ColorCBuffer);

#ifdef _DEBUG
    _outLineRender = true;
#endif // _DEBUG

    _tileSBuffer = FIND_SBUFFER("TileInstance", TileSBuffer);
    _tileInstanceShader = FIND_SHADER("TileInstanceShader");

    _tileLineSBuffer = FIND_SBUFFER("TileLineInstance", TileLineSBuffer);
    _tileLineInstanceShader = FIND_SHADER("TileLineInstanceShader");

    SetRenderLayer("BackGround");

    return true;
}

void TileComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);
}

void TileComponent::Collision(float deltaTiem)
{
    SceneComponent::Collision(deltaTiem);
}

void TileComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    RenderTile();
    RenderOutLine();
}

void TileComponent::Destroy()
{
    SceneComponent::Destroy();

    for (auto& it : _tiles)
        DESTROY(it);

    _tiles.clear();
}

void TileComponent::SetTexture(const std::string& name)
{
    Ptr<Texture> foundTexture = TEXTURE_MANAGER->Findtexture(name);
    if (!foundTexture)
        return;

    SetTexture(foundTexture);
}

void TileComponent::SetTexture(Ptr<class Texture> texture)
{
    _tileTexture = texture;

    _tileTextureSize._x = static_cast<float>(_tileTexture->GetTexture(0)->_width);
    _tileTextureSize._y = static_cast<float>(_tileTexture->GetTexture(0)->_height);

    SetTileInstRefresh(true);
}

void TileComponent::SetTexture(const std::string& name, const std::wstring& fileName)
{
    if (!TEXTURE_MANAGER->LoadTexture(name, fileName))
        return;

    Ptr<Texture> texture = TEXTURE_MANAGER->Findtexture(name);
    if (texture)
        return SetTexture(texture);
}

void TileComponent::AddTileFrame(const FVector2D& start, const FVector2D& size)
{
    AddTileFrame(start._x, start._y, size._x, size._y);
}

void TileComponent::AddTileFrame(float startX, float startY, float sizeX, float sizeY)
{
    FAnimationFrame frame;
    frame._start._x = startX;
    frame._start._y = startY;
    frame._size._x = sizeX;
    frame._size._y = sizeY;

    _tileFrames.push_back(frame);

    SetTileInstRefresh(true);
}

int32 TileComponent::GetTileIndexX(const FVector2D& pos) const
{
    Ptr<Actor> owner = GetOwner();
    if (nullptr == owner)
        return -1;

    //타일이 위치가 중앙이 중심이므로,
    //시작점은 ownerPos - 타일사이즈 * 0.5;
    float startX = owner->GetWorldPosition()._x - (_tileSize._x * 0.5f);

    float localX = pos._x - startX;

    if (localX < 0.f)
        return -1;

    int32 indexX = static_cast<int32>(localX / _tileSize._x);
    if (indexX >= _countX)
        return -1;

    return indexX;
}

int32 TileComponent::GetTileIndexY(const FVector2D& pos) const
{
    Ptr<Actor> owner = GetOwner();
    if (nullptr == owner)
        return -1;

    //타일이 위치가 중앙이 중심이므로,
    //시작점은 ownerPos - 타일사이즈 * 0.5;
    float startY = owner->GetWorldPosition()._y - (_tileSize._y * 0.5f);

    float localY = pos._y - startY;

    if (localY < 0.f)
        return -1;

    int32 indexY = static_cast<int32>(localY / _tileSize._y);
    if (indexY >= _countY)
        return -1;

    return indexY;
}

int32 TileComponent::GetTileIndex(const FVector2D& pos) const
{
    int32 indexX = GetTileIndexX(pos);
    int32 indexY = GetTileIndexY(pos);

    if (indexX == -1 || indexY == -1)
        return -1;

    return indexY * _countX + indexX;
}

Ptr<Tile> TileComponent::GetTile(const FVector2D& pos)
{
    int32 index = GetTileIndex(pos);
    if (index == -1)
        return nullptr;

    return _tiles[index];
}

Ptr<Tile> TileComponent::GetTile(int32 index)
{
    if (index == -1)
        return nullptr;

    return _tiles[index];
}

std::optional<FVector2D> TileComponent::GetTileWorldPos(int32 index)
{
    Ptr<Actor> owner = GetOwner();
    if (!owner)
        return std::nullopt; //없다.

    FVector2D ownerPos;
    ownerPos._x = owner->GetWorldPosition()._x;
    ownerPos._y = owner->GetWorldPosition()._y;

    return _tiles[index]->GetPos() + ownerPos;
}


void TileComponent::CreateTile(int32 countX, int32 countY, const FVector2D& tileSize, int32 tileTextureFrame)
{
    _countX = countX;
    _countY = countY;

    _tileSize = tileSize;

    _mapSize._x = _countX * _tileSize._x;
    _mapSize._y = _countY * _tileSize._y;

    _tiles.resize(countX * countY);

    for (int32 i = 0; i < _countY; ++i)
    {
        for (int32 j = 0; j < _countX; ++j)
        {
            Ptr<Tile> tile = New<Tile>();
            tile->_type = eTileType::NORMAL;

            tile->_pos._x = j * _tileSize._x;
            tile->_pos._y = i * _tileSize._y;

            tile->_size = _tileSize;
            tile->_center = tile->_pos + tileSize * 0.5f;
            tile->_textureFrame = tileTextureFrame;

            _tiles[i * _countX + j] = tile;
        }
    }

    SetTileInstRefresh(true);
    SetTileLineInstRefresh(true);
}

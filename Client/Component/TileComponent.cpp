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
                    // UV 계산

                    FVector2D LTUV, RBUV;
                    LTUV = _tileFrames[frameIndex]._start / _tileTextureSize;
                    RBUV = LTUV + _tileFrames[frameIndex]._size / _tileTextureSize;
                    // 반전 처리
                    if (tile->GetFlipX())
                    {
                        float tmpX = LTUV._x;
                        LTUV._x = RBUV._x;
                        RBUV._x = tmpX;
                    }
                    if (tile->GetFlipY())
                    {
                        float tmpY = LTUV._y;
                        LTUV._y = RBUV._y;
                        RBUV._y = tmpY;
                    }
                    
                    //
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
            _tileLineSBuffer->Clear();
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

bool TileComponent::IsBlocked(const FVector2D& worldPos)
{
    Ptr<Tile> tile = GetTile(worldPos);

    if (!tile)
        return true;

    return tile->GetTileType() == eTileType::BLOCK;

}

bool TileComponent::IsBlocked(const FVector3D& worldPos)
{
    return IsBlocked(FVector2D(worldPos._x, worldPos._y));
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
    _tileSBuffer = New<TileSBuffer>();
    _tileSBuffer->Create(sizeof(FTileInstanceData), 100, 1, SHADER_TYPE::VERTEX);
    //_tileSBuffer = FIND_SBUFFER("TileInstance", TileSBuffer);
    _tileInstanceShader = FIND_SHADER("TileInstanceShader");

    _tileLineSBuffer = New<TileLineSBuffer>();
    _tileLineSBuffer->Create(sizeof(FTileLineInstanceData), 100, 2, SHADER_TYPE::VERTEX);
    //_tileLineSBuffer = FIND_SBUFFER("TileLineInstance", TileLineSBuffer);
    _tileLineInstanceShader = FIND_SHADER("TileLineInstanceShader");

    SetRenderLayer("BackGround");

    return true;
}

void TileComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);

    Ptr<Actor> owner = GetOwner();
    if (!owner)
        return;
    FVector3D curOwnerPos = owner->GetWorldPosition();

    if (!_ownerPosInit)
    {
        _prevOwnerPos = curOwnerPos;
        _ownerPosInit = true;
        return;
    }

    if (_prevOwnerPos._x != curOwnerPos._x ||
        _prevOwnerPos._y != curOwnerPos._y ||
        _prevOwnerPos._z != curOwnerPos._z)
    {
        SetTileInstRefresh(true);
        SetTileLineInstRefresh(true);

        _prevOwnerPos = curOwnerPos;
    }
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

    DESTROY(_tileSBuffer);
    DESTROY(_tileLineSBuffer);
}

void TileComponent::Save(std::ofstream& file)
{
    // 타일 기본 정보
    file.write((char*)&_countX, sizeof(int32));
    file.write((char*)&_countY, sizeof(int32));
    file.write((char*)&_tileSize, sizeof(FVector2D));

    // 텍스처 경로
    int32 pathLen = (int32)_texturePath.size();
    file.write((char*)&pathLen, sizeof(int32));
    file.write((char*)_texturePath.c_str(), pathLen * sizeof(wchar_t));

    int32 nameLen = (int32)_textureName.size();
    file.write((char*)&nameLen, sizeof(int32));
    file.write(_textureName.c_str(), nameLen);

    // 프레임 정보
    int32 frameCount = (int32)_tileFrames.size();
    file.write((char*)&frameCount, sizeof(int32));
    for (auto& frame : _tileFrames)
    {
        file.write((char*)&frame._start, sizeof(FVector2D));
        file.write((char*)&frame._size, sizeof(FVector2D));
    }

    // 각 타일 상태
    int32 tileCount = (int32)_tiles.size();
    file.write((char*)&tileCount, sizeof(int32));
    for (auto& tile : _tiles)
    {
        eTileType type = tile->GetTileType();
        int32 texFrame = tile->GetTextureFrame();
        bool flipX = tile->GetFlipX();
        bool flipY = tile->GetFlipY();
        file.write((char*)&type, sizeof(eTileType));
        file.write((char*)&texFrame, sizeof(int32));
        file.write((char*)&flipX, sizeof(bool));
        file.write((char*)&flipY, sizeof(bool));
    }

    
}

void TileComponent::Load(std::ifstream& file)
{
    // 타일 기본 정보
    file.read((char*)&_countX, sizeof(int32));
    file.read((char*)&_countY, sizeof(int32));
    file.read((char*)&_tileSize, sizeof(FVector2D));

    // 텍스처 경로
    int32 pathLen = 0;
    file.read((char*)&pathLen, sizeof(int32));
    _texturePath.resize(pathLen);
    file.read(reinterpret_cast<char*>(_texturePath.data()), pathLen * sizeof(wchar_t));

    int32 nameLen = 0;
    file.read((char*)&nameLen, sizeof(int32));
    _textureName.resize(nameLen);
    file.read(_textureName.data(), nameLen);

    // 텍스처 로드
    SetTexture(_textureName, _texturePath);

    // 프레임 정보
    int32 frameCount = 0;
    file.read((char*)&frameCount, sizeof(int32));
    _tileFrames.clear();
    for (int32 i = 0; i < frameCount; ++i)
    {
        FAnimationFrame frame;
        file.read((char*)&frame._start, sizeof(FVector2D));
        file.read((char*)&frame._size, sizeof(FVector2D));
        _tileFrames.push_back(frame);
    }

    // 타일 생성 후 상태 복원
    CreateTile(_countX, _countY, _tileSize, 0);
    int32 tileCount = 0;
    file.read((char*)&tileCount, sizeof(int32));
    for (int32 i = 0; i < tileCount && i < (int32)_tiles.size(); ++i)
    {
        eTileType type; int32 texFrame;
        bool flipX = false, flipY = false;
        file.read((char*)&type, sizeof(eTileType));
        file.read((char*)&texFrame, sizeof(int32));
        file.read((char*)&flipX, sizeof(bool));
        file.read((char*)&flipY, sizeof(bool));
        _tiles[i]->SetTileType(type);
        _tiles[i]->SetTextureFrame(texFrame);
        _tiles[i]->SetFlipX(flipX);
        _tiles[i]->SetFlipY(flipY);
    }

    SetTileInstRefresh(true);
    SetTileLineInstRefresh(true);
}

void TileComponent::RemoveTileFrame(int32 idx)
{
    if (idx < 0 || idx >= (int32)_tileFrames.size())
        return;
    _tileFrames.erase(_tileFrames.begin() + idx);
    // 삭제된 프레임 이후 인덱스를 참조하던 타일들 보정.
    for (auto& tile : _tiles)
    {
        int32 frame = tile->GetTextureFrame();
        if (frame == idx)
            tile->SetTextureFrame(0);
        else if (frame > idx)
            tile->SetTextureFrame(frame - 1);
    }
    SetTileInstRefresh(true);
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
    
    _textureName = name;
    _texturePath = fileName;

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

#pragma once
#include "Actor.h"

class TileMap : public Actor
{
public:
    TileMap();
    virtual ~TileMap();

private:
    
    //타일 드래그 선택 범위 
    bool _isTileDragging = false;
    bool _hasTileSelection = false;
    int32 _paintFrameIndex = -1;

    int32 _tileSelectStartIdx = -1;
    int32 _tileSelectEndIdx = -1;

    // 타일 회전 반전
    bool _flipX = false;    // 좌우
    bool _flipY = false;    // 상하
    
    Ptr<class TileComponent> _tileComponent;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;
    Ptr<TileComponent> GetTileComponent() const { return _tileComponent; }

    void ChangeTileTexture(const FVector2D& pos, int32 frameIndex);
    void ChangeTileType(const FVector2D& pos);
    void Save(std::ofstream& file);
    void Load(std::ifstream& file);
    void SetPaintFrameIndex(int32 idx) { _paintFrameIndex = idx; }
    bool HasTileSelection() const { return _hasTileSelection; }

    // 선택된 범위에 프레임 일괄 적용
    void ApplyFrameToSelection(int32 frameIndex);

    // 선택 범위의 시작/끝 타일 인덱스
    int32 GetTileSelectStartIdx() const { return _tileSelectStartIdx; }
    int32 GetTileSelectEndIdx() const { return _tileSelectEndIdx; }
    void ClearTileSelection() { _hasTileSelection = false; _tileSelectStartIdx = -1; _tileSelectEndIdx = -1; }

};


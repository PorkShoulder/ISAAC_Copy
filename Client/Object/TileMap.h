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
    int32 _viewFrameIndex = -1;

    int32 _tileSelectStartIdx = -1;
    int32 _tileSelectEndIdx = -1;

    // 타일 flip 상태 추가
    bool _viewFlipX = false;
    bool _viewFlipY = false;
    
    Ptr<class TileComponent> _tileComponent;

public:
    // 타일 빈 공간확인
    int32 _gridW = 1;
    int32 _gridH = 1;
    std::vector <std::pair<int32, int32>> _emptyCells;
    // Apply 그리드 확인용
    int32 GetGridW() const { return _gridW; }
    int32 GetGridH() const { return _gridH; }
    const std::vector<std::pair<int32, int32>>& GetEmptyCells() const { return _emptyCells; }

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
    void SetviewFrameIndex(int32 idx) { _viewFrameIndex = idx; }
    bool HasTileSelection() const { return _hasTileSelection; }

    // 선택된 범위에 프레임 일괄 적용
    void ApplyFrameToSelection(int32 frameIndex, bool flipX, bool flipY);

    // 선택 범위의 시작/끝 타일 인덱스
    int32 GetTileSelectStartIdx() const { return _tileSelectStartIdx; }
    int32 GetTileSelectEndIdx() const { return _tileSelectEndIdx; }
    void ClearTileSelection() { _hasTileSelection = false; _tileSelectStartIdx = -1; _tileSelectEndIdx = -1; }

    // 반전 토글기능추가
    void FlipTileX(const FVector2D& pos);
    void FlipTileY(const FVector2D& pos);

    // 미리보기 반전
    void SetViewFlipX(bool v) { _viewFlipX = v; }
    void SetViewFlipY(bool v) { _viewFlipY = v; }

    //빈 공간 판별
    void DetectEmptyCells();

    //그리드 2x2 빈칸 정보 전달
    void SetEmptyCells(const std::vector<std::pair<int32, int32>>& cells) { _emptyCells = cells; }



};


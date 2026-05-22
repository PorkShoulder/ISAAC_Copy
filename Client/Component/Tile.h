#pragma once
#include "Core/Object.h"

//타일 하나를 표현하는 클래스
class Tile : public Object
{
    friend class TileComponent;

public:
    Tile() = default;
    virtual ~Tile() = default;

private:
    //타일의 속성
    eTileType _type = eTileType::END;

    //타일의 위치
    FVector2D _pos;

    //타일의 크기
    FVector2D _size;

    //타일의 중앙 위치
    FVector2D _center;

    //타일의 텍스쳐 프레임
    int32 _textureFrame = -1;

    // 타일 회전 반전
    bool _flipX = false;    // 좌우
    bool _flipY = false;    // 상하

public:
    const eTileType GetTileType() const;
    void SetTileType(eTileType type);

    const FVector2D& GetPos() const;
    void SetPos(const FVector2D& pos);

    const FVector2D& GetSize() const;
    void SetSize(const FVector2D& size);

    const FVector2D& GetCenter() const;
    void SetCenter(const FVector2D& center);

    const int32 GetTextureFrame() const;
    void SetTextureFrame(int32 frame);

    // 등록된 타일 반전
    bool GetFlipX() const { return _flipX; }
    bool GetFlipY() const { return _flipY; }
    void SetFlipX(bool flip) { _flipX = flip; }
    void SetFlipY(bool flip) { _flipY = flip; }
    void ToggleFlipX() { _flipX = !_flipX; }
    void ToggleFlipY() { _flipY = !_flipY; }

    virtual void Destroy() override;
};


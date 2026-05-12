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


    virtual void Destroy() override;
};


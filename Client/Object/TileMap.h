#pragma once
#include "Actor.h"

//================================
// TileComponent 이용해서 격자형 맵을 생성
// 마우스 클릭으로 텍스처 타입 변경
//================================


class TileMap : public Actor
{
public:
    TileMap();
    virtual ~TileMap();

private:
    Ptr<class TileComponent> _tileComponent;    //실제 타일의 데이터와 렌더링 담당.

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot);
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);

    virtual void Destroy() override;

    void ChangeTileTexture(const FVector2D& pos);   // 좌클릭시 해당 타일의 텍스처 프레임 순환변경
    void ChangeTileType(const FVector2D& pos);      // 우클릭시 이동 가능, 불가능 변경.
};


#pragma once
#include "Actor.h"

class TileMap : public Actor
{
public:
    TileMap();
    virtual ~TileMap();

private:
    Ptr<class TileComponent> _tileComponent;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;

    virtual void Destroy() override;

    void ChangeTileTexture(const FVector2D& pos);
    void ChangeTileType(const FVector2D& pos);

    Ptr<TileComponent> GetTileComponent() const { return _tileComponent; }

};


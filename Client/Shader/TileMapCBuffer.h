#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class TileMapCBuffer : public CBuffer
{
public:
    TileMapCBuffer();
    virtual ~TileMapCBuffer();


private:
    FTileMapCBufferData _data;

public:
    virtual void Update() override;

    void SetUV(const FVector2D& uvLT, const FVector2D& uvRB)
    {
        _data._uvLT = uvLT;
        _data._uvRB = uvRB;
    }
};


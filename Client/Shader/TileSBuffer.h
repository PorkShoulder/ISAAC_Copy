#pragma once
#include "SBuffer.h"

class TileSBuffer : public SBuffer
{
public:
    TileSBuffer();
    virtual ~TileSBuffer();

private:
    std::vector<FTileInstanceData> _datas;

public:
    virtual void Update() override;
    void AddData(const FVector2D& pos, const FVector2D& uvLT, const FVector2D& uvRB, const FVector2D& size)
    {
        FTileInstanceData data;
        data._worldPos = pos;
        data._uvLT = uvLT;
        data._uvRB = uvRB;
        data._size = size;

        _datas.push_back(data);
    }

    void Clear()
    {
        _datas.clear();
    }

    int32 GetCount()
    {
        return static_cast<int32>(_datas.size());
    }
};


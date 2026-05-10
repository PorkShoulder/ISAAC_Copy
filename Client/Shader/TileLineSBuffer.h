#pragma once
#include "SBuffer.h"

class TileLineSBuffer : public SBuffer
{
public:
    TileLineSBuffer();
    virtual ~TileLineSBuffer();

private:
    std::vector<FTileLineInstanceData> _datas;

public:
    virtual void Update() override;

    void AddData(const FVector2D& pos, const FVector4D& color, const FVector2D& size)
    {
        FTileLineInstanceData data;
        data._worldPos = pos;
        data._color = color;
        data._size = size;

        _datas.push_back(data);
    }

    void Clear()
    {
        _datas.clear();
    }

    int GetCount()
    {
        return static_cast<int32>(_datas.size());
    }
};


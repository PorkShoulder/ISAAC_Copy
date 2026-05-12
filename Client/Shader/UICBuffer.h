#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class UICBuffer : public CBuffer
{
public:
    UICBuffer();
    virtual ~UICBuffer();

private:
    FUIBrushData _data;

public:
    virtual void Update() override;
    virtual void Destroy() override;

    void SetTint(const FVector4D& tint)
    {
        _data._tint = tint;
    }

    void SetUV(const FVector2D& uvLT, const FVector2D& uvRB)
    {
        _data._uvLT = uvLT;
        _data._uvRB = uvRB;
    }

    void SetAnimEnable(bool enable)
    {
        _data._animEnable = enable ? 1 : 0;
    }

    void SetTextureEnable(bool enable)
    {
        _data._textureEnable = enable ? 1 : 0;
    }
};


#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

class AnimCBuffer : public CBuffer
{
public:
    AnimCBuffer();
    virtual ~AnimCBuffer();

private:
    FAnimation2DCBufferData _data;

public:
    virtual void Update() override;
    void SetUV(float ltx, float lty, float rbx, float rby);
    void SetAnimFilp(bool filp);
};


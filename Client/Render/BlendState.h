#pragma once
#include "RenderState.h"

//블렌드스페이스 처리를 할꺼에요.
class BlendState : public RenderState
{
public:
    BlendState();
    virtual ~BlendState();

protected:
    std::vector<D3D11_RENDER_TARGET_BLEND_DESC> _desc;
    float _blendFactor[4] = {};
    float _prevblendFactor[4] = {};
    UINT _sampleMask = 0xffffffff;
    UINT _prevSampleMask = 0;

public:
    void SetBlendFactor(float r, float g, float b, float a);
    void SetSampleMask(UINT mask);
    void AddBlendDesc(bool enable, D3D11_BLEND src,
        D3D11_BLEND dest, D3D11_BLEND_OP blendop,
        D3D11_BLEND srcAlpha, D3D11_BLEND destAlpha,
        D3D11_BLEND_OP alphaOp, UINT8 renderTargetWriteMask);
    bool CreateState(bool alphaToCoverage, bool Indendent);

public:
    virtual void SetState() override;
    virtual void ResetState() override;
    virtual void Destroy() override;

};


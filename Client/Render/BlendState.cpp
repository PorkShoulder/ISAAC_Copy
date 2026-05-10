#include "pch.h"
#include "BlendState.h"
#include "../Core/Device.h"

BlendState::BlendState()
{
    _type = eRenderState::BLEND;
}

BlendState::~BlendState()
{}

void BlendState::SetBlendFactor(float r, float g, float b, float a)
{
    _blendFactor[0] = r;
    _blendFactor[1] = g;
    _blendFactor[2] = b;
    _blendFactor[3] = a;
}

void BlendState::SetSampleMask(UINT mask)
{
    _sampleMask = mask;
}

void BlendState::AddBlendDesc(bool enable, D3D11_BLEND src, D3D11_BLEND dest, D3D11_BLEND_OP blendop, D3D11_BLEND srcAlpha, D3D11_BLEND destAlpha, D3D11_BLEND_OP alphaOp, UINT8 renderTargetWriteMask)
{
    D3D11_RENDER_TARGET_BLEND_DESC desc = {};

    desc.BlendEnable = enable;
    desc.SrcBlend = src;
    desc.DestBlend = dest;
    desc.BlendOp = blendop;
    desc.SrcBlendAlpha = srcAlpha;
    desc.DestBlendAlpha = destAlpha;
    desc.BlendOpAlpha = alphaOp;
    desc.RenderTargetWriteMask = renderTargetWriteMask;

    _desc.emplace_back(desc);
}

bool BlendState::CreateState(bool alphaToCoverage, bool Indendent)
{
    if (_desc.empty())
        return false;

    //블렌딩 상태를 정의하는 구조체
    D3D11_BLEND_DESC desc = {};

    //알파투커버리지
    //멀티샘플링 할때 픽셀의 알파값을 기반으로 커버리지 마스크를 생성
    //이때 투명한물체의 외곽선을 부드럽게
    desc.AlphaToCoverageEnable = alphaToCoverage;

    //멀티랜더타켓 사용할때, 각 랜더타켓 별로 다른 블랜딩 설정
    desc.IndependentBlendEnable = Indendent;
    
    //랜더타겟 기본값
    for (int i = 0; i < 8; ++i)
    {
        desc.RenderTarget[i].BlendEnable = false;
        desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
        desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
        desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    memcpy(desc.RenderTarget, &_desc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * _desc.size());

    if (FAILED(Device::Instance().GetDevice()->CreateBlendState(&desc, (ID3D11BlendState**)_state.GetAddressOf())))
        return false;

    return true;
}

void BlendState::SetState()
{
    Device::Instance().GetContext()->OMGetBlendState((ID3D11BlendState**)_prevState.GetAddressOf(), _prevblendFactor, &_prevSampleMask);
    //
    Device::Instance().GetContext()->OMSetBlendState((ID3D11BlendState*)_state.Get(), _blendFactor, _sampleMask);
}

void BlendState::ResetState()
{
    Device::Instance().GetContext()->OMSetBlendState((ID3D11BlendState*)_prevState.Get(), _prevblendFactor, _prevSampleMask);

    _prevState = nullptr;
}

void BlendState::Destroy()
{

}

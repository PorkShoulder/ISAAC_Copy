#include "pch.h"
#include "DepthStencilState.h"
#include "Core/Device.h"

DepthStencilState::DepthStencilState()
{
    _type = eRenderState::DEPTHSTENCIL;
}

DepthStencilState::~DepthStencilState()
{}

bool DepthStencilState::CreateState(bool depthEnalbe, D3D11_DEPTH_WRITE_MASK depthWriteMask, D3D11_COMPARISON_FUNC depthFunc, bool stencilEnalbe, UINT8 stencilReadMask, UINT8 stencilWriteMask, D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace)
{
    //뎁스 스탠실 상태 구조체
    D3D11_DEPTH_STENCIL_DESC desc = {};

    desc.DepthEnable = depthEnalbe; //깊이 테스트 할 지 말지
    desc.DepthWriteMask = depthWriteMask; //깊어 버퍼 쓰기 방식
    desc.DepthFunc = depthFunc;//깊이 비교 방식(보통은 less)

    desc.StencilEnable = stencilEnalbe;
    desc.StencilReadMask = stencilReadMask;
    desc.StencilWriteMask = stencilWriteMask;

    desc.FrontFace = frontFace; //앞면 : 스텐실 실패, 깊이 실패/통과 시 처리 방식
    desc.BackFace = backFace; //뒷면


    if(FAILED(Device::Instance().GetDevice()->CreateDepthStencilState(&desc, (ID3D11DepthStencilState**)_state.GetAddressOf())))
        return false;

    return true;
}

void DepthStencilState::SetState()
{
    Device::Instance().GetContext()->OMGetDepthStencilState((ID3D11DepthStencilState**)_prevState.GetAddressOf(), &_prevStencilRef);
    //
    Device::Instance().GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)_state.Get(), _stencilRef);
}

void DepthStencilState::ResetState()
{
    Device::Instance().GetContext()->OMSetDepthStencilState((ID3D11DepthStencilState*)_prevState.Get(), _prevStencilRef);

    _prevState = nullptr;
}

void DepthStencilState::Destroy()
{}

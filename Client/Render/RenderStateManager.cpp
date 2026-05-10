#include "pch.h"
#include "RenderStateManager.h"
#include "BlendState.h"
#include "DepthStencilState.h"


RenderStateManager::RenderStateManager()
{}

RenderStateManager::~RenderStateManager()
{}

bool RenderStateManager::Init()
{
    //색상 섞는다 -->알파블렌드
    //최종 색상 : srcColor * invSrcA + destColor * destA
    AddBlendDesc("AlphaBlend", 
        true,                               //blendEnable
        D3D11_BLEND_SRC_ALPHA,              //srcBlend      ->  srcColor에 곱할 가중치 (알파)
        D3D11_BLEND_INV_SRC_ALPHA,          //destBlend     ->  destColor 에 곱할 가중치 (1-픽셀알파)
        D3D11_BLEND_OP_ADD,                 //                  둘을 더함
        D3D11_BLEND_ONE,
        D3D11_BLEND_ZERO,
        D3D11_BLEND_OP_ADD,
        D3D11_COLOR_WRITE_ENABLE_ALL);

    CreateState("AlphaBlend", true, false);

    CreateDepthStencilState("DepthStencil", false);

    return true;
}

void RenderStateManager::SetBlendFactor(const std::string& name, float r, float g, float b, float a)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state = New<BlendState>();
        _renderStates[name] = state;
    }

    state->SetBlendFactor(r, g, b, a);
}

void RenderStateManager::SetSampleMask(const std::string & name, UINT mask)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state = New<BlendState>();
        _renderStates[name] = state;
    }

    state->SetSampleMask(mask);
}

void RenderStateManager::AddBlendDesc(const std::string & name, bool enable, D3D11_BLEND src, D3D11_BLEND dest, D3D11_BLEND_OP blendop, D3D11_BLEND srcAlpha, D3D11_BLEND destAlpha, D3D11_BLEND_OP alphaOp, UINT8 renderTargetWriteMask)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state = New<BlendState>();
        _renderStates[name] = state;
    }

    state->AddBlendDesc(enable, src, dest, blendop, srcAlpha, destAlpha, alphaOp, renderTargetWriteMask);
}

bool RenderStateManager::CreateState(const std::string & name, bool alphaToCoverage, bool Indentent)
{
    Ptr<BlendState> state = FindRenderState<BlendState>(name);
    if (nullptr == state)
    {
        state = New<BlendState>();
        _renderStates[name] = state;
    }

    if (!state->CreateState(alphaToCoverage, Indentent))
    {
        DESTROY(state);
        return false;
    }

    return true;
}

bool RenderStateManager::CreateDepthStencilState(const std::string& name, bool depthEnalbe, D3D11_DEPTH_WRITE_MASK depthWriteMask, D3D11_COMPARISON_FUNC depthFunc, bool stencilEnalbe, UINT8 stencilReadMask, UINT8 stencilWriteMask, D3D11_DEPTH_STENCILOP_DESC frontFace, D3D11_DEPTH_STENCILOP_DESC backFace)
{
    Ptr<DepthStencilState> state = FindRenderState<DepthStencilState>(name);
    if (nullptr != state)
        return false;

    state = New<DepthStencilState>();
    if (!state->CreateState(depthEnalbe, depthWriteMask, depthFunc, stencilEnalbe, stencilReadMask, stencilWriteMask, frontFace, backFace))
    {
        DESTROY(state);
        return false;
    }

    _renderStates[name] = state;

    return true;
}

void RenderStateManager::Destroy()
{
    for (auto& it : _renderStates)
    {
        DESTROY(it.second);
    }
}

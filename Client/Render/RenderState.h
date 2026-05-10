#pragma once
#include "../Core/Object.h"

class RenderState : public Object
{
public:
    RenderState() = default;
    virtual ~RenderState() = default;

protected:
    ComPtr<ID3D11DeviceChild> _state;
    ComPtr<ID3D11DeviceChild> _prevState;
    eRenderState _type = eRenderState::END;

public:
    virtual void SetState() = 0;
    virtual void ResetState() = 0;

};


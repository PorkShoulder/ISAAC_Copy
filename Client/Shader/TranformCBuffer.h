#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

//===================================
// Transf
//===================================

class TranformCBuffer : public CBuffer
{
public:
    TranformCBuffer();
    virtual ~TranformCBuffer();

private:
    FTransformCBufferData _data;

public:
    virtual void Update();

    void SetWorldMatrix(const FMatrix& mat)
    {
        _data._world = mat;
    }

    void SetViewMatrix(const FMatrix& mat)
    {
        _data._view = mat;
    }

    void SetProjMatrix(const FMatrix& mat)
    {
        _data._proj = mat;
    }
};


#include "pch.h"
#include "TranformCBuffer.h"

TranformCBuffer::TranformCBuffer()
{
}

TranformCBuffer::~TranformCBuffer()
{
}

void TranformCBuffer::Update()
{
    _data._wv = _data._world * _data._view;
    _data._wvp = _data._wv * _data._proj;

    _data._world.Transpose();
    _data._view.Transpose();
    _data._proj.Transpose();
    _data._wv.Transpose();
    _data._wvp.Transpose();

    SetData(&_data);
}

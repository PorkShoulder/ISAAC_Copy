#include "pch.h"
#include "AnimCBuffer.h"

AnimCBuffer::AnimCBuffer()
{}

AnimCBuffer::~AnimCBuffer()
{}

void AnimCBuffer::Update()
{
    SetData(&_data);
}

void AnimCBuffer::SetUV(float ltx, float lty, float rbx, float rby)
{
    _data._uvLT._x = ltx;
    _data._uvLT._y = lty;

    _data._uvRB._x = rbx;
    _data._uvRB._y = rby;
}

void AnimCBuffer::SetAnimFilp(bool filp)
{
    _data._animFilp = filp ? 1 : 0;
}

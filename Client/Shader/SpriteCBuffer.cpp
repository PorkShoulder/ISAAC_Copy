#include "pch.h"
#include "SpriteCBuffer.h"

SpriteCBuffer::SpriteCBuffer()
{}

SpriteCBuffer::~SpriteCBuffer()
{}

void SpriteCBuffer::Update()
{
    SetData(&_data);
}

void SpriteCBuffer::Destroy()
{}

void SpriteCBuffer::SetTint(float r, float g, float b, float a)
{
    _data._tint._x = r;
    _data._tint._y = g;
    _data._tint._z = b;
    _data._tint._w = a;
}

void SpriteCBuffer::SetTint(const FVector4D& color)
{
    SetTint(color._x, color._y, color._z, color._w);
}

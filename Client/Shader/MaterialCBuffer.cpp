#include "pch.h"
#include "MaterialCBuffer.h"

MaterialCBuffer::MaterialCBuffer()
{
}

MaterialCBuffer::~MaterialCBuffer()
{
}

void MaterialCBuffer::Update()
{
	SetData(&_data);
}

void MaterialCBuffer::SetBaseColor(float r, float g, float b, float a)
{
    _data._basaColor._x = r;
    _data._basaColor._y = g;
    _data._basaColor._z = b;
    _data._basaColor._w = a;
}

void MaterialCBuffer::SetBaseColor(const FVector4D& color)
{
    SetBaseColor(color._x, color._y, color._z, color._w);
}

void MaterialCBuffer::SetOpacity(float op)
{
    _data._opacity = op;
}

void MaterialCBuffer::SetTextureWidth(int32 w)
{
    _data._textureWidht = w;
}

void MaterialCBuffer::SetTextureHeight(int32 h)
{
    _data._textureHeight = h;
}

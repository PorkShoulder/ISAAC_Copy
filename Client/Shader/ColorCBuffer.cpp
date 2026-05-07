#include "pch.h"
#include "ColorCBuffer.h"

ColorCBuffer::ColorCBuffer()
{
}

ColorCBuffer::~ColorCBuffer()
{
}

void ColorCBuffer::Update()
{
	SetData(&_data);
}

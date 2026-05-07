#pragma once

#include "CBuffer.h"
#include "CBufferData.h"

//====================================
// 쉐이더에 색상값을 보내는 전용CBuffer
//====================================

class ColorCBuffer : public CBuffer
{
public:
	ColorCBuffer();
	virtual ~ColorCBuffer();

private:
	FColorCBufferData _data;

public:
	virtual void Update();
	void SetColor(float r, float g, float b, float a)
	{
		_data._color._x = r;
		_data._color._y = g;
		_data._color._z = b;
		_data._color._w = a;
	}


};


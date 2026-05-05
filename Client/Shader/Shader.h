#pragma once
#include "../Core/Object.h"

class Shader
{
public:
	Shader() = default;
	virtual ~Shader() = default;
public:
	virtual bool Init() = 0;
	virtual bool SetShader() = 0;

};


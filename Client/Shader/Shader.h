#pragma once
#include "../Core/Object.h"


//===================================
// 실제로 메쉬를 화면에 그리는 컴포넌트 
// 메쉬+쉐이더+재질을 조합해서 렌더링
//===================================


class Shader : public Object
{
public:
	Shader() = default;
	virtual ~Shader() = default;
public:
	virtual bool Init() = 0;
	virtual void SetShader() = 0;

};


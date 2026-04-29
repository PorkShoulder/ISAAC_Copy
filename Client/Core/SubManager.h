#pragma once
#include "Object.h"


//====================================
// 여러 매니저 클래스들의 공통 부모 역할
//====================================


class SubManager : public Object
{
public:
	SubManager();
	virtual ~SubManager();
public:
	virtual bool Init() { return false; } // 자식 클래스에 재정의 하도록 역할을 넘김.


};


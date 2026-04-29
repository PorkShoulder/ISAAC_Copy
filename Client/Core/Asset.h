#pragma once

#include "Object.h"

//===========================
// 객체들의 이름 관리기능 제공
//===========================


class Asset : public Object
{
public:
	Asset();
	virtual ~Asset();
protected:
	std::string _name;
public:
	void SetName(const std::string& name) { _name = name; }
	const std::string GetName() const { return _name; }

};


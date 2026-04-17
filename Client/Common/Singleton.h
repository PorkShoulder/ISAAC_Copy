#pragma once
#include "Info.h"



#include "../Core/Defines.h"




template<typename T>
class Singleton
{
protected:
	Singleton() = default;
	~Singleton() = default; 
	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton operator=(const Singleton&) = delete;
	Singleton operator=(Singleton&&) = delete;
public:
	static T& Instance() 
	{
		static T Inst;
		return Inst;
	}
public:
	// 가상함수인 이유 : 싱글톤을 상속받을 자식에서 내부 동작이 달라질 것이기 때문에.
	// = 0 : 순수가상함수라는 표시
	virtual void Destroy() = 0;

};


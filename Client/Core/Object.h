#pragma once
#include "Common/Info.h"
#include "Defines.h"
#include <memory>

using namespace std;

//========================
// 모든 객체의 최상위 관리자
//========================


class Object : public enable_shared_from_this<Object>		// 이걸 상속 받을 시 자기자신을 안전하게 shared_ptr 가능
{
public:
	Object() {}
	~Object() {}
protected:
	bool m_bEnable = false;		// 살아는 있지만 활성화 여부
	bool m_bActive = false;		// 살았는지 죽었는지 확인

public:
	bool IsActive() const { return m_bActive; }
	bool IsEnable() const { return m_bEnable; }
	void SetActive(bool _active) { m_bActive = _active; }
	void SetEnable(bool _enable) { m_bEnable = _enable; }
	
	virtual void Destroy() = 0;

	//자기 자신의 스마트 포인터를 원하는 타입으로 캐스팅해서 돌려준느 함수 
	template<typename T>
	Ptr<T> This()
	{
		return Cast<Object, T>(shared_from_this());
	}
};


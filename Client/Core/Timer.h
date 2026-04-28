#pragma once
#include "Object.h"
#include <functional>


//==========================================
// 지정된 시간 경과 후 시간 기반 이벤트 처리기
//==========================================

class Timer : public Object
{
public:
	Timer() = default;
	~Timer() = default;
	Timer(const Timer&) = delete;
	Timer(Timer&&) = delete;
	Timer& operator=(const Timer&) = delete;
	Timer& operator=(Timer&&) = delete;

private:
	float _expireTime	= 0.f;	// 만료 시간
	float _accTime		= 0.f;	// 누적 시간
	bool _run			= false;	 // 타이머 동작여부
	bool _repeat		= false;	 // 반복 여부
	std::function<void()> _callback; //타이머가 지정된 시간에 따라 해야할 일을 바인딩함.

public:
	const bool IsRun() const;
	void Tick(float deltaTime);
	void Stop();
	virtual void Destroy() override;

	template<typename T>
	void SetTimer(float expireTime, bool repeat, T&& func)
	{
		//  람다or일반 함수용
		if (_run)
			return;

		_expireTime = expireTime;		
		_repeat		= repeat;			
		_callback	= std::forward<T>(func);	// 넘어온 함수를 그대로 전달함.
		_accTime	= 0.f;				
		_run		= true;				
	}

	template<typename T>
	void SetTimer(float expireTime, bool repeat, T* obj, void(T::* func)())
	{
		_expireTime = expireTime;
		_repeat		= repeat;
		_callback	= std::bind(func, obj); // 넘어온 함수를 그대로 전달함.
		_accTime	= 0.f;
		_run		= true;
	}

};



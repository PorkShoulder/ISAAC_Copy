#include "pch.h"
#include "Timer.h"

const bool Timer::IsRun() const
{
	return _run;
}

void Timer::Tick(float deltaTime)
{
	// 매프레임 호출 
	
	// 실행중이 아니라면 아무것도 안함.
	if (!_run)
		return;

	// 시간을 누적함.
	_accTime += deltaTime;
	if (_accTime < _expireTime)
		return;

	_accTime -= _expireTime;
	if (_callback)
		_callback();

	if (!_repeat)
		_run = false;
	

}

void Timer::Stop()
{
	_run = false;
	_accTime = 0.f;


}

void Timer::Destroy()
{
}

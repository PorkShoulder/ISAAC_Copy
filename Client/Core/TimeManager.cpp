#include "pch.h"
#include "TimeManager.h"

bool TimeManager::Init()
{
	// 현재 시간으로 초기화
	_prevTime = std::chrono::high_resolution_clock::now();
	return true;

}

float TimeManager::Tick()
{
	auto curTime = std::chrono::high_resolution_clock::now();		//
	std::chrono::duration<float> frameTime = curTime - _prevTime;	//
	_deltaTime = frameTime.count();
	_prevTime = curTime;

	// 삭제 예정 타이머 처리
	for (int32 id : _removeTimers)
	{
		auto it = _timers.find(id);
		if (_timers.end() == it)
			continue;

		DESTROY(it->second);
		_timers.erase(id);
	}
	_removeTimers.clear();
	
	//
	for (auto& it : _timers) 
	{
		if (nullptr == it.second)
		{
			RemoveTimer(it.first);
			continue;
		}
		it.second->Tick(_deltaTime);
		if (!it.second->IsRun())
			RemoveTimer(it.first);
	}
	return _deltaTime;
}

float TimeManager::GetFPS()
{
	// 1 / _deltaTime 초당 프레임 수 계산
	if (_deltaTime <= 0.f)
		return 0.f;

	return 1.f / _deltaTime;
}

void TimeManager::RemoveTimer(int32 id)
{
	// 특정 타이머를 삭제 목록에 추가
	_removeTimers.push_back(id);
}

void TimeManager::Destroy()
{
	for (auto& it : _timers)
		DESTROY(it.second);

	_timers.clear();
	_removeTimers.clear();
}

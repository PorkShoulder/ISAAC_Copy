#pragma once
#include "../Common/Singleton.h"
#include "Timer.h"

#include <chrono> // C++ 에서 사용하는 시간 관련 헤더
#include <map>


class TimeManager :public Singleton<TimeManager>
{
	SINGLETON(TimeManager)
private:
	// 특정 시간을 나타내는 타임스탬프
	std::chrono::high_resolution_clock::time_point _prevTime;  // 이전 프레임의 시간
	float _deltaTime = 0.f;		
	
	std::map<int32, Ptr<class Timer>> _timers;	// 생성된 타이머들을 ID로 관리하는 맵
	std::vector<int32> _removeTimers;			// 삭제할 타이머 ID목록 Tick중에 삭제시 순회가 꼬임 나중에 처리
	int32 _timerID = 0;

public:
	bool Init();
	float Tick();
	float GetDeltaTime() const { return _deltaTime; }
	float GetFPS();
	void RemoveTimer(int32 id);
	virtual void Destroy() override;

    template<typename T>
    int32 SetTimer(float expireTime, bool repeat, T&& func)
    {
        int32 id = _timerID++;
        Ptr<Timer> timer = New<Timer>();
        timer->SetTimer(expireTime, repeat, std::forward<T>(func));
        _timers[id] = timer;

        return id;
    }

    template<typename T>
    int32 SetTimer(float expireTime, bool repeat, T* obj, void(T::* func)())
    {
        int32 id = _timerID++;
        Ptr<Timer> timer = New<Timer>();
        timer->SetTimer(expireTime, repeat, obj, func);
        _timers[id] = timer;

        return id;
    }

};


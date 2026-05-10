#pragma once
#include "Object.h"

#include "Animation2DData.h"

#include <functional>
#include <map>

//애니메이션 노티파이
//애니메이션 프레임별로 바인딩하고 싶은 함수가 있을 경우
//바인딩 할 수 있게 해주는 역할
struct FAnimation2DNofity
{
    int32 _frame;
    std::vector<std::function<void()>> _funcs;
};


//애니메이션데이터를 진짜 재생할수 있게 해주는 클래스 ---> 
//IDLE 애니메이션 관리, 
//ATTACK 애니메이션 관리..
class Animation2DSequence : public Object
{
    friend class Animation2D;

public:
    Animation2DSequence();
    virtual ~Animation2DSequence();

private:
    Weak<class Animation2D> _owner;    //내가 어떤 애니메이션2D클래스에 속해있는지
    Ptr<class Animation2DData> _data;  //진짜 애니메이션데이터
    int32 _frame = 0;                   //현재 몇번째 프레임인지? 
    float _time = 0;                    //누적시간
    float _frameTime = 0;               //프레임별 시간
    float _playTime = 0;                //애니메이션 총 플레이 시간
    float _playRate = 0;                //플레이 속도 배율 (x1.0, x2.0)
    bool _loop = false;                         //반복 여부
    bool _reverse = false;                      //재생 방향
    bool _play = true;

    std::map<int32, FAnimation2DNofity> _notifyes;

public:
    Ptr<class Animation2DData> GetAnimation2DData() const;
    int32 GetFrame() const { return _frame; }
    const float GetPlayTime() const { return _playTime; }
    const std::string& GetName() const;

public:
    void SetData(Ptr<class Animation2DData> data);
    void SetPlayTime(float playTime);
    void SetPlayRate(float rateTime);
    void SetLoop(bool loop);
    void SetReverse(bool reverse);
    void SetPlay(bool play);

    virtual void DrawInspector() override;

public:
    //게임 틱을 돌면서
    //현재 애니메이션의 프레임을 계산해주는 함수
    void Tick(float deltaTime);

    virtual void Destroy() override;

private:
    //노티파이를 호출해주는 함수
    void InvokeNotify();

    template<typename T>
    void AddNotify(int32 frame, T* obj, void (T::* memFunc)())
    {
        //todo : frame 처리
        int32 count = _data->GetFrameCount();
        if (frame >= count || frame < 0)
            return;

        auto it = _notifyes.find(frame);
        if (_notifyes.end() != it)
        {
            it->second._funcs.push_back(std::bind(memFunc, obj));
            return;
        }

        FAnimation2DNofity notify;
        notify._frame = frame;
        notify._funcs.push_back(std::bind(memFunc, obj));

        _notifyes[frame] = notify;
    }

    template<typename T>
    void AddNotify(int32 frame, T&& func)
    {
        auto it = _notifyes.find(frame);
        if (_notifyes.end() != it)
        {
            it->second._funcs.push_back(std::forward<T>(func));
            return;
        }

        FAnimation2DNofity notify;
        notify._frame = frame;
        notify._funcs.push_back(std::forward<T>(func));

        _notifyes[frame] = notify;
    }

};


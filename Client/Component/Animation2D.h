#pragma once
#include "../Core/Object.h"

#include "../Core/Animation2DSequence.h"
#include <unordered_map>

//라이언
//라이언 Move, 라이언Idle, 라이언 Sleep...
//여러 종류의 애니메이션 시퀀스를 관리할수 있는 클래스
class Animation2D : public Object
{
    friend class SpriteComponent;

public:
    Animation2D();
    virtual ~Animation2D();

private:
    Ptr<class AnimCBuffer> _animBuffer;
    Weak<class SpriteComponent> _owner;
    Ptr<class Animation2DSequence> _currentSequence;                                //현재 재생중인 시퀸스
    std::unordered_map<std::string, Ptr<class Animation2DSequence>> _sequences;     //가지고 있는 모든 시퀸스
    bool _filp = false; //애니메이션 반전

public:
    virtual void DrawInspector() override;

    void SetAnimFilp(bool filp);

public:
    bool Init();
    void Tick(float deltaTime);

    //시퀀스를 추가하는 함수
    bool AddSequeunce(const std::string& name, bool loop = false, bool reverse = false, float playTime = 1.f, float playRate = 1.f);

    //시퀀스를 추가하는 함수
    bool AddSequeunce(Ptr<class Animation2DData> data, bool loop = false, bool reverse = false, float playTime = 1.f, float playRate = 1.f);

    //이름을 통해 해당 시퀀스의 재생시간을 조절하는 함수
    void SetPlayTime(const std::string& name, float playTime);

    //이름을 통해 해당 시퀀스의 재생속도를 조절하는 함수
    void SetPlayRate(const std::string& name, float playRate);

    //이름을 통해 해당 시퀀스의 반복여부를 지정하는 함수
    void SetLoop(const std::string& name, bool loop);

    void SetReverse(const std::string& name, bool reverse);

    void SetPlay(const std::string& name, bool play);

    //애니메이션 시퀀스 간의 전환이 필요할때 사용하는 함수
    void ChangeAnimation(const std::string& name);

    void SetShader();

    virtual void Destroy() override;

private:
    Ptr<Animation2DSequence> FindSequence(const std::string& name);

public:
    template<typename T>
    void AddNotify(const std::string& name, int32 frame, T* obj, void (T::* memFunc)())
    {
        Ptr<Animation2DSequence> seq = FindSequence(name);
        if (nullptr == seq)
            return;

        seq->AddNotify(frame, obj, memFunc);
    }

    template<typename T>
    void AddNotify(const std::string& name, int32 frame, T&& func)
    {
        Ptr<Animation2DSequence> seq = FindSequence(name);
        if (nullptr == seq)
            return;

        seq->AddNotify(frame, std::forward<T>(func));
    }




};


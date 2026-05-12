#pragma once
#include "Asset.h"
#include "FMOD/fmod.hpp"

class Sound : public Asset
{
public:
    Sound();
    virtual ~Sound();
    
protected:
    //fmod 시스템 객체
    FMOD::System* _system = nullptr;

    //사운드가 어떤  채널에서 재생되는지 관리하는 객체
    //bgm, effect, ui 등등 사운드 그룹으로 나누어서 관리가 가능
    FMOD::ChannelGroup* _group = nullptr;
    FMOD::Channel* _channel = nullptr;
 
    //실제 사운드 데이터 객체
    FMOD::Sound* _sound = nullptr;
    
    bool _loop = false;
    int32 _playCount = 0;

public:
    bool LoadSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop, const char* fileName);

    void Play();
    void Stop();
    void Pause();
    void Resume();

public:
    //사운드가 재생되는 채널에서 발생하는 이벤트를 처리하는 콜백함수
    static FMOD_RESULT SoundCallBack(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);

    virtual void Destroy() override;
};


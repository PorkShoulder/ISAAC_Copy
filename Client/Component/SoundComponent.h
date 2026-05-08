#pragma once
#include "SceneComponent.h"

class SoundComponent : public SceneComponent
{
public:
    SoundComponent();
    virtual ~SoundComponent();

protected:
    Ptr<class Sound> _sound;

public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;

    virtual void Destroy() override;

    void SetSound(const std::string& name, bool play);
    void SetSound(const std::string& name, const std::string& groupName, const char* fileName, bool loop, bool play);

    void Play();
    void Stop();
    void Pause();
    void Resume();
};


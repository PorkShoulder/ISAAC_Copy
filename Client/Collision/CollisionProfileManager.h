#pragma once
#include "Common/Singleton.h"
#include "Common/Info.h"

#include "CollisionProfile.h"

#include <map>
#include <unordered_map>

//채널, 프로파일 이라는것을 통해
//충돌을 함에 있어서, 각 충돌을 어떤 반응을 할것인지 지정한다.
class CollisionProfileManager : public Singleton<CollisionProfileManager>
{
    SINGLETON(CollisionProfileManager)

private:
    //각 채널들이 기본적으로 어떤 반응을 할것인지
    std::map<eCollisionChannel, eCollisionResponse> _channels;
    
    //이름으로 프로파일 관리
    std::unordered_map<std::string, Ptr<CollisionProfile>> _profiles;

public:
    bool Init();
    Ptr<CollisionProfile> FindProfile(const std::string& name);
    void CreateChannel(eCollisionChannel channel, eCollisionResponse response);
    void CreateProfile(const std::string& name, eCollisionChannel channel);
    void SetProfileResponse(const std::string& proflieName, eCollisionChannel channel, eCollisionResponse response);

    template<typename... Args>
    void SetProfileResponse(const std::string& name, std::pair<eCollisionChannel, eCollisionResponse> response, Args... args)
    {
        Ptr<CollisionProfile> profile = FindProfile(name);
        if (nullptr == profile)
            return;

        profile->SetProfileResponse(response, args...);
    }

    virtual void Destroy() override;


};


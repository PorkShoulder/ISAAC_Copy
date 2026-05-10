#pragma once
#include "../Core/Object.h"
#include "../Common/Info.h"

class CollisionProfile :
    public Object
{
    friend class CollisionProfileManager;

public:
    CollisionProfile();
    virtual ~CollisionProfile();

private:
    //프로파일 이름
    std::string _name;

    //기본으로 사용할 채널
    eCollisionChannel _channel = eCollisionChannel::COLLISION_CHANNEL_END;

    //해당 프로파일에서 사용하는 채널이 다른 채널들과
    //어떤 충돌을 반응을 할 것인지
    eCollisionResponse _response[eCollisionChannel::COLLISION_CHANNEL_END] = {};

public:
    const eCollisionChannel GetChannel() const;
    const std::string& GetName() const;
    const eCollisionResponse GetResponse(eCollisionChannel channel);

public:
    virtual void Destroy() override;

public:
    //채널이랑 반응을 페어로 넣어주면
    //해당 프로파일의 반응목록에 매개변수로 들어온 채널의 반응을 매개변수로 들어온 반응으로
    //지정해준다.
    //ex) 프로파일이 player
    //SetProfileResponse({몬스터, 블록})
    void SetProfileResponse(std::pair<eCollisionChannel, eCollisionResponse> response)
    {
        _response[response.first] = response.second;
    }

    template<typename... Args>
    void SetProfileResponse(std::pair<eCollisionChannel, eCollisionResponse> response, Args ...args)
    {
        //C++ 17
        //(SetProfileResponse(response), args...);

        //옛날 버전
        SetProfileResponse(response);
        SetProfileResponse(args...);
    }

    void SetProfileResponse()
    {
        //nothing
    }

};


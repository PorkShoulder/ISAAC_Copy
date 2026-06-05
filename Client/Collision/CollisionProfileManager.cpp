#include "pch.h"
#include "CollisionProfileManager.h"

bool CollisionProfileManager::Init()
{
    // 채널 등록
    CreateChannel(COLLISION_CHANNEL_PLAYER, COLLISION_RESPONSE_IGNORE);
    CreateChannel(COLLISION_CHANNEL_PLAYER_BULLET, COLLISION_RESPONSE_IGNORE);
    CreateChannel(COLLISION_CHANNEL_MONSTER, COLLISION_RESPONSE_IGNORE);
    CreateChannel(COLLISION_CHANNEL_MONSTER_BULLET, COLLISION_RESPONSE_IGNORE);
    CreateChannel(COLLISION_CHANNEL_ITEM, COLLISION_RESPONSE_IGNORE);
    CreateChannel(COLLISION_CHANNEL_DOOR, COLLISION_RESPONSE_IGNORE);
    CreateChannel(COLLISION_CHANNEL_BOSS, COLLISION_RESPONSE_IGNORE);
    CreateChannel(COLLISION_CHANNEL_NPC, COLLISION_RESPONSE_IGNORE);

    //프로파일 생성 : 이름은 Player, 사용 채널은 PLAYER 채널
    CreateProfile("Player", COLLISION_CHANNEL_PLAYER);
    CreateProfile("PlayerBullet", COLLISION_CHANNEL_PLAYER_BULLET);
    CreateProfile("Monster", COLLISION_CHANNEL_MONSTER);
    CreateProfile("MonsterBullet", COLLISION_CHANNEL_MONSTER_BULLET);
    CreateProfile("Item", COLLISION_CHANNEL_ITEM);
    CreateProfile("Door", COLLISION_CHANNEL_DOOR);
    CreateProfile("Boss", COLLISION_CHANNEL_BOSS);
    CreateProfile("Npc", COLLISION_CHANNEL_NPC);

    // 플레이어 상호작용
    SetProfileResponse("Player",
        std::make_pair(COLLISION_CHANNEL_MONSTER, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_MONSTER_BULLET, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_PLAYER_BULLET, COLLISION_RESPONSE_IGNORE),
        std::make_pair(COLLISION_CHANNEL_ITEM, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_DOOR, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_BOSS, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_NPC, COLLISION_RESPONSE_OVERLAP));

    // PlayerBullet 반응
    SetProfileResponse("PlayerBullet",
        std::make_pair(COLLISION_CHANNEL_MONSTER, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_BOSS, COLLISION_RESPONSE_OVERLAP));

    // Monster 반응
    SetProfileResponse("Monster",
        std::make_pair(COLLISION_CHANNEL_PLAYER, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_PLAYER_BULLET, COLLISION_RESPONSE_OVERLAP));

    // MonsterBullet 반응
    SetProfileResponse("MonsterBullet",
        std::make_pair(COLLISION_CHANNEL_PLAYER, COLLISION_RESPONSE_OVERLAP));

    // Item 반응
    SetProfileResponse("Item",
        std::make_pair(COLLISION_CHANNEL_PLAYER, COLLISION_RESPONSE_OVERLAP));

    // Door 반응
    SetProfileResponse("Door",
        std::make_pair(COLLISION_CHANNEL_PLAYER, COLLISION_RESPONSE_OVERLAP));

    // Boss 반응
    SetProfileResponse("Boss",
        std::make_pair(COLLISION_CHANNEL_PLAYER, COLLISION_RESPONSE_OVERLAP),
        std::make_pair(COLLISION_CHANNEL_PLAYER_BULLET, COLLISION_RESPONSE_OVERLAP));

    // Npc 반응
    SetProfileResponse("Npc",
        std::make_pair(COLLISION_CHANNEL_PLAYER, COLLISION_RESPONSE_OVERLAP));

    return true;
}

Ptr<class CollisionProfile> CollisionProfileManager::FindProfile(const std::string& name)
{
    auto it = _profiles.find(name);
    if (_profiles.end() == it)
        return nullptr;

    return it->second;
}

void CollisionProfileManager::CreateChannel(eCollisionChannel channel, eCollisionResponse response)
{
    if (_channels.end() != _channels.find(channel))
        return;

    _channels[channel] = response;
}

void CollisionProfileManager::CreateProfile(const std::string& name, eCollisionChannel channel)
{
    if (_channels.end() == _channels.find(channel))
        return;

    if (_profiles.end() != _profiles.find(name))
        return;

    Ptr<CollisionProfile> profile = New<CollisionProfile>();
    profile->_name = name;
    profile->_channel = channel;

    //프로파일을 새로 만들게 되면
    //채널 목록을 확인하여 다른 채널들과의 기본 반응을 넣어준다.
    for (auto it : _channels)
        profile->_response[it.first] = it.second;

    _profiles[profile->_name] = profile;
}

void CollisionProfileManager::SetProfileResponse(const std::string& proflieName, eCollisionChannel channel, eCollisionResponse response)
{
    Ptr<CollisionProfile> profile = FindProfile(proflieName);
    if (nullptr == profile)
        return;

    profile->_response[channel] = response;
}

void CollisionProfileManager::Destroy()
{
    for (auto& it : _profiles)
        DESTROY(it.second);

    _profiles.clear();
    _channels.clear();
}

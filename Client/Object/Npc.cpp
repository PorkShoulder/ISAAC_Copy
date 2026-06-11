#include "pch.h"
#include "Npc.h"
#include "Player.h"
#include "Item.h"
#include "../World/Level.h"

#include "../Component/SpriteComponent.h"
#include "../Component/AABBCollisionComponent.h"

Npc::Npc()
{
}

Npc::~Npc()
{
}

bool Npc::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);
    _type = eActorType::Npc;

    _npcMesh = CreateSceneComponent<SpriteComponent>("Mesh");
    SetRootComponent(_npcMesh);

    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(32.f, 32.f);
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Npc");
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Npc::OnOverlap);


    return true;
}

void Npc::Tick(float deltaTime)
{
}

void Npc::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void Npc::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Npc::Destroy()
{
    Actor::Destroy();
}

void Npc::Save(std::ofstream& file)
{
    Actor::Save(file);

    // NPC 본체
    size_t len = _data.textureName.size();
    file.write((char*)&len, sizeof(len));
    file.write(_data.textureName.c_str(), len);

    len = _data.texturePath.size();
    file.write((char*)&len, sizeof(len));
    file.write((char*)_data.texturePath.c_str(), len * sizeof(wchar_t));

    file.write((char*)&_data.npcType, sizeof(_data.npcType));
    file.write((char*)&_data.renderSize, sizeof(_data.renderSize));
    file.write((char*)&_data.collisionSize, sizeof(_data.collisionSize));

    size_t frameCount = _data.idleFrames.size();
    file.write((char*)&frameCount, sizeof(frameCount));
    for (auto& f : _data.idleFrames)
        file.write((char*)&f, sizeof(f));

    file.write((char*)&_data.cost, sizeof(_data.cost));
    file.write((char*)&_data.rewardConsumable, sizeof(_data.rewardConsumable));

    // rewardItem
    size_t itemNameLen = _data.rewardItem.textureName.size();
    file.write((char*)&itemNameLen, sizeof(itemNameLen));
    file.write(_data.rewardItem.textureName.c_str(), itemNameLen);

    size_t itemPathLen = _data.rewardItem.texturePath.size();
    file.write((char*)&itemPathLen, sizeof(itemPathLen));
    file.write((char*)_data.rewardItem.texturePath.c_str(), itemPathLen * sizeof(wchar_t));

    file.write((char*)&_data.rewardItem.itemType, sizeof(_data.rewardItem.itemType));
    file.write((char*)&_data.rewardItem.renderSize, sizeof(_data.rewardItem.renderSize));
    file.write((char*)&_data.rewardItem.collisionSize, sizeof(_data.rewardItem.collisionSize));
    file.write((char*)&_data.rewardItem.bonusDamage, sizeof(float));
    file.write((char*)&_data.rewardItem.bonusSpeed, sizeof(float));
    file.write((char*)&_data.rewardItem.bonusHp, sizeof(int32));
    file.write((char*)&_data.rewardItem.effect, sizeof(uint32));
    file.write((char*)&_data.rewardItem.effect, sizeof(uint32));
    file.write((char*)&_data.rewardItem.consumableAmount, sizeof(int32));

    auto saveFrames = [&](const std::vector<FVector4D>& frames) {
        size_t count = frames.size();
        file.write((char*)&count, sizeof(count));
        for (auto& f : frames)
            file.write((char*)&f, sizeof(f));
        };
    saveFrames(_data.rewardItem.itemFrames);
    saveFrames(_data.rewardItem.equipHeadFrames);
    saveFrames(_data.rewardItem.equipBodyFrames);
    saveFrames(_data.rewardItem.bulletFrames);

    file.write((char*)&_hasInteracted, sizeof(_hasInteracted));
}

void Npc::Load(std::ifstream& file)
{
    Actor::Load(file);

    // NPC 본체
    size_t len = 0;
    file.read((char*)&len, sizeof(len));
    _data.textureName.resize(len);
    file.read(&_data.textureName[0], len);

    file.read((char*)&len, sizeof(len));
    _data.texturePath.resize(len);
    file.read((char*)&_data.texturePath[0], len * sizeof(wchar_t));

    file.read((char*)&_data.npcType, sizeof(_data.npcType));
    file.read((char*)&_data.renderSize, sizeof(_data.renderSize));
    file.read((char*)&_data.collisionSize, sizeof(_data.collisionSize));

    size_t frameCount = 0;
    file.read((char*)&frameCount, sizeof(frameCount));
    _data.idleFrames.resize(frameCount);
    for (size_t i = 0; i < frameCount; i++)
        file.read((char*)&_data.idleFrames[i], sizeof(FVector4D));

    file.read((char*)&_data.cost, sizeof(_data.cost));
    file.read((char*)&_data.rewardConsumable, sizeof(_data.rewardConsumable));

    // rewardItem
    size_t itemNameLen = 0;
    file.read((char*)&itemNameLen, sizeof(itemNameLen));
    _data.rewardItem.textureName.resize(itemNameLen);
    file.read(&_data.rewardItem.textureName[0], itemNameLen);

    size_t itemPathLen = 0;
    file.read((char*)&itemPathLen, sizeof(itemPathLen));
    _data.rewardItem.texturePath.resize(itemPathLen);
    file.read((char*)&_data.rewardItem.texturePath[0], itemPathLen * sizeof(wchar_t));

    file.read((char*)&_data.rewardItem.itemType, sizeof(_data.rewardItem.itemType));
    file.read((char*)&_data.rewardItem.renderSize, sizeof(_data.rewardItem.renderSize));
    file.read((char*)&_data.rewardItem.collisionSize, sizeof(_data.rewardItem.collisionSize));
    file.read((char*)&_data.rewardItem.bonusDamage, sizeof(float));
    file.read((char*)&_data.rewardItem.bonusSpeed, sizeof(float));
    file.read((char*)&_data.rewardItem.bonusHp, sizeof(int32));
    file.read((char*)&_data.rewardItem.effect, sizeof(uint32));
    file.read((char*)&_data.rewardItem.effect, sizeof(uint32));
    file.read((char*)&_data.rewardItem.consumableAmount, sizeof(int32));

    auto loadFrames = [&](std::vector<FVector4D>& frames) {
        size_t count = 0;
        file.read((char*)&count, sizeof(count));
        frames.resize(count);
        for (size_t i = 0; i < count; i++)
            file.read((char*)&frames[i], sizeof(FVector4D));
        };
    loadFrames(_data.rewardItem.itemFrames);
    loadFrames(_data.rewardItem.equipHeadFrames);
    loadFrames(_data.rewardItem.equipBodyFrames);
    loadFrames(_data.rewardItem.bulletFrames);

    file.read((char*)&_hasInteracted, sizeof(_hasInteracted));

    SetNpcData(_data);
}

void Npc::SetNpcData(const FNpcData& data)
{
    _data = data;
    _col->SetBoxSize(data.collisionSize._x, data.collisionSize._y);

    if (_npcMesh && !data.texturePath.empty())
    {
        std::string animName = GetName() + "_IDLE";

        if (!data.idleFrames.empty())
            _npcMesh->AddAnimSequence(animName, data.texturePath, data.idleFrames, true);

        if (!data.idleFrames.empty())
            _npcMesh->ChangeAnimation(animName);
    }
}

void Npc::OnOverlap(Weak<class CollisionComponent> dest)
{
    if (_hasInteracted) return;

    Ptr<Actor> owner = dest.lock()->GetOwner();
    Ptr<Player> player = Cast<Actor, Player>(owner);
    if (!player) return;

    // 비용 체크
    if (player->GetCoin() < _data.cost.coin) return;
    if (player->GetKey() < _data.cost.key)  return;
    if (player->GetBomb() < _data.cost.bomb) return;

    // 비용 차감
    player->AddCoin(-_data.cost.coin);
    player->AddKey(-_data.cost.key);
    player->AddBomb(-_data.cost.bomb);

    // 보상 지급
    if (_data.npcType == eNpcType::CONSUMABLE)
    {
        player->AddCoin(_data.rewardConsumable.coin);
        player->AddKey(_data.rewardConsumable.key);
        player->AddBomb(_data.rewardConsumable.bomb);
    }
    else if (_data.npcType == eNpcType::EFFECT_ITEM)
    {
        Ptr<Level> level = GetLevel();
        if (level)
        {
            float angle = (float)(rand() % 360) * 3.14159f / 180.f;
            float dist = 40.f + (float)(rand() % 30);

            FVector3D spawnPos = GetWorldPosition();
            spawnPos._x += cosf(angle) * dist;
            spawnPos._y += sinf(angle) * dist;

            Ptr<Item> item = level->SpawnActor<Item>("NpcDrop", spawnPos,
                FVector3D(_data.rewardItem.renderSize._x, _data.rewardItem.renderSize._y, 1.f),
                FRotator(0, 0, 0));
            if (item)
                item->SetItemData(_data.rewardItem);
        }
    }

    _hasInteracted = true;
    // NPC 외형 변경이나 Remove() 처리는 취향에 따라
}




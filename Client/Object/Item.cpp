#include "pch.h"
#include "Item.h"
#include "Player.h"


#include "../Component/SpriteComponent.h"
#include "../Component/AABBCollisionComponent.h"

Item::Item()
{
}

Item::~Item()
{
}

bool Item::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
	Actor::Init(id, pos, scale, rot, name);
    _type = eActorType::Item;
    
    _itemMesh = CreateSceneComponent<SpriteComponent>("Mesh");
    SetRootComponent(_itemMesh);

    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(32.f, 32.f);
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Item");
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Item::OnOverlap);

    return true;
}

void Item::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Item::Collision(float deltaTime)
{

}

void Item::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}


void Item::Destroy()
{
    Actor::Destroy();
}

void Item::SetItemData(const FItemData& data)
{
    _data = data;
    if (_col)
        _col->SetBoxSize(data.collisionSize._x, data.collisionSize._y);
    if (_itemMesh && !data.texturePath.empty() && !data.itemFrames.empty())
    {
        std::string animName = GetName() + "_ITEM";
        _itemMesh->AddAnimSequence(animName, data.texturePath, data.itemFrames, true);
        _itemMesh->ChangeAnimation(animName);
        _itemMesh->SetPlay(animName, true);
    }
}

void Item::OnOverlap(Weak<CollisionComponent> dest)
{
    if (_isPickedUp) return;

    Ptr<Actor> owner = dest.lock()->GetOwner();
    Ptr<Player> player = Cast<Actor, Player>(owner);
    if (!player) return;

    // 가격 체크
    if (player->GetCoin() < _data.cost.coin) return;
    if (player->GetKey() < _data.cost.key)  return;
    if (player->GetBomb() < _data.cost.bomb) return;

    // 비용 차감
    player->AddCoin(-_data.cost.coin);
    player->AddKey(-_data.cost.key);
    player->AddBomb(-_data.cost.bomb);

    _isPickedUp = true;

    switch (_data.itemType)
    {
    case eItemType::PASSIVE:
    case eItemType::ACTIVE:
        player->ApplyItemVisual(_data);
        player->ApplyItemStats(_data);
        if (_data.effect != EFFECT_NONE)
            player->AddEffect(_data.effect);
        break;
    case eItemType::COIN:
        player->AddCoin(_data.consumableAmount);
        break;
    case eItemType::KEY:
        player->AddKey(_data.consumableAmount);
        break;
    case eItemType::BOMB:
        player->AddBomb(_data.consumableAmount);
        break;
    case eItemType::HEART:
        player->AddHp(_data.consumableAmount);
        break;
    }

    Remove();
}
void Item::Save(std::ofstream& file)
{
    Actor::Save(file);

    // itemType
    file.write((char*)&_data.itemType, sizeof(eItemType));

    // textureName
    int32 nameLen = (int32)_data.textureName.size();
    file.write((char*)&nameLen, sizeof(int32));
    file.write(_data.textureName.c_str(), nameLen);

    // texturePath
    int32 pathLen = (int32)_data.texturePath.size();
    file.write((char*)&pathLen, sizeof(int32));
    file.write((char*)_data.texturePath.c_str(), pathLen * sizeof(wchar_t));

    // 크기
    file.write((char*)&_data.renderSize, sizeof(FVector2D));
    file.write((char*)&_data.collisionSize, sizeof(FVector2D));

    // isPickedUp
    file.write((char*)&_isPickedUp, sizeof(bool));
    file.write((char*)&_data.cost, sizeof(_data.cost));
    file.write((char*)&_data.consumableAmount, sizeof(int32));

    // 프레임 3종
    auto saveFrames = [&](const std::vector<FVector4D>& frames)
        {
            int32 count = (int32)frames.size();
            file.write((char*)&count, sizeof(int32));
            for (auto& f : frames)
                file.write((char*)&f, sizeof(FVector4D));
        };

    saveFrames(_data.itemFrames);
    saveFrames(_data.equipHeadFrames);
    saveFrames(_data.equipBodyFrames);
}

void Item::Load(std::ifstream& file)
{
    Actor::Load(file);

    FItemData data;

    // itemType
    file.read((char*)&data.itemType, sizeof(eItemType));

    // textureName
    int32 nameLen = 0;
    file.read((char*)&nameLen, sizeof(int32));
    data.textureName.resize(nameLen);
    file.read(&data.textureName[0], nameLen);

    // texturePath
    int32 pathLen = 0;
    file.read((char*)&pathLen, sizeof(int32));
    data.texturePath.resize(pathLen);
    file.read((char*)&data.texturePath[0], pathLen * sizeof(wchar_t));

    // 크기
    file.read((char*)&data.renderSize, sizeof(FVector2D));
    file.read((char*)&data.collisionSize, sizeof(FVector2D));

    // isPickedUp
    file.read((char*)&_isPickedUp, sizeof(bool));
    file.read((char*)&data.cost, sizeof(data.cost));
    file.read((char*)&data.consumableAmount, sizeof(int32));

    // 프레임 3종
    auto loadFrames = [&](std::vector<FVector4D>& frames)
        {
            int32 count = 0;
            file.read((char*)&count, sizeof(int32));
            frames.resize(count);
            for (int32 i = 0; i < count; i++)
                file.read((char*)&frames[i], sizeof(FVector4D));
        };

    loadFrames(data.itemFrames);
    loadFrames(data.equipHeadFrames);
    loadFrames(data.equipBodyFrames);

    SetItemData(data);
}
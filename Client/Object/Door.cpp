#include "pch.h"
#include "Door.h"

#include "Actor.h"
#include "Player.h"
#include "../Editor/EditorEngine.h"

#include "../Component/SceneComponent.h"
#include "../Component/CollisionComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/AABBCollisionComponent.h"

#include "../Render/RenderManager.h"

Door::Door()
{
}

Door::~Door()
{
}

bool Door::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);
    _type = eActorType::Door;

    _doorFrame = CreateSceneComponent<SpriteComponent>("DoorFrame");
    _doorFrame->SetRenderLayer("DoorFrame");
    SetRootComponent(_doorFrame);

    _doorLeft = CreateSceneComponent<SpriteComponent>("DoorLeft");
    _doorLeft->SetRelativeScale(1.f, 1.f, 1.f);  // 추가
    _doorLeft->SetRenderLayer("DoorPanel");
    _doorLeft->AttachToComponent(_doorFrame);

    _doorRight = CreateSceneComponent<SpriteComponent>("DoorRight");
    _doorLeft->SetRenderLayer("DoorPanel");
    _doorRight->SetRelativeScale(1.f, 1.f, 1.f);  // 추가
    _doorRight->AttachToComponent(_doorFrame);

    _doorOpen = CreateSceneComponent<SpriteComponent>("DoorOpen");
    _doorLeft->SetRenderLayer("DoorPanel");
    _doorOpen->SetRelativeScale(1.f, 1.f, 1.f);  // 추가
    _doorOpen->AttachToComponent(_doorFrame);
    _doorOpen->SetEnable(false);

    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(48.f, 48.f);
    _col->AttachToComponent(_doorFrame);
    _col->SetCollisionProfile("Door");
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Door::OnOverlap);
    
    return true;
}

void Door::RegisterPartAnim(Ptr<class SpriteComponent> sprite, const std::string& animName, const FVector4D& rect)
{
    if (!sprite)
        return;
    if (rect._z <= 0 || rect._w <= 0)
        return;
    std::vector<FVector4D> frames = { rect };
    sprite->AddAnimSequence(animName, _doorData.texturePath, frames, false);
    sprite->ChangeAnimation(animName);
    sprite->SetPlay(animName, false);
}

void Door::SetDoorData(const FDoorSpawnData& data)
{
    _doorData = data;
    if (_col)
        _col->SetBoxSize(data.collisionSize._x, data.collisionSize._y);
    
    std::string base = GetName();
    RegisterPartAnim(_doorFrame, base + "_FRAME", data.frame);
    RegisterPartAnim(_doorLeft, base + "_LEFT", data.left);
    RegisterPartAnim(_doorRight, base + "_RIGHT", data.right);
    RegisterPartAnim(_doorOpen, base + "_OPEN", data.openImage);

    SetOpen(data.bOpen);
}

void Door::SetOpen(bool bOpen)
{
    _doorData.bOpen = bOpen;
    UpdateVisibility();
}



void Door::UpdateVisibility()
{
    if(_doorData.bOpen)
    {
        if (_doorLeft)
            _doorLeft->SetEnable(false);
        if (_doorRight)
            _doorRight->SetEnable(false);
        if (_doorOpen)
            _doorOpen->SetEnable(true);
    }
    else
    {
        if (_doorLeft)
            _doorLeft->SetEnable(true);
        if (_doorRight)
            _doorRight->SetEnable(true);
        if (_doorOpen)
            _doorOpen->SetEnable(false);
    }
}
void Door::TryOpen(Ptr<class Player> player)
{
    if (_doorData.bOpen)
        return;
    switch (_doorData.doorType)
    {case eDoorType::NORMAL:
        SetOpen(true);
        break;
    case eDoorType::KEY:
        // 열쇠 소모 --_key 
        break;
    case eDoorType::COIN:
        // 코인 소모 -- coin
        break;

    }
}

void Door::OnOverlap(Weak<class CollisionComponent> dest)
{
    Ptr<CollisionComponent> col = Lock<CollisionComponent>(dest);
    if (!col)
        return;
    Ptr<Actor> owner = col->GetOwner();
    if (!owner)
        return;
    Ptr<Player> player = Cast<Actor, Player>(owner);
    if (player)
        TryOpen(player);
}

void Door::doorOpen(Ptr<Player> player)
{
    if (_doorData.bOpen)
        return;
    switch (_doorData.doorType)
    {
    case eDoorType::NORMAL:
        SetOpen(true);
        break;
    case eDoorType::KEY:
        if (player->GetKey() > 0)
        {
            player->AddKey(-1);
            SetOpen(true);
        }
        break;
    case eDoorType::COIN:
        if (player->GetCoin() > 0)
        {
            player->AddCoin(-1);
            SetOpen(true);
        }
        break;
    }
}




void Door::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Door::Collision(float deltaTime)
{
    Actor::Collision(deltaTime);
}

void Door::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Door::Destroy()
{
    Actor::Destroy();
}


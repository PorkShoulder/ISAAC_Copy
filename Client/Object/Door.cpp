#include "pch.h"
#include "Door.h"

#include "Actor.h"
#include "Player.h"

#include "../Component/CollisionComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/AABBCollisionComponent.h"


Door::Door()
{
}

Door::~Door()
{
}

bool Door::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    _mesh = CreateSceneComponent<SpriteComponent>("Mesh");
    SetRootComponent(_mesh);

    _frame = CreateSceneComponent<SpriteComponent>("DoorFrame");
    SetRootComponent(_frame);

    _frame->AddAnimSequence("Door_Frame", true);
    _frame->ChangeAnimation("Door_Frame");
    

    _panel = CreateSceneComponent<SpriteComponent>("DoorPanel");
    _panel->AttachToComponent(_root);
    
    _panel->AddAnimSequence("Door_Open", true);
    _panel->AddAnimSequence("Door_LockedClosed", true);
    _panel->AddAnimSequence("Door_CombatClosed", true);
    _panel->AddAnimSequence("Door_Opening", true);
    _panel->ChangeAnimation("Door_Open");
    
    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(52.f, 52.f);
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Door");
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Door::OverlapCallback);

    

    _type = eActorType::Door;

    return true;
}

void Door::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Door::Collision(float deltaTime)
{
}

void Door::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Door::Destroy()
{
    Actor::Destroy();
}

void Door::SetTexture(const std::string& name)
{
    
}

void Door::SetDoorType(eDoorType type)
{
    _doorType = type;

    switch (_doorType)
    {
    case eDoorType::LOCKED:
        ChangeDoorState(eDoorState::LOCKED_CLOSED);
        break;
    default:
        ChangeDoorState(eDoorState::OPEN);
        break;
    }

}

void Door::OverlapCallback(Weak<class CollisionComponent> dest)
{
    Ptr<CollisionComponent> destCol = Lock<CollisionComponent>(dest);
    if (!destCol)
        return;

    Ptr<Actor> actor = destCol->GetOwner();
    Ptr<Player> player = Cast<Actor, Player>(actor);
    if (!player)
        return;

    // 문 타입별 처리
    switch (_doorType)
    {
    case eDoorType::NORMAL:
        // 이동 가능
        break;
    case eDoorType::LOCKED:
        // 열쇠 확인 후 이동
        break;
    case eDoorType::HIDDEN:
        // 폭탄/해금 여부 확인 후 이동
        break;
    case eDoorType::BOSS:
        // 보스방 이동
        break;
    }
}

void Door::ChangeDoorState(eDoorState state)
{
    if (_doorState == state)
        return;

    _doorState = state;

    switch (_doorState)
    {
    case eDoorState::OPEN:
        _panel->ChangeAnimation("Door_Open");
        break;

    case eDoorState::NORMAL_CLOSED:
        _panel->ChangeAnimation("Door_CombatClosed");
        break;

    case eDoorState::LOCKED_CLOSED:
        _panel->ChangeAnimation("Door_LockedClosed");
        break;

    case eDoorState::OPENING:
        _panel->ChangeAnimation("Door_Opening");
        _panel->SetPlay("Door_Opening", true);
        break;
    }
}

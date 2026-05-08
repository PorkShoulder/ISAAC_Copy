#include "pch.h"
#include "Player.h"
#include "../Controller/PlayerController.h"

#include "../World/Level.h"

#include "../Component/MovementComponent.h"
#include "../Component/MeshComponent.h"
#include "../Component/CameraComponent.h"
//#include "../Component/AABBCollisionComponent.h"
//#include "../Component/SphereCollisionComponent.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/SpriteComponent.h"
#include "../Component/SoundComponent.h"
#include "../Component/WidgetComponent.h"

bool Player::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    Pawn::Init(id, pos, scale, rot);

    // 컨트롤러 생성
    _controller = GetLevel()->SpawnActor<PlayerController>(pos, scale, rot);

    // 컨트롤러가 조종하는 대상을 지정
    _controller->SetPawn(This<Player>());

    Ptr<SpriteComponent> msehComp = CreateSceneComponent<SpriteComponent>("Anim");












    return false;
}

void Player::Tick(float deltaTime)
{
    
}

void Player::Collision(float deltaTime)
{
}

void Player::Render(float deltaTime)
{
}

void Player::Destroy()
{
}

void Player::MoveRight(float deltaTime)
{
}

void Player::MoveLeft(float deltaTime)
{
}

void Player::MoveUp(float deltaTime)
{
}

void Player::MoveDown(float deltaTime)
{
}

void Player::MoveStop(float deltaTime)
{
}

void Player::OnDance(float deltaTime)
{
}

void Player::mouseDown(float deltaTime)
{
}

void Player::onDanceNotifyCallBack()
{
}

void Player::BlockCallBack(Weak<class CollisionComponent> dest)
{
}

void Player::OverlapCallBack(Weak<class CollisionComponent> dest)
{
}

void Player::ReleaseCallBack(Weak<class CollisionComponent> dest)
{
}

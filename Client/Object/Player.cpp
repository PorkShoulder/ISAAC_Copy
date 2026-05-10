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

#include "Common/LogManager.h"

#include "../Core/TimeManager.h"
#include "../Core/GameEngine.h"





bool Player::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    Pawn::Init(id, pos, scale, rot);

    // 컨트롤러 생성
    _controller = GetLevel()->SpawnActor<PlayerController>(pos, scale, rot);

    // 컨트롤러가 조종하는 대상을 지정
    _controller->SetPawn(This<Player>());
        
    // 아이작 Head / Body
    Ptr<SpriteComponent> head = CreateSceneComponent<SpriteComponent>("Head");
    Ptr<SpriteComponent> body = CreateSceneComponent<SpriteComponent>("Body");

    // 머리는 몸통을 따라서 이동
    SetRootComponent(body);

    head->AttachToComponent(body);
    head->SetRelativePosition(0.f, 20.f); //몸통 위로 ##오프셋 값 수정할것.

    // 이동
    _movement = CreateActorComponent<MovementComponent>("Movement");
    _movement->SetUpdateComponent(_root);
    _movement->SetSpeed(100.f);

    // 카메라
    _camera = CreateSceneComponent<CameraComponent>("Cam");
    _camera->AttachToComponent(_root);




    return false;
}

void Player::Tick(float deltaTime)
{
    Pawn::Tick(deltaTime);
    
}

void Player::Collision(float deltaTime)
{
    Pawn::Collision(deltaTime);
}

void Player::Render(float deltaTime)
{
    Pawn::Render(deltaTime);
}

void Player::Destroy()
{
    Pawn::Destroy();

    TimeManager::Instance().RemoveTimer(_timerID);
}

void Player::MoveRight(float deltaTime)
{
    _movement->SetMoveAxis(FVector3D::Axis_X);

    Ptr<SpriteComponent> anim = FindSceneComponent<SpriteComponent>("Body");
    if (nullptr == anim)
        return;

    anim->SetAnimFilp(true);
}

void Player::MoveLeft(float deltaTime)
{
    _movement->SetMoveAxis(-FVector3D::Axis_X);

    Ptr<SpriteComponent> anim = FindSceneComponent<SpriteComponent>("Body");
    if (nullptr == anim)
        return;

    anim->SetAnimFilp(false);
}

void Player::MoveUp(float deltaTime)
{
    _movement->SetMoveAxis(FVector3D::Axis_Y);
}

void Player::MoveDown(float deltaTime)
{
    _movement->SetMoveAxis(-FVector3D::Axis_Y);
}

void Player::MoveStop(float deltaTime)
{
    _movement->Stop();
}


void Player::mouseDown(float deltaTime)
{

}


void Player::BlockCallBack(Weak<class CollisionComponent> dest)
{
    LogManager::Instance().Debug("충돌함..!");
}

void Player::OverlapCallBack(Weak<class CollisionComponent> dest)
{
    LogManager::Instance().Debug("지금 겹쳐있음..!");
}

void Player::ReleaseCallBack(Weak<class CollisionComponent> dest)
{
    LogManager::Instance().Debug("충돌했다가 이제 안합니다..!");
}


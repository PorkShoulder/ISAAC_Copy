#include "pch.h"
#include "Player.h"

#include "Controller/PlayerController.h"

#include "World/Level.h"
#include "World/World.h"


#include "Component/MovementComponent.h"
#include "Component/MeshComponent.h"
#include "Component/CameraComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SphereCollisionComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/SoundComponent.h"
#include "Component/WidgetComponent.h"

#include "Common/LogManager.h"

#include "Core/TimeManager.h"
#include "Core/GameEngine.h"

#include "UI/TextBlock.h"

bool Player::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Pawn::Init(id, pos, scale, rot, name);

    //컨트롤러를 생성(PlayerController로)
    _controller = GetLevel()->SpawnActor<PlayerController>("PlayerController", pos, scale, rot);

    //컨트롤러가 조종하는 대상을 지정
    _controller->SetPawn(This<Player>());

    // 아이작 몸 바디 
    // 아이작 몸통
    Ptr<SpriteComponent> body = CreateSceneComponent<SpriteComponent>("Body");
    body->AddAnimSequence("IASSC_BODY_FRONT", true);
    body->AddAnimSequence("IASSC_BODY_BACK", true);
    body->AddAnimSequence("IASSC_BODY_SIDE", true);

    body->ChangeAnimation("IASSC_BODY_FRONT");
    body->SetPlay("IASSC_BODY_FRONT", false);

    SetRootComponent(body);
    body->SetRelativeScale(64.f, 64.f, 64.f);

    // 아이작 머리
    Ptr<SpriteComponent> head = CreateSceneComponent<SpriteComponent>("Head");
    head->AddAnimSequence("IASSC_HEAD", false);
    head->SetPlay("IASSC_HEAD", false);

    head->AttachToComponent(body);
    head->SetRelativeScale(1.f, 1.f, 1.f);
    head->SetRelativePosition(0.f, 0.32f, 0.f); // 머리니까 몸통 크기만큼 +
  
    //InputComponent를 가져온다.
    Ptr<InputComponent> inputComp = GetController<PlayerController>()->GetInputComponent();

    //MoveContext를 있으면 가져오고 없으면 만들어서 가져온다.
    auto moveContext = InputSystem::Instance().FindOrAddInputContext("DEFAULT_CONTEXT");

    //MoveRight 액션을 있으면 가져오고 없으면 만들어서 가져온다.
    auto moveRight = InputSystem::Instance().FindOrAddInputAction("MOVE_RIGHT");
    auto moveleft = InputSystem::Instance().FindOrAddInputAction("MOVE_LEFT");
    auto moveUp = InputSystem::Instance().FindOrAddInputAction("MOVE_UP");
    auto moveDown = InputSystem::Instance().FindOrAddInputAction("MOVE_DOWN");
    auto change = InputSystem::Instance().FindOrAddInputAction("change");
    //auto dance = InputSystem::Instance().FindOrAddInputAction("Dance"); // To do 미사일로 나중에 바꿀것
    auto mouseL = InputSystem::Instance().FindOrAddInputAction("MouseL");

    //MoveContext 에 MoveRight 액션을 키보드 D키와 함께 바인딩한다.
    moveContext->BindInputAction(moveRight, 'D');
    moveContext->BindInputAction(moveleft, 'A');
    moveContext->BindInputAction(moveUp, 'W');
    moveContext->BindInputAction(moveDown, 'S');
    moveContext->BindInputAction(change, '1');
    //moveContext->BindInputAction(dance, VK_SPACE); // To do 미사일로 나중에 바꿀것
    moveContext->BindInputAction(mouseL, VK_LBUTTON);

    //InputComponent에 MoveContext를 등록한다.
    inputComp->AddInputContext("DEFAULT_CONTEXT");

    //InputComponent에 등록된 컨텍스트와 그 안에 액션에 따른 함수를 등록해준다.
    //누르고 있을때, Player 클래스의 MoveRight 함수를 호출해준다.
    inputComp->BindAction(moveContext->GetName(), moveRight->GetName(), INPUT_TYPE::DOWN, this, &Player::MoveRight);
    inputComp->BindAction(moveContext->GetName(), moveRight->GetName(), INPUT_TYPE::UP, this, &Player::MoveStop);

    inputComp->BindAction(moveContext->GetName(), moveUp->GetName(), INPUT_TYPE::DOWN, this, &Player::MoveUp);
    inputComp->BindAction(moveContext->GetName(), moveUp->GetName(), INPUT_TYPE::UP, this, &Player::MoveStop);
    
    inputComp->BindAction(moveContext->GetName(), moveDown->GetName(), INPUT_TYPE::DOWN, this, &Player::MoveDown);
    inputComp->BindAction(moveContext->GetName(), moveDown->GetName(), INPUT_TYPE::UP, this, &Player::MoveStop);

    inputComp->BindAction(moveContext->GetName(), moveleft->GetName(), INPUT_TYPE::DOWN, this, &Player::MoveLeft);
    inputComp->BindAction(moveContext->GetName(), moveleft->GetName(), INPUT_TYPE::UP, this, &Player::MoveStop);

    inputComp->BindAction(moveContext->GetName(), change->GetName(), INPUT_TYPE::DOWN, [&](float time)
        {
            auto Context = InputSystem::Instance().FindOrAddInputContext("DEFAULT_CONTEXT");
            Context->ChangeInputActionKey("MOVE_RIGHT", VK_RIGHT);
            Context->ChangeInputActionKey("MOVE_LEFT", VK_LEFT);
        });

    /*inputComp->BindAction(moveContext->GetName(), dance->GetName(), INPUT_TYPE::DOWN, this, &Player::OnDance);*/

    inputComp->BindAction(moveContext->GetName(), mouseL->GetName(), INPUT_TYPE::DOWN, this, &Player::mouseDown);

    //MovementComponent 생성한다.
    _movement = CreateActorComponent<MovementComponent>("Movement");
    _movement->SetUpdateComponent(_root);

    _movement->SetSpeed(100.f);

     _camera = CreateSceneComponent<CameraComponent>("Cam");

    //카메라 컴포넌트를 루트컴포넌트의 자식 컴포넌트로 붙힌다.
    _camera->AttachToComponent(_root);

    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(300.f, 300.f);
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Player");

  

    /*auto name_ui = CreateSceneComponent<WidgetComponent>("nameUI");
    auto nameWidget = GetLevel()->CreateWidget<TextBlock>("name");
    nameWidget->SetPos(FVector2D(-50.f, 25.f));
    nameWidget->SetSize(FVector2D(100.f, 100.f));
    nameWidget->SetText(TEXT("라이언"));
    name_ui->SetWidget(nameWidget);
    name_ui->AttachToComponent(_root);*/

    GameEngine::Instance().GetWorld()->SetMainPlayer(This<Player>());

    return true;
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

    Ptr<SpriteComponent> body = FindSceneComponent<SpriteComponent>("Body");
    //if (nullptr == body)
    if (!body)
        return;

    body->ChangeAnimation("IASSC_BODY_SIDE");
    body->SetAnimFilp(false);
    body->SetPlay("IASSC_BODY_SIDE", true);
}

void Player::MoveLeft(float deltaTime)
{
    _movement->SetMoveAxis(-FVector3D::Axis_X);

    Ptr<SpriteComponent> body = FindSceneComponent<SpriteComponent>("Body");
    //if (nullptr == body)
    if (!body)
        return;

    body->ChangeAnimation("IASSC_BODY_SIDE");
    body->SetAnimFilp(true);
    body->SetPlay("IASSC_BODY_SIDE", true);
}

void Player::MoveUp(float deltaTime)
{
    _movement->SetMoveAxis(FVector3D::Axis_Y);

    Ptr<SpriteComponent> body = FindSceneComponent<SpriteComponent>("Body");
    //if (nullptr == body)
    if (!body)
        return;

    body->ChangeAnimation("IASSC_BODY_BACK");
    body->SetPlay("IASSC_BODY_BACK", true);
    body->SetAnimFilp(false);
}

void Player::MoveDown(float deltaTime)
{
    _movement->SetMoveAxis(-FVector3D::Axis_Y);

    Ptr<SpriteComponent> body = FindSceneComponent<SpriteComponent>("Body");
    //if (nullptr == body)
    if (!body)
        return;

    body->ChangeAnimation("IASSC_BODY_FRONT");
    body->SetAnimFilp(false);
    body->SetPlay("IASSC_BODY_FRONT", true);
}

void Player::MoveStop(float deltaTime)
{
    _movement->Stop();

    Ptr<SpriteComponent> body = FindSceneComponent<SpriteComponent>("Body");
    //if (nullptr == body)
    if (!body)
        return;

    body->ChangeAnimation("IASSC_BODY_FRONT");
    body->SetAnimFilp(false);
    body->SetPlay("IASSC_BODY_FRONT", false);
}


void Player::mouseDown(float deltaTime)
{
    //
    LogManager::Instance().Debug("Mouse Down!");
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
    LogManager::Instance().Debug("충돌 해제!");
}

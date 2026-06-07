#include "pch.h"
#include "Player.h"

#include "Controller/PlayerController.h"

#include "World/Level.h"
#include "World/World.h"
#include "world/CameraManager.h"

#include "../Object/Bullet.h"


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
    body->AddAnimSequence("IASSC_BODY_SIDE", true);
    body->ChangeAnimation("IASSC_BODY_FRONT");
    body->SetPlay("IASSC_BODY_FRONT", false);

    // 아이작 머리
    Ptr<SpriteComponent> head = CreateSceneComponent<SpriteComponent>("Head");
    head->AddAnimSequence("IASSC_HEAD_FRONT", true);
    head->AddAnimSequence("IASSC_HEAD_SIDE", true);
    head->AddAnimSequence("IASSC_HEAD_BACK", true);
    head->ChangeAnimation("IASSC_HEAD_FRONT");
    head->SetPlay("IASSC_HEAD_FRONT", false);

    head->AttachToComponent(body);
    head->SetRelativeScale(1.f, 1.f, 1.f);
    head->SetRelativePosition(0.f, 0.32f, 0.f); // 머리니까 몸통 크기만큼 +

    SetRootComponent(body);
    body->SetRelativeScale(64.f, 64.f, 64.f);

    //InputComponent를 가져온다.
    Ptr<InputComponent> inputComp = GetController<PlayerController>()->GetInputComponent();

    //MoveContext를 있으면 가져오고 없으면 만들어서 가져온다.
    auto moveContext = InputSystem::Instance().FindOrAddInputContext("DEFAULT_CONTEXT");

  
    //몽통 액션을 있으면 가져오고 없으면 만들어서 가져온다.
    auto moveRight  = InputSystem::Instance().FindOrAddInputAction("MOVE_RIGHT");
    auto moveleft   = InputSystem::Instance().FindOrAddInputAction("MOVE_LEFT");
    auto moveUp     = InputSystem::Instance().FindOrAddInputAction("MOVE_UP");
    auto moveDown   = InputSystem::Instance().FindOrAddInputAction("MOVE_DOWN");
    auto change     = InputSystem::Instance().FindOrAddInputAction("change");
    auto mouseL     = InputSystem::Instance().FindOrAddInputAction("MouseL");

    //MoveContext 에 MoveRight 액션을 화살표와 함께 바인딩한다.
    moveContext->BindInputAction(moveRight, 'D');
    moveContext->BindInputAction(moveleft,  'A');
    moveContext->BindInputAction(moveUp,    'W');
    moveContext->BindInputAction(moveDown,  'S');
    moveContext->BindInputAction(mouseL, VK_LBUTTON);

    //머리 액션을 있으면 가져오고 없으면 만들어서 가져온다.
    auto headRight = InputSystem::Instance().FindOrAddInputAction("HEAD_RIGHT");
    auto headLeft = InputSystem::Instance().FindOrAddInputAction("HEAD_LEFT");
    auto headUp = InputSystem::Instance().FindOrAddInputAction("HEAD_UP");
    auto headDown = InputSystem::Instance().FindOrAddInputAction("HEAD_DOWN");

    //moveContext에 headRight 액션을 화살표와 함께 바인딩한다.
    moveContext->BindInputAction(headRight, VK_RIGHT);
    moveContext->BindInputAction(headLeft, VK_LEFT);
    moveContext->BindInputAction(headUp, VK_UP);
    moveContext->BindInputAction(headDown, VK_DOWN);

    //InputComponent에 MoveContext를 등록한다.
    inputComp->AddInputContext("DEFAULT_CONTEXT");

    //InputComponent에 등록된 컨텍스트와 그 안에 액션에 따른 함수를 등록해준다.
   //누르고 있을때, Player 클래스의 MoveRight 함수를 호출해준다.
    inputComp->BindAction(moveContext->GetName(), moveRight->GetName(), INPUT_TYPE::DOWN, this, &Player::MoveRight);
    inputComp->BindAction(moveContext->GetName(), moveRight->GetName(), INPUT_TYPE::UP, this, &Player::MoveRightStop);
    inputComp->BindAction(moveContext->GetName(), moveUp->GetName(), INPUT_TYPE::DOWN, this, &Player::MoveUp);
    inputComp->BindAction(moveContext->GetName(), moveUp->GetName(), INPUT_TYPE::UP, this, &Player::MoveUpStop);
    inputComp->BindAction(moveContext->GetName(), moveDown->GetName(), INPUT_TYPE::DOWN, this, &Player::MoveDown);
    inputComp->BindAction(moveContext->GetName(), moveDown->GetName(), INPUT_TYPE::UP, this, &Player::MoveDownStop);
    inputComp->BindAction(moveContext->GetName(), moveleft->GetName(), INPUT_TYPE::DOWN, this, &Player::MoveLeft);
    inputComp->BindAction(moveContext->GetName(), moveleft->GetName(), INPUT_TYPE::UP, this, &Player::MoveLeftStop);

    //누르고 있을때, Player 클래스의 MoveRight 함수를 호출해준다.
    inputComp->BindAction(moveContext->GetName(), headRight->GetName(), INPUT_TYPE::DOWN, this, &Player::HeadRight);
    inputComp->BindAction(moveContext->GetName(), headLeft->GetName(), INPUT_TYPE::DOWN, this, &Player::HeadLeft);
    inputComp->BindAction(moveContext->GetName(), headUp->GetName(), INPUT_TYPE::DOWN, this, &Player::HeadUp);
    inputComp->BindAction(moveContext->GetName(), headDown->GetName(), INPUT_TYPE::DOWN, this, &Player::HeadDown);

    inputComp->BindAction(moveContext->GetName(), headRight->GetName(), INPUT_TYPE::UP, this, &Player::HeadRelease);
    inputComp->BindAction(moveContext->GetName(), headLeft->GetName(), INPUT_TYPE::UP, this, &Player::HeadRelease);
    inputComp->BindAction(moveContext->GetName(), headUp->GetName(), INPUT_TYPE::UP, this, &Player::HeadRelease);
    inputComp->BindAction(moveContext->GetName(), headDown->GetName(), INPUT_TYPE::UP, this, &Player::HeadRelease);


    inputComp->BindAction(moveContext->GetName(), mouseL->GetName(), INPUT_TYPE::DOWN, this, &Player::mouseDown);

    //MovementComponent 생성한다.
    _movement = CreateActorComponent<MovementComponent>("Movement");
    _movement->SetUpdateComponent(_root);
    _movement->SetSpeed(0.f);
    _movement->SetMaxSpeed(120.f);
    _movement->SetAccel(200.f);    // 낮추면 천천히 가속
    _movement->SetFriction(400.f); // 가속보다 크면 빠르게 멈춤

     _camera = CreateSceneComponent<CameraComponent>("Cam");

    //카메라 컴포넌트를 루트컴포넌트의 자식 컴포넌트로 붙힌다.
    _camera->AttachToComponent(_root);
    
    // 충돌체 설정.
    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(32.f, 32.f);
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Player");

    GameEngine::Instance().GetWorld()->SetMainPlayer(This<Player>());

    return true;
}

void Player::Tick(float deltaTime)
{
    Pawn::Tick(deltaTime);
    LogManager::Instance().Debug("PLAYER FIRE");
    _fireTimer -= deltaTime;
    if (_headKeyActive && _fireTimer <= 0.f)
    {
        Fire();
        _fireTimer = _fireRate;
    }
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

void Player::SetHeadDirection(const std::string& anim, bool flip)
{
    if (_headKeyActive) return;
    Ptr<SpriteComponent> head = FindSceneComponent<SpriteComponent>("Head");
    if (!head) return;
    head->ChangeAnimation(anim);
    head->SetAnimFilp(flip);
}

void Player::UpdateMovement()
{
    FVector3D axis(0.f, 0.f, 0.f);

    if (_moveRight) axis._x += 1.f;
    if (_moveLeft)  axis._x -= 1.f;
    if (_moveUp)    axis._y += 1.f;
    if (_moveDown)  axis._y -= 1.f;

    Ptr<SpriteComponent> body = FindSceneComponent<SpriteComponent>("Body");
    if (!body) return;

    // 아무 키도 안 눌림 → 정지
    if (axis._x == 0.f && axis._y == 0.f)
    {
        _movement->SetMoveAxis(FVector3D::Zero);
        body->ChangeAnimation("IASSC_BODY_FRONT");
        body->SetAnimFilp(false);
        body->SetPlay("IASSC_BODY_FRONT", false);
        SetHeadDirection("IASSC_HEAD_FRONT", false);
        return;
    }

    _movement->SetMoveAxis(axis);

    // 애니메이션은 X축 우선
    if (axis._x > 0.f)
    {
        body->ChangeAnimation("IASSC_BODY_SIDE");
        body->SetAnimFilp(false);
        body->SetPlay("IASSC_BODY_SIDE", true);
        SetHeadDirection("IASSC_HEAD_SIDE", false);
    }
    else if (axis._x < 0.f)
    {
        body->ChangeAnimation("IASSC_BODY_SIDE");
        body->SetAnimFilp(true);
        body->SetPlay("IASSC_BODY_SIDE", true);
        SetHeadDirection("IASSC_HEAD_SIDE", true);
    }
    else if (axis._y > 0.f)
    {
        body->ChangeAnimation("IASSC_BODY_FRONT");
        body->SetAnimFilp(true);
        body->SetPlay("IASSC_BODY_FRONT", true);
        SetHeadDirection("IASSC_HEAD_BACK", false);
    }
    else if (axis._y < 0.f)
    {
        body->ChangeAnimation("IASSC_BODY_FRONT");
        body->SetAnimFilp(false);
        body->SetPlay("IASSC_BODY_FRONT", true);
        SetHeadDirection("IASSC_HEAD_FRONT", false);
    }
}

void Player::MoveRight(float deltaTime) { _moveRight = true;  UpdateMovement(); }
void Player::MoveLeft(float deltaTime) { _moveLeft = true;  UpdateMovement(); }
void Player::MoveUp(float deltaTime) { _moveUp = true;  UpdateMovement(); }
void Player::MoveDown(float deltaTime) { _moveDown = true;  UpdateMovement(); }
void Player::MoveRightStop(float deltaTime) { _moveRight = false; UpdateMovement(); }
void Player::MoveLeftStop(float deltaTime) { _moveLeft = false; UpdateMovement(); }
void Player::MoveUpStop(float deltaTime) { _moveUp = false; UpdateMovement(); }
void Player::MoveDownStop(float deltaTime) { _moveDown = false; UpdateMovement(); }

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

void Player::HeadRight(float deltaTime)
{
    _headKeyActive = true;
    _headDir = { 1.f, 0.f, 0.f };
    Ptr<SpriteComponent> head = FindSceneComponent<SpriteComponent>("Head");
    if (!head) return;
    head->ChangeAnimation("IASSC_HEAD_SIDE");
    head->SetAnimFilp(false);
}

void Player::HeadLeft(float deltaTime)
{
    _headKeyActive = true;
    _headDir = { -1.f, 0.f, 0.f };
    Ptr<SpriteComponent> head = FindSceneComponent<SpriteComponent>("Head");
    if (!head) return;
    head->ChangeAnimation("IASSC_HEAD_SIDE");
    head->SetAnimFilp(true);
}

void Player::HeadUp(float deltaTime)
{
    _headKeyActive = true;
    _headDir = { 0.f, 1.f, 0.f };
    Ptr<SpriteComponent> head = FindSceneComponent<SpriteComponent>("Head");
    if (!head) return;
    head->ChangeAnimation("IASSC_HEAD_BACK");
    head->SetAnimFilp(false);
}

void Player::HeadDown(float deltaTime)
{
    _headKeyActive = true;
    _headDir = { 0.f, -1.f, 0.f };
    Ptr<SpriteComponent> head = FindSceneComponent<SpriteComponent>("Head");
    if (!head) return;
    head->ChangeAnimation("IASSC_HEAD_FRONT");
    head->SetAnimFilp(false);
}

void Player::HeadRelease(float deltaTime)
{
    _headKeyActive = false;
}


void Player::mouseDown(float deltaTime)
{
    //
    LogManager::Instance().Debug("Mouse Down!");
}

void Player::Fire()
{
    Ptr<Level> level = GetLevel();
    if (!level) return;

    Ptr<Bullet> bullet = level->SpawnActor<Bullet>("bullet", GetWorldPosition(), GetWorldScale() / 2, GetWorldRotation());
    if (!bullet) return;

    bullet->SetProfile("PlayerBullet");   // 몬스터를 맞히는 총알
    bullet->SetDir(_headDir);             // 화살표(머리) 방향
    bullet->SetSpeed(200.f);
    
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

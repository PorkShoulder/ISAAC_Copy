#include "pch.h"
#include "Player.h"

#include "Controller/PlayerController.h"

#include "World/Level.h"
#include "World/World.h"
#include "world/CameraManager.h"

#include "../Object/Bullet.h"
#include "../Object/Monster.h"
#include "../Object/Obstacle.h"
#include "../Object/Door.h"
#include "../Object/Item.h"
#include "../Object/Npc.h"


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
#include "UI/Image.h"


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
    
    // 아이작 사망 애니메이션
    std::vector<FVector4D> deathFrames = { FVector4D(192.f, 128.f, 64.f, 64.f) };
    body->AddAnimSequence("IASSC_DEATH", TEXT("ISAAC_Charater\\issac_ctlas.png"), deathFrames, false);

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
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Player::OverlapCallBack);

    GameEngine::Instance().GetWorld()->SetMainPlayer(This<Player>());
    
    SetMaxHp(8);
    CreateHeartUI();
    

    return true;
}

void Player::Tick(float deltaTime)
{
    Pawn::Tick(deltaTime);

    if (_invincibleTimer > 0.f)
        _invincibleTimer -= deltaTime;
    for (int32 i = 0; i < _heartMax; ++i)
    {
        int32 slotHp = _hp - i * 2;
        if (slotHp >= 2)
            _hearts[i]->SetCurrentFrame(0);
        else if (slotHp == 1)
            _hearts[i]->SetCurrentFrame(1);
        else
            _hearts[i]->SetCurrentFrame(2);
    }

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
    if (HasEffect(EFFECT_HOMING))
        bullet->SetHoming(true);
    LogManager::Instance().Debug("Effects: " + std::to_string(_effects));
    bullet->SetDamage(_baseDamage + _bonusDamage);
    
}

void Player::OnDeath()
{
    Ptr<SpriteComponent> body = FindSceneComponent<SpriteComponent>("Body");
    if(body)
    {
        body->ChangeAnimation("IASSC_DEATH");
        body->SetPlay("IASSC_DEATH", false);
    }

    // 일정 시간 이후 타이틀 복귀 or 복귀 버튼 구현.
}

void Player::SetMaxHp(int32 v)
{
    Pawn::SetMaxHp(v);

    _heartMax = v / 2;
}

Ptr<class Image> Player::CreateSingleHeart(int32 idx)
{
    float texW = 112.f;
    float texH = 64.f;

    Ptr<Level> level = GetLevel();
    auto heart = level->CreateWidget<Image>("Heart_" + std::to_string(idx));
    heart->SetTexture("UI_Heart", TEXT("ISAAC_UI\\UI_Heart.png"));
    heart->SetBrushAnimEnable(true);
    heart->AddBrushFrame(0.f / texW, 0.f / texH, 16.f / texW, 16.f / texH);     // 풀 하트
    heart->AddBrushFrame(16.f / texW, 0.f / texH, 16.f / texW, 16.f / texH);    // 반 하트
    heart->AddBrushFrame(32.f / texW, 0.f / texH, 16.f / texW, 16.f / texH);    // 빈 하트
    heart->SetSize(FVector2D(32.f, 32.f));          // 화면 표시 크기
    heart->SetPos(FVector2D(20.f + idx * 24.f, 600.f));  // 좌상단 배치 
    level->AddToViewport(heart);
    return heart;

}

void Player::CreateHeartUI()
{
    for (int32 i = 0; i < _heartMax; ++i)
        _hearts.push_back(CreateSingleHeart(i));
}

void Player::AddHeartContainer()
{
    int32 idx = _heartMax;
    _heartMax++;
    _maxHp += 2;
    _hp += 2;
    _hearts.push_back(CreateSingleHeart(idx));
}

void Player::ApplyItemVisual(const FItemData& data)
{
    if (!data.equipHeadFrames.empty())
    {
        Ptr<SpriteComponent> head = FindSceneComponent<SpriteComponent>("Head");
        if (head)
        {
            head->AddAnimSequence("ITEM_HEAD", data.texturePath, data.equipHeadFrames, true);
            head->ChangeAnimation("ITEM_HEAD");
        }
    }
    if (!data.equipBodyFrames.empty())
    {
        Ptr<SpriteComponent> body = FindSceneComponent<SpriteComponent>("Body");
        if (body)
        {
            body->AddAnimSequence("ITEM_BODY", data.texturePath, data.equipBodyFrames, true);
            body->ChangeAnimation("ITEM_BODY");
        }
    }
}

void Player::ApplyItemStats(const FItemData& data)
{
    _bonusDamage += data.bonusDamage;
    _bonusSpeed += data.bonusSpeed;
    

    if (data.bonusHp > 0)
        AddHeartContainer();

    if (_movement && data.bonusSpeed != 0.f)
        _movement->SetMaxSpeed(_baseSpeed + _bonusSpeed);
}

void Player::AddHp(int32 amount)
{
    _hp = min(_hp + amount, _heartMax * 2);  // 최대 체력 초과 방지
    // 하트 UI 갱신 필요하면 여기서 처리
}



void Player::BlockCallBack(Weak<class CollisionComponent> dest)
{
    LogManager::Instance().Debug("충돌함..!");
}

void Player::OverlapCallBack(Weak<class CollisionComponent> dest)
{
    // 무적 중이면 무시함.
    if (_invincibleTimer > 0.f)
        return;

    Ptr<CollisionComponent> col = Lock<CollisionComponent>(dest);
    if (!col)
        return;

    // 탐지용 콜라이더는 데미지 무시
    if (col->GetName() == "Detect")
        return;

    Ptr<Actor> owner = col->GetOwner();
    if (!owner)
        return;

    // 몬스터
    Ptr<Monster> monster = Cast<Actor, Monster>(owner);
    if (monster)
    {
        TakeDamage((int32)monster->GetAttackPower());
        _invincibleTimer = _invincibleTime;
        if (IsDead())
            OnDeath();
        return;
    }

    // 장애물
    Ptr<Obstacle> obstacle = Cast<Actor, Obstacle>(owner);
    if (obstacle && obstacle->GetDamageAmount() > 0.f)
    {
        TakeDamage((int32)obstacle->GetDamageAmount());
        _invincibleTimer = _invincibleTime;
        if (IsDead())
            OnDeath();
        return;
    }
 
}

void Player::ReleaseCallBack(Weak<class CollisionComponent> dest)
{
    LogManager::Instance().Debug("충돌 해제!");
}


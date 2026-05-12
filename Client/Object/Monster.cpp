#include "pch.h"
#include "Monster.h"
#include "Bullet.h"
#include "RotBullet.h"
#include "Player.h"

#include "Core/TimeManager.h"
#include "Component/MeshComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SphereCollisionComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/AIComponent.h"

#include "Component/WidgetComponent.h"

#include "World/Level.h"

#include "Common/LogManager.h"

#include "Controller/AIController.h"

//#include "World/Level.h"


bool Monster::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Pawn::Init(id, pos, scale, rot, name);

    //AI State Machine 사용방법
    // 
    //1. AI컨트롤러를 생성(PlayerController로)
    _controller = GetLevel()->SpawnActor<AIController>("AIController", pos, scale, rot);

    //2. AI컨트롤러가 조종하는 대상을 지정
    _controller->SetPawn(This<Monster>());

    //3. AI컨트롤러로부터 AIComponent를 가져온다.(AIComponent는 AIController를 생성하면 자동으로 생성)
    Ptr<AIController> ctrl = Cast<Controller, AIController>(_controller);
    Ptr<AIComponent> aicomp = ctrl->GetAI();

    //4. MachineBase를 상속받은 스테이트머신 자식클래스로 스테이트머신을 생성
    

    Ptr<SpriteComponent> meshComp = CreateSceneComponent<SpriteComponent>("Mesh");
    //meshComp->SetMesh("Triangle");
    //meshComp->SetShader("ColorMeshShader");

    //meshComp->SetMesh("TexRect");
    //meshComp->AddTexture(0, "apeach", 0);

    meshComp->AddAnimSequence("APEACH_IDLE", true);
    meshComp->SetAnimFilp(true);


    SetRootComponent(meshComp);

    meshComp->SetRenderLayer("Monster");

    ////3초에 한번씩 플레이어 방향으로 총알 발사하기
    //_bulletTimer = TimeManager::Instance().SetTimer(1.f, true, this, &Monster::Fire);

    ////_rotBulletTimer = TimeManager::Instance().SetTimer(3.f, true, this, &Monster::RotBulletFire);

    ///*TimeManager::Instance().SetTimer(10.f, false, []()
    //    {
    //        LogManager::Instance().Debug("10초 후 실행하고 종료되는 타이머");
    //    });*/

    //Weak<Monster> mine = This<Monster>();
    /*TimeManager::Instance().SetTimer(15.f, false, [mine]()
        {
            Ptr<Monster> monster = Lock<Monster>(mine);
            if (nullptr == monster)
                return;

            float dela = TimeManager::Instance().GetDeltaTime();

            monster->Test(dela);
        });*/

        /*  _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
          _col->SetBoxSize(300.f, 300.f);
          _col->AttachToComponent(_root);
          _col->SetCollisionProfile("Monster");
          _col->SetCollisionCallBack(eCollisionState::COLLISION_STATE_BLOCK, this, &Monster::BlockCallback);*/

    _colSphere = CreateSceneComponent<SphereCollisionComponent>("Sphere");
    _colSphere->SetRedius(150.f);
    _colSphere->AttachToComponent(_root);
    _colSphere->SetCollisionProfile("Monster");
    /*_colSphere->SetCollisionCallBack(eCollisionState::COLLISION_STATE_BLOCK, this, &Monster::BlockCallback);*/

    /*auto name_ui = CreateSceneComponent<WidgetComponent>("nameUI");
    auto nameWidget = GetLevel()->CreateWidget<TextBlock>("name");
    nameWidget->SetPos(FVector2D(-50.f, 25.f));
    nameWidget->SetSize(FVector2D(100.f, 100.f));
    nameWidget->SetText(TEXT("어피치"));
    name_ui->SetWidget(nameWidget);
    name_ui->AttachToComponent(_root);

    auto hp_ui = CreateSceneComponent<WidgetComponent>("HPUI");
    auto hpWidget = GetLevel()->CreateWidget<ProgressBar>("HP");
    hpWidget->SetPos(FVector2D(0, 50));
    hpWidget->SetSize(FVector2D(100, 10));
    hpWidget->SetTexture(PROGRESS_BAR_IMAGE::FILL, "hp_img", TEXT("red_texture.png"));
    hp_ui->SetWidget(hpWidget);
    hp_ui->AttachToComponent(_root);

    auto img_ui = CreateSceneComponent<WidgetComponent>("ImageUI");
    auto imgWidget = GetLevel()->CreateWidget<Image>("IMAGE");
    imgWidget->SetPos(FVector2D(-50, 50));
    imgWidget->SetSize(FVector2D(30, 30));
    imgWidget->SetTexture("img_ui", TEXT("apeach.png"));
    img_ui->SetWidget(imgWidget);
    img_ui->AttachToComponent(_root);

    auto btn_ui = CreateSceneComponent<WidgetComponent>("buttonUI");
    auto btnWidget = GetLevel()->CreateWidget<Button>("button1");
    btnWidget->SetPos(FVector2D(80, 50));
    btnWidget->SetSize(FVector2D(50, 50));
    btnWidget->SetTexture(UI_BUTTON_STATE::ENABLE, "btnImage", TEXT("black.bmp"));
    btnWidget->SetTexture(UI_BUTTON_STATE::CLICK, "btnImage");
    btnWidget->SetTexture(UI_BUTTON_STATE::DISABLE, "btnImage");
    btnWidget->SetTexture(UI_BUTTON_STATE::HOVERED, "btnImage");

    btn_ui->SetWidget(btnWidget);
    btn_ui->AttachToComponent(_root);*/

    return true;
}

void Monster::Tick(float deltaTime)
{
    Pawn::Tick(deltaTime);
}

void Monster::Collision(float deltaTime)
{
    Pawn::Collision(deltaTime);
}

void Monster::Render(float deltaTime)
{
    Pawn::Render(deltaTime);
}

void Monster::Destroy()
{
    Pawn::Destroy();

    TimeManager::Instance().RemoveTimer(_bulletTimer);
    TimeManager::Instance().RemoveTimer(_rotBulletTimer);
}

void Monster::SetTarget(Ptr<class Player> player)
{
    _target = player;
}

void Monster::Fire()
{
    //총알 생성 및 총알 나가야 할 위치를 지정해준다.
    //속도랑
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    LogManager::Instance().Debug("Monster Fire..!");

    Ptr<Bullet> bullet = level->SpawnActor<Bullet>("bullet", GetWorldPosition(), GetWorldScale() / 2, GetWorldRotation());

    //Ptr<Player> player = level->FindActor()
    FVector3D targetPos = Lock<Player>(_target)->GetWorldPosition();
    FVector3D pos = GetWorldPosition();

    bullet->SetDir(targetPos - pos);
    bullet->SetSpeed(15);
}

void Monster::RotBulletFire()
{
    //총알 생성 및 총알 나가야 할 위치를 지정해준다.
   //속도랑
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    //LogManager::Instance().Debug("Monster Fire..!");

    Ptr<RotBullet> bullet = level->SpawnActor<RotBullet>("bullet", GetWorldPosition(), GetWorldScale() / 2, GetWorldRotation());

    //Ptr<Player> player = level->FindActor()
    FVector3D targetPos = Lock<Player>(_target)->GetWorldPosition();
    FVector3D pos = GetWorldPosition();

    bullet->SetDir(targetPos - pos);
    bullet->SetSpeed(10);
}


void Monster::BlockCallback(Weak<CollisionComponent> comp)
{
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;

    level->RemoveActor(_id);
}

bool Monster::IsCheck()
{
    Ptr<Actor> player = Lock<Player>(_target);
    if (!player)
        return false;

    float dist = player->GetWorldPosition().Distance(GetWorldPosition());
    if (dist <= 5)
        return true;
    else
        return false;
}


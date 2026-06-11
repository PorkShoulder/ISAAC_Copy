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


#include "../Component/MovementComponent.h"



#include "World/Level.h"

#include "../Core/TimeManager.h"

#include "Common/LogManager.h"

#include "Controller/AIController.h"

//#include "World/Level.h"


bool Monster::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Pawn::Init(id, pos, scale, rot, name);
    _type = eActorType::Monster;

    

    //AI State Machine 사용방법
    //1. AI컨트롤러를 생성(PlayerController로)
    _controller = GetLevel()->SpawnActor<AIController>("AIController", pos, scale, rot);

    //2. AI컨트롤러가 조종하는 대상을 지정
    _controller->SetPawn(This<Monster>());

    //3. AI컨트롤러로부터 AIComponent를 가져온다.(AIComponent는 AIController를 생성하면 자동으로 생성)
    Ptr<AIController> ctrl = Cast<Controller, AIController>(_controller);
    Ptr<AIComponent> aicomp = ctrl->GetAI();

    // 몬스터 스프라이트 생성 및 루트 컴포넌트 설정
    _monsterMesh= CreateSceneComponent<SpriteComponent>("Mesh");
    SetRootComponent(_monsterMesh);

    // 이동 동작 컴포넌트
    _movement = CreateActorComponent<MovementComponent>("Movement");
    _movement->SetUpdateComponent(_root);
    _movement->SetMaxSpeed(_monsterData.moveSpeed);
    _movement->SetAccel(200.f);
    _movement->SetFriction(400.f);


    // 1. 피격/접촉용 충돌체
    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(32.f, 32.f);   // 에디터에서 추가 설정변경 -> 동적(RoomObjectUI)
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Monster");
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Monster::OnHit); 
    
    // 2. 감지용 충돌체 
    _detectCol = CreateSceneComponent<SphereCollisionComponent>("Detect");
    _detectCol->SetRadius(_monsterData.detectRange);
    _detectCol->AttachToComponent(_root);
    _detectCol->SetCollisionProfile("MonsterDetect");
    _detectCol->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Monster::OnDetectPlayer);
    


    return true;
}

void Monster::Tick(float deltaTime)
{
    Pawn::Tick(deltaTime);

    Ptr<Player> player = Lock<Player>(_target);
    /*if (!_monsterMesh || !_monsterMesh->HasAnimation())
        return;*/
    if (_isChasing && player)
    {
        FVector3D dir = player->GetWorldPosition() - GetWorldPosition();
        dir.Normalize();
        _movement->SetMoveAxis(dir);
        UpdateAnimation(dir); //방향 전달
    }
    else
    {
        _movement->SetMoveAxis(FVector3D::Zero);
        UpdateAnimation(FVector3D::Zero);
    }
    
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

void Monster::Save(std::ofstream& file)
{
    Actor::Save(file);  // type, name, pos, scale, rot

    // --- FMonsterData ---
    // textureName
    int32 nameLen = (int32)_monsterData.textureName.size();
    file.write((char*)&nameLen, sizeof(int32));
    file.write(_monsterData.textureName.c_str(), nameLen);

    // texturePath (wstring)
    int32 pathLen = (int32)_monsterData.texturePath.size();
    file.write((char*)&pathLen, sizeof(int32));
    file.write((char*)_monsterData.texturePath.c_str(), pathLen * sizeof(wchar_t));

    // 수치 데이터
    file.write((char*)&_monsterData.moveSpeed, sizeof(float));
    file.write((char*)&_monsterData.chargeSpeed, sizeof(float));
    file.write((char*)&_monsterData.detectRange, sizeof(float));
    file.write((char*)&_monsterData.attackPower, sizeof(float));
    file.write((char*)&_monsterData.hp, sizeof(float));
    file.write((char*)&_monsterData.monsterType, sizeof(eMonsterType));
    file.write((char*)&_monsterData.renderSize, sizeof(FVector2D));
    file.write((char*)&_monsterData.collisionSize, sizeof(FVector2D));

    // 애니메이션 프레임 (5종)
    auto saveFrames = [&](const std::vector<FVector4D>& frames)
        {
            int32 count = (int32)frames.size();
            file.write((char*)&count, sizeof(int32));
            for (auto& f : frames)
                file.write((char*)&f, sizeof(FVector4D));
        };

    saveFrames(_monsterData.idleFrames);
    saveFrames(_monsterData.moveFrontFrames);
    saveFrames(_monsterData.moveSideFrames);
    saveFrames(_monsterData.moveBackFrames);
    saveFrames(_monsterData.deathFrames);
}

void Monster::Load(std::ifstream& file)
{
    Actor::Load(file);  // name, pos, scale, rot

    FMonsterData data;

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

    // 수치 데이터
    file.read((char*)&data.moveSpeed, sizeof(float));
    file.read((char*)&data.chargeSpeed, sizeof(float));
    file.read((char*)&data.detectRange, sizeof(float));
    file.read((char*)&data.attackPower, sizeof(float));
    file.read((char*)&data.hp, sizeof(float));
    file.read((char*)&data.monsterType, sizeof(eMonsterType));
    file.read((char*)&data.renderSize, sizeof(FVector2D));
    file.read((char*)&data.collisionSize, sizeof(FVector2D));

    // 애니메이션 프레임
    auto loadFrames = [&](std::vector<FVector4D>& frames)
        {
            int32 count = 0;
            file.read((char*)&count, sizeof(int32));
            frames.resize(count);
            for (int32 i = 0; i < count; i++)
                file.read((char*)&frames[i], sizeof(FVector4D));
        };

    loadFrames(data.idleFrames);
    loadFrames(data.moveFrontFrames);
    loadFrames(data.moveSideFrames);
    loadFrames(data.moveBackFrames);
    loadFrames(data.deathFrames);

    SetMonsterData(data);  // 충돌체, 이동속도, 애니메이션 전부 갱신
}

void Monster::SetTarget(Ptr<class Player> player)
{
    _target = player;
}

void Monster::SetMonsterData(const FMonsterData& data)
{
    
    _monsterData = data;
    SetMaxHp((int32)data.hp);

    if (_col)
        _col->SetBoxSize(data.collisionSize._x, data.collisionSize._y);

    if (_detectCol)
        _detectCol->SetRadius(data.detectRange);

    if (_movement)
        _movement->SetMaxSpeed(data.moveSpeed);

    //애니메이션 등록
    if (_monsterMesh && !data.texturePath.empty())
    {
        _animName = GetName() + "_ANIM";

        if (!data.idleFrames.empty())
            _monsterMesh->AddAnimSequence(_animName, data.texturePath, data.idleFrames, true);
        if (!data.moveFrontFrames.empty())
            _monsterMesh->AddAnimSequence(_animName, data.texturePath, data.moveFrontFrames, true);
        if (!data.moveBackFrames.empty())
            _monsterMesh->AddAnimSequence(_animName, data.texturePath, data.moveBackFrames, true);
        if (!data.moveSideFrames.empty())
            _monsterMesh->AddAnimSequence(_animName, data.texturePath, data.moveSideFrames, true);
        if (!data.deathFrames.empty())
            _monsterMesh->AddAnimSequence(_animName, data.texturePath, data.deathFrames, false);

        if (!data.idleFrames.empty())
            _monsterMesh->ChangeAnimation(_animName);
    }

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

void Monster::UpdateAnimation(const FVector3D& dir)
{
    // 스프라이트가 없으면 리턴함.
    if (!_monsterMesh)
        return;

    _animName = GetName() + "_";


    // 타겟(플레이어) 가져오기 
    if (dir._x == 0.f && dir._y ==0.f)
    {
        // 플레이어가 없으면 대기 애니메이션
        _monsterMesh->ChangeAnimation(_animName + "IDLE");
        return;
    }

    // X차이가 Y보다 크다면 좌우 이동 
    if (abs(dir._x) > abs(dir._y))
    {
        // dir._x < 0 -> 플레이어가 왼쪽 -> 스프라이트 좌우 반전.
        _monsterMesh->ChangeAnimation(_animName + "MOVE_SIDE");
        _monsterMesh->SetAnimFilp(dir._x < 0.f);
    }
    // y차이가 더 크면 정면/뒷면 이동 애니메이션
    else if(dir._y > 0.f)
    {
        _monsterMesh->ChangeAnimation(_animName + "MOVE_BACK");
        
    }
    else 
    {
        _monsterMesh->ChangeAnimation(_animName + "MOVE_FRONT");
        
    }
}

void Monster::OnDetectPlayer(Weak<class CollisionComponent> dest)
{
    Ptr<CollisionComponent> col = Lock<CollisionComponent>(dest);
    if (!col) return;

    Ptr<Actor> owner = col->GetOwner();
    if (!owner) return;

    Ptr<Player> player = Cast<Actor, Player>(owner);
    if (!player) return;

    _target = player;
    _isChasing = true;
}

void Monster::OnHit(Weak<class CollisionComponent> dest)
{
    Ptr<CollisionComponent> col = Lock<CollisionComponent>(dest);
    if (!col) return;

    Ptr<Actor> owner = col->GetOwner();
    if (!owner) return;

    // 플레이어 총알에 맞았을 때
    Ptr<Bullet> bullet = Cast<Actor, Bullet>(owner);
    if (bullet)
    {
        TakeDamage(1);
        if (IsDead())
            OnDeath();
        
    }
}

void Monster::OnDeath()
{
    
    _isChasing = false;         //몬스터 사망 추격 중지
    if(_movement)
        _movement->SetMoveAxis(FVector3D::Zero);    // 움직임 멈춤 

    if (_col) 
        _col->SetEnable(false); 

    if (_detectCol)
        _detectCol->SetEnable(false);

    if (_monsterMesh)
    {
        _monsterMesh->ChangeAnimation("DEATH");
        _monsterMesh->SetPlay("DEATH", false);
    }

    TimeManager::Instance().SetTimer(0.5f, false, this, &Monster::RemoveMonster);
}

void Monster::RemoveMonster()
{
    Pawn::OnDeath();
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


#include "pch.h"
#include "Pawn.h"
#include "Controller/Controller.h"

#include "../Common/LogManager.h"
#include "World/Level.h"

Pawn::Pawn()
{
}

Pawn::~Pawn()
{
}


bool Pawn::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);

    _type = eActorType::Pawn;

    return true;
}

void Pawn::Tick(float deltaTime)
{
	//컨트롤러가 있다면 컨트롤러의 Tick 함수를 호출해준다.
	if (_controller)
		_controller->Tick(deltaTime);
   

	Actor::Tick(deltaTime);
}

void Pawn::Collision(float deltaTime)   
{
    Actor::Collision(deltaTime);
}

void Pawn::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Pawn::Destroy()
{
    Actor::Destroy();

    Ptr<Level> level = GetLevel();
    if (!level)
        return;

     if (_controller)
        _controller->SetActive(false);

    //level->RemoveActor(_controller->GetActorID());
}

void Pawn::TakeDamage(int32 dmg)
{
    if(_hp <= 0)    // 체력이 0보다 낮음 -> 이미 사망함.
        return;
    _hp -= dmg;     // 체력에 피해량 만큼 감소해서 대입
    LogManager::Instance().Debug("TakeDamage!!! hp =" + std::to_string(_hp));
    
}

void Pawn::OnDeath()
{
    // 기본 동작: 레벨에서 자기 제거
    Ptr<Level> level = GetLevel();
    if (level)
        level->RemoveActor(GetActorID());


}

void Pawn::SetController(Ptr<class Controller> ctrl)
{
    if (_controller)
        DESTROY(_controller);

    _controller = ctrl;
}
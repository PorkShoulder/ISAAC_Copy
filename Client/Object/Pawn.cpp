#include "pch.h"
#include "Pawn.h"
#include "Controller/Controller.h"

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

void Pawn::SetController(Ptr<class Controller> ctrl)
{
    if (_controller)
        DESTROY(_controller);

    _controller = ctrl;
}
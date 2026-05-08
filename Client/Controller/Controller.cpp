#include "pch.h"
#include "Controller.h"

bool Controller::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
    Actor::Init(id, pos, scale, rot);

    //컨트롤러는 enable false로
    SetEnable(false);

    return true;
}

void Controller::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Controller::Collision(float deltaTime)
{
    //nothing
}

void Controller::Render(float deltaTime)
{
    //nothing
}

void Controller::Destroy()
{
    Actor::Destroy();
}

void Controller::SetPawn(Ptr<class Pawn> pawn)
{
    _pawn = pawn;
}

Ptr<class Pawn> Controller::GetPawn() const
{
    return Lock<Pawn>(_pawn);
}

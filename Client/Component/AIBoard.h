#pragma once
#include "../Core/Object.h"
#include "../Object/Actor.h"

class AIBoard : public Object
{
public:
    AIBoard();
    virtual ~AIBoard();

public:
    virtual void Destroy() override;
};


class AIBoard_Apeach : public AIBoard
{
public:
    AIBoard_Apeach();
    virtual ~AIBoard_Apeach();

public:
    float _dist;
    Weak<Actor> _actor;

    void SetDist(float dist)
    {
        _dist = dist;
    }

    void SetTarget(Ptr<Actor> target)
    {
        _actor = target;
    }
};



#pragma once
#include "Player.h"


class IsaacPlayer : public Player
{
public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale,
        const FRotator& rot, const std::string& name) override;

private:
    void InitBody();
    void InitHead();
    void InitInput();

    // 머리 방향 함수
    void HeadRight(float deltaTime);
    void HeadLeft(float deltaTime);
    void HeadUp(float deltaTime);
    void HeadDown(float deltaTime);
};
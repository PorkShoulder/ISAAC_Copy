#pragma once

#include "../Object/Actor.h"
#include "../Controller/Controller.h"

//=====================================
// Actor를 상속받아서 Controller를 가질수 있는 클래스
// 조종될 수 있는 Actor
//=====================================

class Pawn : public Actor
{
public:
    Pawn();
    virtual ~Pawn();

protected:
    Ptr<Controller> _controller; // Pawn을 조종하는 컨트롤러 (PlayerController면 키보드,마우스 입력으로 조종함.)

    // 캐릭터 
    int32 _hp = 1;
    int32 _maxHp = 1;

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;

    virtual void TakeDamage(int32 dmg); // 피해 받기
    virtual void OnDeath();             // 사망처리 

    bool IsDead() const { return _hp <= 0; }
    int32 GetHp() const { return _hp; }
    int32 GetMaxHp() const { return _maxHp; }
    void SetMaxHp(int32 v) { _maxHp = v; _hp = v; } 

public:
    void SetController(Ptr<Controller> ctrl); //컨트롤러 연결

    template<typename T>
    Ptr<T> GetController() const    //컨트롤러가 원하는 타입으로 캐스팅함.
    {
        return Cast<Controller, T>(_controller);
    }
};


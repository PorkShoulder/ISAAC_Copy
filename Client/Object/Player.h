#pragma once
#include "Pawn.h"

//====================================
// Pawn을 상속받은 플레이어 캐릭터, 
// 컴포넌트를 조립해서
// 실제 게임에서 동작하는 캐릭터를 만드는곳.
//====================================


class Player : public Pawn
{
public:
    Player() = default;
    virtual ~Player() = default;

private:
    Ptr<class MovementComponent> _movement;     // 이동처리 
    Ptr<class CameraComponent> _camera;         // 카메라가 플레이어를 따라다님.
    Ptr<class AABBCollisionComponent> _col;     // 충돌판정 
    Ptr<class SphereCollisionComponent> _colSphere; 
    Ptr<class SoundComponent> _sound;
    int32 _timerID = -1;

    bool reverse = false;
    bool _headKeyActive = false;
    float _opacity = 0.f;
    
    // 피격 무적
    float _invincibleTime = 1.0f; // 무적 지속 시간(초)
    float _invincibleTimer = 0.f; // 남은 무적 시간.


    //이동조작 플래그
    bool _moveRight = false;
    bool _moveLeft = false;
    bool _moveUp = false;
    bool _moveDown = false;

    // 총알 발사와 머리 방향 일치
    FVector3D _headDir = FVector3D{ 0.f, -0.f, 0.f }; 
    float _fireRate = 0.8f; // 발사간격
    float _fireTimer = 1.f; // 쿨다운

    // 아이템 소모
    FConsumableStat _consumable;

    // 캐릭터 UI
    std::vector<Ptr<class Image>> _hearts;
    int32 _heartMax = 4; // 시작 하트 수, 아이템으로 증가 가능.

    // 캐릭터 능력치
    float _baseDamage = 1.f;
    float _baseSpeed = 120.f;
    int32 _baseHp = 8;

    float _bonusDamage = 0.f;
    float _bonusSpeed = 0.f;


    // 아이템 효과적용
    uint32 _effects = EFFECT_NONE;

    // 총알 외형정보
    std::string _bulletTextureName;
    std::wstring _bulletTexturePath;
    std::vector<FVector4D> _bulletFrames;

    

public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;
public:
    int GetKey() const { return _consumable.key; }
    int GetCoin() const { return _consumable.coin; }
    int GetBomb() const { return _consumable.bomb; }
    void AddKey(int amount) { _consumable.key += amount; }
    void AddCoin(int amount) { _consumable.coin += amount; }
    void AddBomb(int amount) { _consumable.bomb += amount; }
    void OnDeath();
    virtual void SetMaxHp(int32 v) override;
    Ptr<class Image> CreateSingleHeart(int32 idx);
    void CreateHeartUI(); // 체력
    void AddHeartContainer();
    void ApplyItemVisual(const FItemData& data);
    void ApplyItemStats(const FItemData& data);
    bool HasEffect(uint32 e) const { return (_effects & e) != 0; }
    void AddEffect(uint32 e) { _effects |= e; }
    void AddHp(int32 amount);

private:
    void SetHeadDirection(const std::string& anim, bool flip);
    void UpdateMovement();
    void MoveRight(float deltaTime);
    void MoveLeft(float deltaTime);
    void MoveUp(float deltaTime);
    void MoveDown(float deltaTime);
    void MoveRightStop(float deltaTime);
    void MoveLeftStop(float deltaTime);
    void MoveUpStop(float deltaTime);
    void MoveDownStop(float deltaTime);

    void MoveStop(float deltaTime);

    //아이작 머리 조작
    void HeadRight(float deltaTime);
    void HeadLeft(float deltaTime);
    void HeadUp(float deltaTime);
    void HeadDown(float deltaTime);
    void HeadRelease(float deltaTime);
    void mouseDown(float deltaTime);
    
    void Fire();
private:
    void BlockCallBack(Weak<class CollisionComponent> dest);
    void OverlapCallBack(Weak<class CollisionComponent> dest);
    void ReleaseCallBack(Weak<class CollisionComponent> dest);
    



};

#include "pch.h"
#include "Obstacle.h"
#include "Bullet.h"
#include "Player.h"

#include "../Component/SpriteComponent.h"
#include "../Component/AABBCollisionComponent.h"



Obstacle::Obstacle()
{
}

Obstacle::~Obstacle()
{
}

bool Obstacle::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    Actor::Init(id, pos, scale, rot, name);
    _type = eActorType::Obstacle;

    // 장애물 스프라이트 생성 및 루트 컴포넌트 설정.
    _obstacleMesh = CreateSceneComponent<SpriteComponent>("Mesh");
    SetRootComponent(_obstacleMesh);

    _col = CreateSceneComponent<AABBCollisionComponent>("AABB");
    _col->SetBoxSize(50.f, 50.f);
    _col->AttachToComponent(_root);
    _col->SetCollisionProfile("Obstacle");
    _col->SetCollisionCallBack(COLLISION_STATE_OVERLAP, this, &Obstacle::OnOverlap);
    
    

    

    return true;
}

void Obstacle::Tick(float deltaTime)
{
    Actor::Tick(deltaTime);
}

void Obstacle::Collision(float deltaTime)
{
    
}

void Obstacle::Render(float deltaTime)
{
    Actor::Render(deltaTime);
}

void Obstacle::Destroy()
{
    Actor::Destroy();
}

void Obstacle::Save(std::ofstream& file)
{
    Actor::Save(file);

    // obstacleType
    file.write((char*)&_data.obstacleType, sizeof(eObstacleType));

    // textureName
    int32 nameLen = (int32)_data.textureName.size();
    file.write((char*)&nameLen, sizeof(int32));
    file.write(_data.textureName.c_str(), nameLen);

    // texturePath
    int32 pathLen = (int32)_data.texturePath.size();
    file.write((char*)&pathLen, sizeof(int32));
    file.write((char*)_data.texturePath.c_str(), pathLen * sizeof(wchar_t));

    // 동작 데이터
    file.write((char*)&_data.canFlyPass, sizeof(bool));
    file.write((char*)&_data.canWalkPass, sizeof(bool));
    file.write((char*)&_data.bulletBreakable, sizeof(bool));
    file.write((char*)&_data.bulletPassThrough, sizeof(bool));
    file.write((char*)&_data.bombBreakable, sizeof(bool));
    file.write((char*)&_data.damage, sizeof(bool));
    file.write((char*)&_data.damageAmount, sizeof(float));
    file.write((char*)&_data.hp, sizeof(int32));

    // 크기
    file.write((char*)&_data.renderSize, sizeof(FVector2D));
    file.write((char*)&_data.collisionSize, sizeof(FVector2D));

    // animFrames
    int32 frameCount = (int32)_data.animFrames.size();
    file.write((char*)&frameCount, sizeof(int32));
    for (auto& f : _data.animFrames)
        file.write((char*)&f, sizeof(FVector4D));

}

void Obstacle::Load(std::ifstream& file)
{
    Actor::Load(file);

    FObstacleData data;

    file.read((char*)&data.obstacleType, sizeof(eObstacleType));

    int32 nameLen = 0;
    file.read((char*)&nameLen, sizeof(int32));
    data.textureName.resize(nameLen);
    file.read(&data.textureName[0], nameLen);

    int32 pathLen = 0;
    file.read((char*)&pathLen, sizeof(int32));
    data.texturePath.resize(pathLen);
    file.read((char*)&data.texturePath[0], pathLen * sizeof(wchar_t));

    file.read((char*)&data.canFlyPass, sizeof(bool));
    file.read((char*)&data.canWalkPass, sizeof(bool));
    file.read((char*)&data.bulletBreakable, sizeof(bool));
    file.read((char*)&data.bulletPassThrough, sizeof(bool));
    file.read((char*)&data.bombBreakable, sizeof(bool));
    file.read((char*)&data.damage, sizeof(bool));
    file.read((char*)&data.damageAmount, sizeof(float));
    file.read((char*)&data.hp, sizeof(int32));

    file.read((char*)&data.renderSize, sizeof(FVector2D));
    file.read((char*)&data.collisionSize, sizeof(FVector2D));

    int32 frameCount = 0;
    file.read((char*)&frameCount, sizeof(int32));
    data.animFrames.resize(frameCount);
    for (int32 i = 0; i < frameCount; i++)
        file.read((char*)&data.animFrames[i], sizeof(FVector4D));

    SetObstacleData(data);
}

void Obstacle::SetObstacleData(const FObstacleData& data)
{
    _data = data;
    _obstacleType = data.obstacleType;
    _hp = data.hp;
 
    // 충돌체
    if (_col)
        _col->SetBoxSize(data.collisionSize._x, data.collisionSize._y);

    // 텍스처 & 애니메이션
    if (_obstacleMesh && !data.texturePath.empty() && !data.animFrames.empty())
    {
        std::string animName = GetName() + "_ANIM";

        _obstacleMesh->AddAnimSequence(animName, data.texturePath, data.animFrames, true);
        _obstacleMesh->ChangeAnimation(animName);
        _obstacleMesh->SetPlay(animName, true);
    }
}

void Obstacle::OnOverlap(Weak<class CollisionComponent> dest)
{
    Ptr<Actor> owner = dest.lock()->GetOwner();

    // 총알 처리
    Ptr<Bullet> bullet = Cast<Actor, Bullet>(owner);
    if (bullet)
    {
        if (_data.bulletBreakable)
        {
            _hp--;
            bullet->Remove();
            if (_hp <= 0)
                Remove();
        }
        else if (!_data.bulletPassThrough)
        {
            // 총알 막힘 → 총알만 제거
            bullet->Remove();
        }
        // bulletPassThrough면 아무것도 안 함
        return;
    }

    // ===== 플레이어 처리 =====
    Ptr<Player> player = Cast<Actor, Player>(owner);
    if (player)
    {
        // 이동 차단 (canWalkPass == false면 못 지나감)
        if (!_data.canWalkPass)
        {
            FVector3D playerPos = player->GetWorldPosition();
            FVector3D obstPos = GetWorldPosition();

            float dx = playerPos._x - obstPos._x;
            float dy = playerPos._y - obstPos._y;

            float halfW = _data.collisionSize._x * 0.5f + 16.f; // 장애물 반 + 플레이어 반
            float halfH = _data.collisionSize._y * 0.5f + 16.f;

            float overlapX = halfW - abs(dx);
            float overlapY = halfH - abs(dy);

            if (overlapX > 0 && overlapY > 0)
            {
                // 겹침이 적은 축으로 밀어냄
                if (overlapX < overlapY)
                    playerPos._x += (dx > 0 ? overlapX : -overlapX);
                else
                    playerPos._y += (dy > 0 ? overlapY : -overlapY);

                player->SetWorldPosition(playerPos);
            }
        }

        // 피해
        if (_data.damage)
        {
            player->TakeDamage((int32)_data.damageAmount);
        }
        return;
    }
}

void Obstacle::OnHitByBullet(Weak<class CollisionComponent> dest)
{
    Ptr<CollisionComponent> col = Lock<CollisionComponent>(dest);
    if (!col)
        return;

    Ptr<Actor> owner = col->GetOwner();
    if (!owner)
        return;
    
    // 총알 처리
    Ptr<Bullet> bullet = Cast<Actor, Bullet>(owner);
    if (bullet)
    {
        if (_data.bulletBreakable)
        {
            _hp--;
            bullet->Remove();
            if (_hp <= 0)
                Remove();
        }
        else if (!_data.bulletPassThrough)
        {
            bullet->Remove();
        }
        return;
    }

    // 플레이어 처리 (피해만 — 이동 차단은 충돌 시스템에서)
    Ptr<Player> player = Cast<Actor, Player>(owner);
    if (player && _data.damage)
    {
        player->TakeDamage((int32)_data.damageAmount);
    }
        
}

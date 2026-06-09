#include "pch.h"
#include "IsaacAnimationLoader.h"
#include "../Core/AssetManager.h"

void IsaacAnimationLoader::Register(AnimationManager* mgr)
{
    constexpr float CELL = 32.f;
    ////////////// 머리    
    // 머리 정면
    mgr->CreateAnimation("IASSC_HEAD_FRONT", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_HEAD_FRONT", "IASSC_HEAD_FRONT", TEXT("ISAAC_Character\\isaac_atlas.png"));
    mgr->AddFrame("IASSC_HEAD_FRONT", 0.f, 0.f, CELL, CELL);  // 프레임 0
    mgr->AddFrame("IASSC_HEAD_FRONT", 1.f * CELL , 0.f, CELL, CELL);  // 프레임 0

    // 머리 측면
    mgr->CreateAnimation("IASSC_HEAD_SIDE", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_HEAD_SIDE", "IASSC_HEAD_SIDE", TEXT("ISAAC_Character\\isaac_atlas.png"));
    mgr->AddFrame("IASSC_HEAD_SIDE", 2.f * CELL, 0.f, CELL, CELL);  // 프레임 1
    mgr->AddFrame("IASSC_HEAD_SIDE", 3.f * CELL, 0.f, CELL, CELL);  // 프레임 1

    // 머리 후면
    mgr->CreateAnimation("IASSC_HEAD_BACK", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_HEAD_BACK", "IASSC_HEAD_BACK", TEXT("ISAAC_Character\\isaac_atlas.png"));
    mgr->AddFrame("IASSC_HEAD_BACK", 4.f * CELL, 0.f, CELL, CELL);  // 프레임 2
    mgr->AddFrame("IASSC_HEAD_BACK", 5.f * CELL, 0.f, CELL, CELL);  // 프레임 2

    ////////////// 몸통
    // 몸통 전면
    mgr->CreateAnimation("IASSC_BODY_FRONT", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_BODY_FRONT", "IASSC_BODY_FRONT", TEXT("ISAAC_Character\\isaac_atlas.png"));
    for (int32 i = 0; i < 8; ++i)
    {
        mgr->AddFrame("IASSC_BODY_FRONT", i * CELL, 1.f * CELL, CELL, CELL);
    }
    mgr->AddFrame("IASSC_BODY_FRONT", 6.f * CELL, 0.f, CELL, CELL);
    mgr->AddFrame("IASSC_BODY_FRONT", 7.f * CELL, 0.f, CELL, CELL);
   

    // 몸통 측면
    mgr->CreateAnimation("IASSC_BODY_SIDE", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_BODY_SIDE", "IASSC_BODY_SIDE", TEXT("ISAAC_Character\\isaac_atlas.png"));
    for (int32 i = 0; i < 8; ++i)
    {
        mgr->AddFrame("IASSC_BODY_SIDE", i * CELL, 2.f * CELL, CELL, CELL);
    }
    for (int32 i = 0; i < 2; ++i)
    {
        mgr->AddFrame("IASSC_BODY_SIDE", i * CELL, 3.f * CELL, CELL, CELL);
    }

    // 플레이어 사망 애니메이션
    mgr->CreateAnimation("IASSC_DEATH", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_DEATH", "IASSC_DEATH", TEXT("ISAAC_Character\\isaac_atlas.png"));
    mgr->AddFrame("IASSC_DEATH", 192.f, 128.f, 64.f, 64.f);

    
    // 총알(눈물)
    mgr->CreateAnimation("Tears", eAnimTextureType::SPRITE);
    mgr->SetTexture("Tears", "Tears", TEXT("ISAAC_Tears\\bulletatlas.png"));   // 눈물 스프라이트가 든 아틀라스 경로


    
    constexpr float TEAR = 64.f; // 총알 전용 사이즈 
    constexpr float TEAR_X = 256.f;
    constexpr float TEAR_Y = 256.f;
    for (int32 row = 0; row < 3; ++row)
    {
        for (int32 col = 0; col < 4; ++col)
        {
            mgr->AddFrame("Tears", TEAR_X + col * TEAR, TEAR_Y + row * TEAR, TEAR, TEAR);
        }
         
    }
 
    // 플레이어 사망 애니메이션




}

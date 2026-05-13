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
    
}

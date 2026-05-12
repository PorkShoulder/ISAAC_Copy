#include "pch.h"
#include "IsaacAnimationLoader.h"
#include "../Core/AssetManager.h"

void IsaacAnimationLoader::Register(AnimationManager* mgr)
{
    constexpr float CELL = 32.f;

    // 1. 머리 첫줄 
    mgr->CreateAnimation("IASSC_HEAD", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_HEAD", "IASSC_HEAD", TEXT("ISAAC_Character\\isaac_atlas.png"));

    for (int32 i = 0; i < 6; ++i)
    {
        mgr->AddFrame("IASSC_HEAD", i * CELL, 0.f, CELL, CELL);
    }
     

    // 2. 몸통 전면부
    mgr->CreateAnimation("IASSC_BODY_FRONT", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_BODY_FRONT", "IASSC_BODY_FRONT", TEXT("ISAAC_Character\\isaac_atlas.png"));

    for (int32 i = 0; i < 8; ++i)
    {
        mgr->AddFrame("IASSC_BODY_FRONT", i * CELL, 1.f * CELL, CELL, CELL);
    }

    // 3. 몸통 후변부
    mgr->CreateAnimation("IASSC_BODY_BACK", eAnimTextureType::SPRITE);
    mgr->SetTexture("IASSC_BODY_BACK", "IASSC_BODY_BACK", TEXT("ISAAC_Character\\isaac_atlas.png"));

    mgr->AddFrame("IASSC_BODY_BACK", 6.f * CELL, 0.f, CELL, CELL);
    mgr->AddFrame("IASSC_BODY_BACK", 7.f * CELL, 0.f, CELL, CELL);



    // 4. 몸통 측면부
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

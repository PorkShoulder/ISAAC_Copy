#include "pch.h"
#include "TitleLevel.h"

#include "../Common/LogManager.h"

#ifdef _EDITOR
#include "../Editor/imgui.h"
#endif

#include "EditorLevel.h"
#include "../UI/Image.h"
#include "../Input/InputSystem.h"
#include "../Core/GameEngine.h"
#include "../Core/Device.h"
#include "GameLevel.h"
#include "World.h"
#include "../Input/Input.h"



bool TitleLevel::Init(const std::string& path)
{
    
    Level::Init(path);

    FResolution rs = Device::Instance().GetRS();

    // ── 1) 배경 (상단 절반) ── 화면 전체를 덮도록
    auto bg = CreateWidget<Image>("Title_BG");
    bg->SetTexture("Menu_Title", TEXT("ISAAC_UI\\Menu_Title.png"));
    bg->SetBrushAnimEnable(true);                       // 애니메이션 없음, 고정 프레임
    bg->AddBrushFrame(0.f, 0.f, 1.f, 0.5f);             // UV: 상단 절반
    bg->SetSize(FVector2D((float)rs._width, (float)rs._height));  // 화면 전체 크기
    bg->SetPos(FVector2D((float)rs._width / 2.f, (float)rs._height / 2.f));
    AddToViewport(bg);
    //AddToViewport(anim);
    auto anim = CreateWidget<Image>("Title_Anim");
    anim->SetTexture("Menu_Title", TEXT("ISAAC_UI\\Menu_Title.png"));
    anim->SetBrushAnimEnable(true);
    anim->AddBrushFrame(0.2f, 0.504f, 0.6f, 0.2f);   // 하단 왼쪽 절반
    float sx = (float)rs._width / 480.f;   // 2.133
    float sy = (float)rs._height / 270.f;  // 2.370
    anim->SetSize(FVector2D(288.f * sx, 56.f * sx));
    anim->SetPos(FVector2D((float)rs._width / 2.f, (float)rs._height / 1.2f));
    AddToViewport(anim);
    //
    auto pressStart = CreateWidget<Image>("Press_Start");
    pressStart->SetTexture("Menu_Title_starting", TEXT("ISAAC_UI\\Menu_Title_starting.png"));
    pressStart->SetBrushAnimEnable(true);
    pressStart->AddBrushFrame(0.f, 0.f, 1.f, 1.f);  // 왼쪽 프레임 1개만
    pressStart->SetSize(FVector2D(480.f, 432.f));
    pressStart->SetPos(FVector2D((float)rs._width / 2.f, (float)rs._height / 2.f - 50.f));
    AddToViewport(pressStart);



    return true;

}

void TitleLevel::Tick(float deltaTime)
{
    Level::Tick(deltaTime);
#ifdef _EDITOR
    if (ImGui::GetIO().WantCaptureMouse)
        return;
#endif
    if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton))
        GameEngine::Instance().GetWorld()->ChangeLevel<GameLevel>("Game");

    if (GetAsyncKeyState(VK_F1) & 0x8000)
        GameEngine::Instance().GetWorld()->ChangeLevel<EditorLevel>("Editor");
}

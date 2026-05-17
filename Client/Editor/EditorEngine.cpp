#include "pch.h"
#include "EditorEngine.h"

#include "Core/GameEngine.h"
#include "Core/Device.h"
#include "Core/TimeManager.h"

#include "World/World.h"
#include "Render/RenderManager.h"

#include "EditorUI.h"
#include "MainMenuUI.h"
#include "OutLinerUI.h"
#include "InspectorUI.h"
#include "PopUpUI.h"
#include "SaveUI.h"
#include "LoadUI.h"
#include "TileMapUI.h"

#include "Client.h"

bool EditorEngine::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    // Setup Platform/Renderer backends
    Device::Instance().GetDevice();
    ImGui_ImplWin32_Init(GameEngine::Instance().GetHWND());
    ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get());

    g_isRun = true;


    CreateEditorUI<MainMenuUI>("MainMenu");
    CreateEditorUI<OutLinerUI>("OutLiner");
    CreateEditorUI<InspectorUI>("Inspector");
    CreateEditorUI<TileMapUI>("Room_Editor");

    
    return true;
}

void EditorEngine::Destroy()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void EditorEngine::Logic()
{
    float deltaTime = TimeManager::Instance().Tick();

    Tick(deltaTime);
    Collision(deltaTime);
    Render(deltaTime);
}

void EditorEngine::Tick(float deltaTime)
{
    GameEngine::Instance().Tick(deltaTime);

    //todo : editor UI begin tick 

    //todo : end
}

void EditorEngine::Collision(float deltaTime)
{
    GameEngine::Instance().Collision(deltaTime);
}

void EditorEngine::Render(float deltaTime)
{
    Device::Instance().ClearBackBuffer(_clearColor);
    Device::Instance().ClearDepthStencil(1.f, 0);
    Device::Instance().SetTarget();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow(); // Show demo window! :)

    GameEngine::Instance().GetWorld()->Render(deltaTime);
    RenderManager::Instance().Render(deltaTime);
    GameEngine::Instance().GetWorld()->RenderUI(deltaTime);

    //todo : editorUI begin Render

    ImGui::Begin("Test");

    ImGui::Text("TEST ..! ");

    ImGui::End();

    for (auto& it : _edits)
    {
        if (!it.second->IsEnable())
            continue;

        it.second->Render(deltaTime);
    }
        
    //end render

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    Device::Instance().Render();
}

bool EditorEngine::Run()
{
    MSG msg = {};

    while (g_isRun)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Logic();
        }
    }


    return (int)msg.wParam;
}

void EditorEngine::Show(ePopUpType type, const std::string& msg)
{
    _popUp->Show(type, msg);
}

void EditorEngine::ShowError(const std::string& msg)
{
    _popUp->ShowError(msg);
}

void EditorEngine::ShowWarning(const std::string& msg)
{
    _popUp->ShowWarning(msg);

}

void EditorEngine::ShowInfo(const std::string& msg)
{
    _popUp->ShowInfo(msg);

}

void EditorEngine::ShowConfirm(const std::string& msg)
{
    _popUp->ShowConfirm(msg);
}
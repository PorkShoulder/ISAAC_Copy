#include "pch.h"
#include "EditorEngine.h"
#include "EditorTool.h"
// UI 목록
#include "TileMapUI.h"



namespace
{
    struct FEditorTab
    {
        const char* name;
        eActorType type;
    };
    const FEditorTab EditorTabs[] =
    {
        { "Tile", eActorType::Tile },

        
        
    };
}

EditorTool::EditorTool()
{
}

EditorTool::~EditorTool()
{
}

bool EditorTool::Init(const std::string& name)
{
    EditorUI::Init(name);
    SetEnable(false);
    // UI 초기화 및 등록
    RegisterMode<TileMapUI>(eActorType::Tile, "TileEdit");
    
    
    
    return true;
		
}

void EditorTool::Render(float deltaTime)
{
    if (!IsOpen())
        return;

    bool open = BeginWindow();

    if (open)
    {
        if (ImGui::BeginTabBar("##EditorTabs")) // 탭 생성 ##nn -> 이름없이 내부 ID만주는 문자열
        {
            for (const auto& tab : EditorTabs)  // 초기화 해둔 배열 순회 
            {
                if (ImGui::BeginTabItem(tab.name))
                {
                    _mode = tab.type;
                    ImGui::EndTabItem();
                }
            }

            ImGui::EndTabBar();
        }

        ImGui::Separator();

        auto it = _findUIs.find(_mode);
        if (it != _findUIs.end() && it->second)
        {
            it->second->Render(deltaTime);
        }
    }

    EndWindow();
}

void EditorTool::Destroy()
{
    for (auto& [key, ui] : _findUIs)
        if (ui) ui->Destroy();
}

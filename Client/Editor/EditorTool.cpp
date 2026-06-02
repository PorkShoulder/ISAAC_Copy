#include "pch.h"
#include "EditorEngine.h"
#include "EditorTool.h"
// UI 목록
#include "TileMapUI.h"
#include "RoomObjectUI.h"


namespace
{
    struct FEditorTab
    {
        const char* name;
        eActorType type;
    };
    // 탭 등록용.
    const FEditorTab EditorTabs[] = 
    {
        { "Tile", eActorType::Tile },
        { "Object", eActorType::Obstacle},
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
    RegisterMode<RoomObjectUI>(eActorType::Obstacle, "RoomObjectEdit");
    
    
    return true;
		
}

void EditorTool::Render(float deltaTime)
{
    if (!IsOpen())
        return;

    bool open = BeginWindow();

    if (open)
    {
        if (ImGui::BeginTabBar("##EditorTabs"))
        {
            for (const auto& tab : EditorTabs)
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
            // 타일맵 전달
            Ptr<RoomEditor> roomUI = Cast<EditorUI, RoomEditor>(it->second);
            if (roomUI && _editingTileMap)
                roomUI->SetTargetTileMap(_editingTileMap);

            it->second->Render(deltaTime);

            // 타일맵 회수
            if (roomUI)
                _editingTileMap = roomUI->GetTargetTileMap();
        }
    }

    EndWindow();
}

void EditorTool::Destroy()
{
    for (auto& [key, ui] : _findUIs)
        if (ui) ui->Destroy();
}

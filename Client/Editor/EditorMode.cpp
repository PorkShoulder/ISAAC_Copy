#include "pch.h"
#include "EditorEngine.h"
#include "EditorMode.h"
#include "TileMapUI.h"
#include "ObstacleUI.h"
#include "DoorUI.h"
#include "ItemUI.h"


EditorMode::EditorMode()
{
}

EditorMode::~EditorMode()
{
}

bool EditorMode::Init(const std::string& name)
{
    EditorUI::Init(name);
    SetEnable(false);

    _findUIs[eActorType::Tile] = New<TileMapUI>();
    _findUIs[eActorType::Tile]->Init("TileEdit");

    _findUIs[eActorType::Obstacle] = New<ObstacleUI>();
    _findUIs[eActorType::Obstacle]->Init("ObstacleEdit");

    _findUIs[eActorType::Door] = New<DoorUI>();
    _findUIs[eActorType::Door]->Init("DoorEdit");
    
    _findUIs[eActorType::Item] = New<ItemUI>();
    _findUIs[eActorType::Item]->Init("ItemEdit");

    return true;
		
}

void EditorMode::Render(float deltaTime)
{
	bool open = BeginWindow();

	if (open) 
	{
        if (ImGui::BeginTabBar("##EditorTabs"))
        {
            if (ImGui::BeginTabItem("Tile"))
            {
                _mode = eActorType::Tile; ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Player"))
            {
                _mode = eActorType::Player; ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Monster"))
            {
                _mode = eActorType::Monster; ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Item"))
            {
                _mode = eActorType::Item; ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Npc"))
            {
                _mode = eActorType::Npc; ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Obstacle"))
            {
                _mode = eActorType::Obstacle; ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Door"))
            {
                _mode = eActorType::Door; ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::Separator();
        auto it = _findUIs.find(_mode);
        if (it != _findUIs.end() && it->second)
            it->second->Render(deltaTime);
	}
	EndWindow();
}

void EditorMode::Destroy()
{
    for (auto& [key, ui] : _findUIs)
        if (ui) ui->Destroy();
}

#include "pch.h"
#include "TileMapUI.h"
#include "EditorEngine.h"
#include "InspectorUI.h"

#include "../Object/TileMap.h"
#include "../Object/Actor.h"

#include "../Core/GameEngine.h"


#include "../World/Level.h"
#include "../World/World.h"

TileMapUI::TileMapUI()
{
}

TileMapUI::~TileMapUI()
{
}

bool TileMapUI::Init(const std::string& name)
{
	EditorUI::Init(name);
	SetEnable(false);

	return true;
}

void TileMapUI::Render(float deltaTime)
{
	EditorUI::Render(deltaTime);

	ImGui::Begin("TileMap_Editor");

	// 여기에 위젯 추가
	ImGui::SeparatorText("Create TileMap");
	ImGui::DragInt("CountX", &_countX);
	ImGui::DragInt("CountY", &_countY);
	ImGui::DragFloat2("TileSize", _tileSize);

	if (ImGui::Button("Create"))
	{
		// TileMap Actor를 찾아서 CreateTile() 호출
		Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
		if (level)
		{
			auto& actors = level->GetActors();
			for (auto& it : actors)
			{
				Ptr<TileMap> tilemap = Cast<Actor, TileMap>(it.second);
				if (tilemap)
				{
					// tilemap에서 TileComponent 가져와서 CreateTile 호출
					break;
				}
			}
		}
	}
	ImGui::End();
}

void TileMapUI::Destroy()
{
}

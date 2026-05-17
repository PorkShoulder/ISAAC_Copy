#include "pch.h"
#include "TileMapUI.h"
#include "EditorEngine.h"
#include "InspectorUI.h"

#include "../Object/TileMap.h"
#include "../Object/Actor.h"

#include "../Core/GameEngine.h"

#include "../Component/TileComponent.h"

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

	ImGui::Begin("Room_Editor");
    //
    Ptr<InspectorUI> inspector = EditorEngine::Instance().FindEditorUI<InspectorUI>("Inspector");
    if (inspector)
    {
        Ptr<Object> target = inspector->GetTarget();
        Ptr<TileMap> selectedTileMap = Cast<Object, TileMap>(target);

        if (selectedTileMap && selectedTileMap != _targetTileMap)
        {
            _targetTileMap = selectedTileMap;

            Ptr<TileComponent> tileComp = _targetTileMap->GetTileComponent();
            if (tileComp)
            {
                _countX = tileComp->GetTileCountX();
                _countY = tileComp->GetTileCountY();
                _tileSize[0] = tileComp->GetTileSize()._x;
                _tileSize[1] = tileComp->GetTileSize()._y;
            }
        }
    }
    
	// 여기에 위젯 추가
	ImGui::SeparatorText("Create Room");
    ImGui::DragInt("CountX", &_countX, 1.f, 1, 100);
    ImGui::DragInt("CountY", &_countY, 1.f, 1, 100);
    ImGui::DragFloat2("TileSize", _tileSize, 1.f, 1.f, 512.f);
    // UI 버튼 사이즈 조절.
    ImVec2 buttonSize(120.f, 28.f);

    // UI키 
    if (ImGui::Button("New_Room", buttonSize))
    {
        // 현재 Level을 가져온다
        Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
        if (level)
        {
            if (_countX <= 0 || _countY <= 0)
                return;

            if (_tileSize[0] <= 0.f || _tileSize[1] <= 0.f)
                return;

            // TileMap Actor의 생성 위치, 크기, 회전 설정
            FVector3D pos(0, 0, 1);
            FVector3D scale(1, 1, 1);
            FRotator rot(0, 0, 1);

            // Level에 TileMap Actor를 새로 생성하고, 생성된 객체를 받아온다
            Ptr<TileMap> tilemap = level->SpawnActor<TileMap>("Room_Editor", pos, scale, rot);
            if (tilemap)
            {
                // 생성된 TileMap에서 TileComponent를 가져온다
                Ptr<TileComponent> tileComp = tilemap->GetTileComponent();

                // UI에서 설정한 값(_countX, _countY, _tileSize)으로 타일을 생성한다
                tileComp->CreateTile(_countX, _countY, FVector2D(_tileSize[0], _tileSize[1]), 0);
                
                // AddTileFrame 없으면 렌더링 안됨 — 텍스처의 어디를 그릴지 지정해야 함
                tileComp->AddTileFrame(0.f, 0.f, 52.f, 52.f);
                tileComp->AddTileFrame(52.f, 0.f, 52.f, 52.f);
                tileComp->AddTileFrame(104.f, 0.f, 52.f, 52.f);
                tileComp->AddTileFrame(156.f, 0.f, 52.f, 52.f);
                tileComp->AddTileFrame(208.f, 0.f, 52.f, 52.f);
            }
        }
    }
    // 가로로 나란히
    ImGui::SameLine();
    // 선택버튼
    if (ImGui::Button("Apply Selected", buttonSize))
    {
        if (_targetTileMap)
        {
            Ptr<TileComponent> tileComp = _targetTileMap->GetTileComponent();
            if (tileComp)
            {
                tileComp->CreateTile(_countX, _countY, FVector2D(_tileSize[0], _tileSize[1]), 0);
            }
        }
    }
    // 가로로 나란히
    ImGui::SameLine();
    // 삭제 버튼
    if(ImGui::Button("Delete Selected", buttonSize))
    {
        if(_targetTileMap)
        {
            Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
            if (level)
            {
                // RemoveActor에 넣어뒀다가 한틱 차이로 처리 
                level->RemoveActor(_targetTileMap->GetActorID());
                // Inspector도 처리해주고
                Ptr<InspectorUI> inspector = EditorEngine::Instance().FindEditorUI<InspectorUI>("Inspector");
                if (inspector)
                    inspector->SetTarget(nullptr);
                _targetTileMap = nullptr;
            }
        }
    }

	ImGui::End();
}

void TileMapUI::Destroy()
{
}

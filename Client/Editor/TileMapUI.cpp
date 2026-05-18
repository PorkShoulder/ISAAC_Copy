#include "pch.h"
#include "TileMapUI.h"
#include "EditorEngine.h"
#include "InspectorUI.h"

#include "../Object/TileMap.h"
#include "../Object/Actor.h"

#include "../Core/GameEngine.h"
#include "../Core/Texture.h"

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
    
    /*if (!IsOpen())
        return;*/
    bool EditorOnOff = BeginWindow(); //"Room_Editor"

    if (EditorOnOff)
    {
        // 여기에 위젯 추가
        ImGui::SeparatorText("Create Room");
        ImGui::DragInt("CountX", &_countX, 1.f, 1, 100);
        ImGui::DragInt("CountY", &_countY, 1.f, 1, 100);
        ImGui::DragFloat2("TileSize", _tileSize, 1.f, 1.f, 512.f);
        // UI 버튼 사이즈 조절.
        ImVec2 buttonSize(120.f, 28.f);

        // UI키
        // 1. 방 만들기 
        if (ImGui::Button("New Room", buttonSize))
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
                    _targetTileMap = tilemap;
                    // 생성된 TileMap에서 TileComponent를 가져온다
                    Ptr<TileComponent> tileComp = tilemap->GetTileComponent();

                    // UI에서 설정한 값(_countX, _countY, _tileSize)으로 타일을 생성한다
                    tileComp->CreateTile(_countX, _countY, FVector2D(_tileSize[0], _tileSize[1]), 0);

                }
            }
        }
        
        // 가로로 나란히
        ImGui::SameLine();
        
        // 2.방 선택하기
        if (ImGui::Button("Apply Selected", buttonSize))
        {
            Ptr<InspectorUI> inspector = EditorEngine::Instance().FindEditorUI<InspectorUI>("Inspector");
            if (inspector)
            {
                Ptr<TileMap> selectedTileMap = Cast<Object, TileMap>(inspector->GetTarget());
                if (selectedTileMap)
                {
                    Ptr<TileComponent> tileComp = selectedTileMap->GetTileComponent();
                    if (tileComp)
                    {
                        tileComp->CreateTile(_countX, _countY, FVector2D(_tileSize[0], _tileSize[1]), 0);
                        _targetTileMap = selectedTileMap;
                    }
                }
            }
         
        }

        // 가로로 나란히
        ImGui::SameLine();

        // 3. 방 삭제하기
        if (ImGui::Button("Delete Selected", buttonSize))
        {
            Ptr<InspectorUI> inspector = EditorEngine::Instance().FindEditorUI<InspectorUI>("Inspector");
            if (inspector)
            {
                Ptr<Object> target = inspector->GetTarget();
                Ptr<TileMap> selectedTileMap = Cast<Object, TileMap>(target);

                if (selectedTileMap)
                {
                    Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
                    if (level)
                    {
                        level->RemoveActor(selectedTileMap->GetActorID());
                        inspector->SetTarget(nullptr);

                        if (_targetTileMap == selectedTileMap)
                            _targetTileMap = nullptr;
                    }
                }
            }

        }

        // 4. 프레임 조절.
        ImGui::SeparatorText("Tile Frame");
        ImGui::DragInt("Frame Width", & _frameWidth, 1.f, 1, 512);
        ImGui::DragInt("Frame Height", & _frameHeight, 1.f, 1, 512);
        if (_targetTileMap)
        {
            Ptr<TileComponent> tileComp = _targetTileMap->GetTileComponent();
            if (tileComp)
            {
                Ptr<Texture> tex = tileComp->GetTexture();
                if (tex)
                {
                    const FTextureInfo* texInfo = tex->GetTexture(0);
                    if (texInfo && texInfo->_srv)
                    {
                        ImTextureID texID = (ImTextureID)texInfo->_srv.Get();
                        
                        float texW = (float)texInfo->_width;
                        float texH = (float)texInfo->_height;

                        float previewW = 300.f;
                        float previewH = previewW * (texH / texW);
                        
                        ImVec2 imagePos = ImGui::GetCursorScreenPos();
                        
                        ImGui::Image(texID, ImVec2(previewW, previewH));
                        
                        if (ImGui::IsItemClicked())
                        {
                            ImVec2 mousePos = ImGui::GetMousePos();

                            float relX = mousePos.x - imagePos.x;
                            float relY = mousePos.y - imagePos.y;

                            float actualX = relX * (texW / previewW);
                            float actualY = relY * (texH / previewH);

                            _selectedFrameX = ((int)actualX / _frameWidth) * _frameWidth;
                            _selectedFrameY = ((int)actualY / _frameHeight) * _frameHeight;
                        }
                        if(_selectedFrameX >= 0 && _selectedFrameY >= 0)
                        {
                            ImVec2 uv0(
                                (float)_selectedFrameX / texW, 
                                (float)_selectedFrameY / texH);

                            ImVec2 uv1(
                                (float)(_selectedFrameX + _frameWidth) / texW,
                                (float)(_selectedFrameY + _frameHeight) / texH);

                            ImGui::Text("Selected: %d, %d", _selectedFrameX, _selectedFrameY);
                            ImGui::Image(texID, ImVec2(64.f, 64.f), uv0, uv1);

                            if (ImGui::Button("Add Frame"))
                            {
                                tileComp->AddTileFrame(
                                    (float)_selectedFrameX,
                                    (float)_selectedFrameY,
                                    (float)_frameWidth,
                                    (float)_frameHeight
                                );
                            }

                        }

                    }
                    
                }

            }
        }

    }
	EndWindow();
}

void TileMapUI::Destroy()
{
}

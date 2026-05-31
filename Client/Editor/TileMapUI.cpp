#include "pch.h"
#include "TileMapUI.h"
#include "EditorEngine.h"
#include "InspectorUI.h"

#include "../Object/TileMap.h"
#include "../Object/Actor.h"

#include "../Core/GameEngine.h"
#include "../Core/Texture.h"
#include "../Core/AssetManager.h"
#include "../Core/DirectoryManager.h"

#include "../Component/TileComponent.h"
#include "../Component/CameraComponent.h"

#include "../World/Level.h"
#include "../World/World.h"
#include "../World/RoomManager.h"



#include <algorithm>



TileMapUI::TileMapUI()
{
}

TileMapUI::~TileMapUI()
{
}

bool TileMapUI::Init(const std::string& name)
{
	RoomEditor::Init(name);

	return true;
}
void TileMapUI::Destroy()
{
}

void TileMapUI::Render(float deltaTime)
{
    RenderTextureSelect("ISAAC_Map");


    ImGui::SameLine();
    if (ImGui::Button("Random"))
    {
        Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
        level->GenerateRandomMap();
    }

    // 여기에 위젯 추가
    ImGui::SeparatorText("Create Room");
    ImGui::InputText("Room Name", _objectName, sizeof(_objectName));
    ImGui::DragInt("CountX", &_countX, 1.f, 1, 100);
    ImGui::DragInt("CountY", &_countY, 1.f, 1, 100);
    ImGui::DragFloat2("TileSize", _tileSize, 1.f, 1.f, 512.f);
    // UI 버튼 사이즈 조절.
    ImVec2 buttonSize(80.f, 20.f);


    // 1. 방 만들기 
    if (ImGui::Button("New", buttonSize))
    {
        // 현재 Level을 가져온다
        Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
        if (level)
        {
            if (_countX > 0 && _countY > 0 && _tileSize[0] > 0.f && _tileSize[1] > 0.f)
            {
                // TileMap Actor의 생성 위치, 크기, 회전 설정
                FVector3D pos(0, 0, 1);
                FVector3D scale(1, 1, 1);
                FRotator rot(0, 0, 1);

                // 이름 번호 추가 
                if (strlen(_objectName) == 0)
                    snprintf(_objectName, sizeof(_objectName), "Room_%02d", _nameCounter++);

                // Level에 TileMap Actor를 새로 생성하고, 생성된 객체를 받아온다
                Ptr<TileMap> tilemap = level->SpawnActor<TileMap>(_objectName, pos, scale, rot);
                if (tilemap)
                {
                    _targetTileMap = tilemap;

                    // 생성된 TileMap에서 TileComponent를 가져온다
                    Ptr<TileComponent> tileComp = tilemap->GetTileComponent();

                    // UI에서 설정한 값(_countX, _countY, _tileSize)으로 타일을 생성한다
                    tileComp->CreateTile(_countX, _countY, FVector2D(_tileSize[0], _tileSize[1]), 0);
                    // 이 부분이 빠져있음
                    if (!_selectedTextureName.empty())
                        tileComp->SetTexture(_selectedTextureName, _selectedTexturePath);

                    if (_selectedFrameX >= 0 && _selectedFrameY >= 0)
                    {
                        tileComp->AddTileFrame(
                            (float)_selectedFrameX,
                            (float)_selectedFrameY,
                            (float)_frameWidth,
                            (float)_frameHeight);
                    }
                }
            }
        }
    }

    // 가로로 나란히
    ImGui::SameLine();
    // 2.방 선택하기
    if (ImGui::Button("Apply", buttonSize))
    {
        Ptr<InspectorUI> inspector = EditorEngine::Instance().FindEditorUI<InspectorUI>("Inspector");
        if (inspector)
        {
            Ptr<TileMap> selectedTileMap = Cast<Object, TileMap>(inspector->GetTarget());
            if (selectedTileMap)
            {
                _targetTileMap = selectedTileMap;

                // 선택한 방의 상태를 UI로 불러오기
                Ptr<TileComponent> tileComp = selectedTileMap->GetTileComponent();
                if (tileComp)
                {
                    // 방의 기존 상태를 불러오기
                    _countX = tileComp->GetTileCountX();
                    _countY = tileComp->GetTileCountY();
                    _tileSize[0] = tileComp->GetTileSize()._x;
                    _tileSize[1] = tileComp->GetTileSize()._y;

                    // 렌더 갱신 
                    tileComp->SetTileInstRefresh(true);
                    tileComp->SetTileLineInstRefresh(true);

                    // 그리드 정보 불러오기
                    _gridW = selectedTileMap->GetGridW();
                    _gridH = selectedTileMap->GetGridH();
                    _emptyCells = selectedTileMap->GetEmptyCells();

                    // 선택한 텍스처가 있으면 적용
                    if (_selectedTexture)
                        tileComp->SetTexture(_selectedTexture);


                    // 숨김 상태였다면 다시 보이게
                    selectedTileMap->SetEnable(true);
                    selectedTileMap->SetActive(true);

                }
            }
        }
    }

    // 가로로 나란히
    ImGui::SameLine();

    // 3. 방 삭제하기
    if (ImGui::Button("Delete", buttonSize))
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

    // 4. Save
    if (ImGui::Button("Save", buttonSize))
    {
        OPENFILENAME ofn = {};
        wchar_t filePath[MAX_PATH] = {};

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = GameEngine::Instance().GetHWND();
        ofn.lpstrFilter = L"Room Files\0*.room\0";
        ofn.lpstrFile = filePath;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrDefExt = L"room";
        static std::wstring roomDirW;
        auto resPath = DirectoryManager::Instance().GetCachePath("Resources");
        if (resPath.has_value())
        {
            std::filesystem::path roomDir;
            if (DirectoryManager::Instance().GetDirectory(resPath.value(), "Room", roomDir))
                roomDirW = roomDir.wstring();
        }
        if (!roomDirW.empty())
            ofn.lpstrInitialDir = roomDirW.c_str();

        ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        if (GetSaveFileName(&ofn))
        {
            std::ofstream file(filePath, std::ios::binary);
            if (file.is_open())
            {
                Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
                const auto& actors = level->GetActors();

                // 변경 — 선택된 TileMap 1개만 저장
                int32 roomCount = 1;
                file.write((char*)&roomCount, sizeof(int32));


                if (_targetTileMap)
                    _targetTileMap->SetEmptyCells(_emptyCells);
                _targetTileMap->Save(file);

                file.close();
            }
        }
    }

    ImGui::SameLine();

    // 5. Load
    if (ImGui::Button("Load", buttonSize))
    {
        OPENFILENAME ofn = {};
        wchar_t filePath[MAX_PATH] = {};

        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = GameEngine::Instance().GetHWND();
        ofn.lpstrFilter = L"Room Files\0*.room\0";
        ofn.lpstrFile = filePath;
        ofn.nMaxFile = MAX_PATH;

        // 초기 폴더를 Resources/Room으로 설정
        static std::wstring roomDirW;
        auto resPath = DirectoryManager::Instance().GetCachePath("Resources");
        if (resPath.has_value())
        {
            std::filesystem::path roomDir;
            if (DirectoryManager::Instance().GetDirectory(resPath.value(), "Room", roomDir))
                roomDirW = roomDir.wstring();
        }
        if (!roomDirW.empty())
            ofn.lpstrInitialDir = roomDirW.c_str();

        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileName(&ofn))
        {
            std::ifstream file(filePath, std::ios::binary);
            if (file.is_open())
            {
                Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();

                int32 roomCount = 0;
                file.read((char*)&roomCount, sizeof(int32));

                for (int32 i = 0; i < roomCount; ++i)
                {
                    FVector3D pos(0, 0, 1);
                    FVector3D scale(1, 1, 1);
                    FRotator rot(0, 0, 0);

                    Ptr<TileMap> room = level->SpawnActor<TileMap>("Room_Editor", pos, scale, rot);
                    if (room)
                        room->Load(file);
                }
                file.close();
            }
        }
    }
    // 빈칸 확인용
    ImGui::SeparatorText("Grid Info");
    ImGui::InputInt("Grid W", &_gridW);
    ImGui::InputInt("Grid H", &_gridH);

    // 빈 칸 토글 (2x2 기준)
    if (_gridW > 1 || _gridH > 1)
    {
        for (int32 y = 0; y < _gridH; ++y)
        {
            for (int32 x = 0; x < _gridW; ++x)
            {
                // 빈 칸인지 체크
                bool isEmpty = false;
                for (auto& e : _emptyCells)
                    if (e.first == x && e.second == y)
                    {
                        isEmpty = true; break;
                    }

                ImGui::PushID(y * _gridW + x);
                if (ImGui::Checkbox("##cell", &isEmpty))
                {
                    if (isEmpty)
                        _emptyCells.push_back({ x, y });
                    else
                        _emptyCells.erase(
                            std::remove(_emptyCells.begin(), _emptyCells.end(),
                                std::make_pair(x, y)), _emptyCells.end());
                }
                ImGui::PopID();

                if (x < _gridW - 1) ImGui::SameLine();
            }
        }
    }
    ////////////////////////////
    // 6. 프레임 추가.
    RenderTexturePreview();

    // Add Frame / Registered Frames (TileMapUI 전용)
    Ptr<TileComponent> tileComp = _targetTileMap ? _targetTileMap->GetTileComponent() : nullptr;

    if (tileComp && _selectedTexture && _selectedFrameX >= 0 && _selectedFrameY >= 0)
    {
        const FTextureInfo* texInfo = _selectedTexture->GetTexture(0);
        if (texInfo && texInfo->_srv)
        {
            ImTextureID texID = (ImTextureID)texInfo->_srv.Get();
            float texW = (float)texInfo->_width;
            float texH = (float)texInfo->_height;

            // 등록 프레임 미리보기
            ImGui::SameLine();
            if (_selectedFrameIndex >= 0 && _selectedFrameIndex < tileComp->GetTileFrameCount())
            {
                const FAnimationFrame& selFrame = tileComp->GetTileFrame(_selectedFrameIndex);
                ImVec2 pUv0(selFrame._start._x / texW, selFrame._start._y / texH);
                ImVec2 pUv1((selFrame._start._x + selFrame._size._x) / texW,
                    (selFrame._start._y + selFrame._size._y) / texH);
                if (_flipX) { float t = pUv0.x; pUv0.x = pUv1.x; pUv1.x = t; }
                if (_flipY) { float t = pUv0.y; pUv0.y = pUv1.y; pUv1.y = t; }
                ImGui::Image(texID, ImVec2(48.f, 48.f), pUv0, pUv1);
            }

            // Add Frame
            if (ImGui::Button("Add Frame"))
            {
                int startGX = _selectedFrameX;
                int startGY = _selectedFrameY;
                int endGX = (int)_dragEndTex._x;
                int endGY = (int)_dragEndTex._y;
                if (endGX <= startGX) endGX = startGX + _frameWidth;
                if (endGY <= startGY) endGY = startGY + _frameHeight;
                for (int gy = startGY; gy < endGY; gy += _frameHeight)
                {
                    for (int gx = startGX; gx < endGX; gx += _frameWidth)
                    {
                        tileComp->AddTileFrame(
                            (float)gx, (float)gy,
                            (float)_frameWidth, (float)_frameHeight);
                    }
                }
                _hasDragSelection = false;
            }

            ImGui::SeparatorText("Registered Frames");
            int32 frameCount = tileComp->GetTileFrameCount();
            if (frameCount > 0)
            {
                ImGui::Text("Frame Count: %d", frameCount);
                if (ImGui::BeginListBox("##FrameList", ImVec2(-1, 100)))
                {
                    int32 removeIdx = -1;
                    for (int32 i = 0; i < frameCount; ++i)
                    {
                        ImGui::PushID(i);
                        bool isSelected = (_selectedFrameIndex == i);
                        char label[32];
                        sprintf_s(label, "Frame [%d]", i);
                        if (ImGui::Selectable(label, isSelected, ImGuiSelectableFlags_AllowOverlap))
                        {
                            _selectedFrameIndex = i;
                            if (_targetTileMap)
                            {
                                _targetTileMap->SetviewFrameIndex(i);
                                _targetTileMap->SetViewFlipX(_flipX);
                                _targetTileMap->SetViewFlipY(_flipY);
                            }
                            if (_targetTileMap && _targetTileMap->HasTileSelection())
                                _targetTileMap->ApplyFrameToSelection(i, _flipX, _flipY);
                        }
                        ImGui::SameLine();
                        if (ImGui::SmallButton("X"))
                            removeIdx = i;
                        ImGui::PopID();
                    }
                    ImGui::EndListBox();
                    if (removeIdx >= 0)
                    {
                        tileComp->RemoveTileFrame(removeIdx);
                        if (_selectedFrameIndex >= (int32)tileComp->GetTileFrameCount())
                            _selectedFrameIndex = max(0, (int32)tileComp->GetTileFrameCount() - 1);
                    }
                }
            }
        }
    }

}
        
    



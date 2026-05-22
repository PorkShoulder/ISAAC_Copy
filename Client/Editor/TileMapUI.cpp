#include "pch.h"
#include "TileMapUI.h"
#include "EditorEngine.h"
#include "InspectorUI.h"

#include "../Object/TileMap.h"
#include "../Object/Actor.h"

#include "../Core/GameEngine.h"
#include "../Core/Texture.h"
#include "../Core/AssetManager.h"

#include "../Component/TileComponent.h"
#include "../Component/CameraComponent.h"

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

    bool EditorOnOff = BeginWindow(); //"Room_Editor"

    if (EditorOnOff)
    {
        ImGui::SeparatorText("Texture Select");

        // 현재 선택된 텍스처 이름 표시
        ImGui::Text("Selected: %s", _selectedTextureName.empty() ? "None" : _selectedTextureName.c_str());
        ImGui::SameLine();
        // ...Browse->찾아보기 버튼.
        if (ImGui::Button("Browse..."))
        {
            OPENFILENAME ofn = {};
            wchar_t filePath[MAX_PATH] = {};

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = GameEngine::Instance().GetHWND();
            ofn.lpstrFilter = L"PNG Files\0*.png\0All Files\0*.*\0";
            ofn.lpstrFile = filePath;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrInitialDir = L"ISAAC_Map\\room";
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

            if (GetOpenFileName(&ofn))
            {
                // 전체 경로에서 텍스처 상대 경로 추출
                std::wstring fullPath = filePath;

                // 텍스처 이름 = 파일명만
                std::wstring wFileName = fullPath.substr(fullPath.find_last_of(L"\\") + 1);
                std::string fileName(wFileName.begin(), wFileName.end());

                // 상대 경로 추출 (ISAAC_Map 이후 부분)
                std::wstring relativePath;
                size_t pos = fullPath.find(L"ISAAC_Map");
                if (pos != std::wstring::npos)
                    relativePath = fullPath.substr(pos);
                else
                    relativePath = fullPath;

                _selectedTextureName = fileName;
                _selectedTexturePath = relativePath;

                TEXTURE_MANAGER->LoadTexture(fileName, relativePath);
                _selectedTexture = TEXTURE_MANAGER->Findtexture(fileName);
            }

        }

        // 여기에 위젯 추가
        ImGui::SeparatorText("Create Room");
        ImGui::InputText("Room Name", _roomName, sizeof(_roomName));
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


                    // Level에 TileMap Actor를 새로 생성하고, 생성된 객체를 받아온다
                    Ptr<TileMap> tilemap = level->SpawnActor<TileMap>(_roomName, pos, scale, rot);
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
                        
                        // 선택한 텍스처가 있으면 적용
                        if (_selectedTexture)
                            tileComp->SetTexture(_selectedTexture);

                        //// 선택한 프레임이 있으면 최소 1개 등록
                        //if (_selectedFrameX >= 0 && _selectedFrameY >= 0)
                        //{
                        //    tileComp->AddTileFrame(
                        //        (float)_selectedFrameX,
                        //        (float)_selectedFrameY,
                        //        (float)_frameWidth,
                        //        (float)_frameHeight
                        //        );
                        //}
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
            ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

            if (GetSaveFileName(&ofn))
            {
                std::ofstream file(filePath, std::ios::binary);
                if (file.is_open())
                {
                    Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
                    const auto& actors = level->GetActors();

                    // TileMap 개수 세기
                    int32 roomCount = 0;
                    for (auto& [id, actor] : actors)
                    {
                        if (Cast<Actor, TileMap>(actor))
                            roomCount++;
                    }
                    file.write((char*)&roomCount, sizeof(int32));

                    // 각 방 저장
                    for (auto& [id, actor] : actors)
                    {
                        Ptr<TileMap> room = Cast<Actor, TileMap>(actor);
                        if (room)
                            room->Save(file);
                    }
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

        ////////////////////////////
        // 6. 프레임 추가.
        ImGui::SeparatorText("Tile Frame");
        // 프레임 하나의 크기 조절
        ImGui::DragInt("Frame Width", &_frameWidth, 1.f, 1, 512);
        ImGui::DragInt("Frame Height", &_frameHeight, 1.f, 1, 512);

        // 선택된 타일맵이 있을때만 미리보기 표시 -> To do텍스처 선택으로 변경예정
        if (_targetTileMap)
        {   // 타일맵에서 TileComponent를 가져온다
            Ptr<TileComponent> tileComp = _targetTileMap->GetTileComponent();
            if (tileComp)
            {   // TileComponent가 사용하는 텍스처를 가져온다.
                Ptr<Texture> tex = tileComp->GetTexture();
                if (tex)
                {   // 텍스처의 첫번째 이미지 정보 (SRV, 크기 등)
                    const FTextureInfo* texInfo = tex->GetTexture(0);
                    if (texInfo && texInfo->_srv)
                    {   // DX11dml SRV포인터를 ImGui가 쓸 수 있는 ImTexture로 캐스팅
                        ImTextureID texID = (ImTextureID)texInfo->_srv.Get();
                        // 원본 텍스처의 실제 픽셀 크기
                        float texW = (float)texInfo->_width;
                        float texH = (float)texInfo->_height;
                        // 미리보기 이미지 크기
                        float previewW = ImGui::GetContentRegionAvail().x;
                        float previewH = previewW * (texH / texW);
                        // ImGui 윈도우 내에서 이미지가 그려질 좌측 상단 좌표
                        ImVec2 imagePos = ImGui::GetCursorScreenPos();
                        
                        // 텍스처 미리보기 크기로 축소해서 표기
                        ImGui::Image(texID, ImVec2(previewW, previewH));
                        ImGui::SetCursorScreenPos(imagePos);
                        ImGui::InvisibleButton("##texPreviewDrag", ImVec2(previewW, previewH));

                        // 이미지 위에 그리드 그리기
                        ImDrawList* drawList = ImGui::GetWindowDrawList();
                        float scaleX = previewW / texW;
                        float scaleY = previewH / texH;

                        // 그리드 색상
                        ImU32 gridColor = IM_COL32(0, 255, 0, 180);
                        //세로선
                        for (float x = 0.f; x <= texW; x += _frameWidth)
                        {
                            float screenX = imagePos.x + x * scaleX;
                            drawList->AddLine(
                                ImVec2(screenX, imagePos.y),
                                ImVec2(screenX, imagePos.y + previewH),
                                gridColor,
                                1.f
                            );
                        }
                        // 가로선
                        for (float y = 0.f; y <= texH; y += _frameHeight)
                        {
                            float screenY = imagePos.y + y * scaleY;

                            drawList->AddLine(
                                ImVec2(imagePos.x, screenY),
                                ImVec2(imagePos.x + previewW, screenY),
                                gridColor,
                                1.f
                            );
                        }
                        //프레임 강조
                        if (_selectedFrameX >= 0 && _selectedFrameY >= 0)
                        {
                            ImVec2 rectMin(
                                imagePos.x + _selectedFrameX * scaleX,
                                imagePos.y + _selectedFrameY * scaleY
                            );

                            ImVec2 rectMax(
                                imagePos.x + (_selectedFrameX + _frameWidth) * scaleX,
                                imagePos.y + (_selectedFrameY + _frameHeight) * scaleY
                            );

                            drawList->AddRect(rectMin, rectMax, IM_COL32(255, 0, 0, 255), 0.f, 0, 2.f);
                        }

                        // 이미지 위에서 마우스 입력 처리 (클릭 / 드래그)
                        if (ImGui::IsItemHovered())
                        {
                            ImVec2 mousePos = ImGui::GetMousePos();
                            float relX = mousePos.x - imagePos.x;
                            float relY = mousePos.y - imagePos.y;

                            float texX = relX * (texW / previewW);
                            float texY = relY * (texH / previewH);
                            // 미리보기 좌표 → 원본 텍스처 좌표 변환
                            //float texMouseX = (mousePos.x - imagePos.x) * (texW / previewW);
                            //float texMouseY = (mousePos.y - imagePos.y) * (texH / previewH);

                            // 드래그 시작
                            if (ImGui::IsMouseClicked(0))
                            {
                                _isDragging = true;
                                _dragStartTex = FVector2D(texX, texY);
                                _dragEndTex = _dragStartTex;
                            }

                            // 드래그 중 - 끝점 갱신
                            if (_isDragging && ImGui::IsMouseDown(0))
                            {
                                _dragEndTex = FVector2D(texX, texY);
                            }
                        }

                        // 마우스 릴리즈 (Hovered 밖에서도 감지)
                        if (_isDragging && ImGui::IsMouseReleased(0))
                        {
                            _isDragging = false;

                            // 드래그 영역 계산 (min/max 정리)
                            float minX = min(_dragStartTex._x, _dragEndTex._x);
                            float minY = min(_dragStartTex._y, _dragEndTex._y);
                            float maxX = max(_dragStartTex._x, _dragEndTex._x);
                            float maxY = max(_dragStartTex._y, _dragEndTex._y);

                            float dragW = maxX - minX;
                            float dragH = maxY - minY;

                            // 3픽셀 이하면 클릭으로 판정 → 기존 그리드 스냅
                            if (dragW < 3.f && dragH < 3.f)
                            {
                                _selectedFrameX = ((int)_dragStartTex._x / _frameWidth) * _frameWidth;
                                _selectedFrameY = ((int)_dragStartTex._y / _frameHeight) * _frameHeight;
                            }
                            else
                            {
                                // 드래그 → 드래그 범위 내에서 그리드 선택
                                _selectedFrameX = ((int)minX / _frameWidth) * _frameWidth;
                                _selectedFrameY = ((int)minY / _frameHeight) * _frameHeight;
                                _dragEndTex._x = (float)(((int)maxX / _frameWidth+1) * _frameWidth);
                                _dragEndTex._y = (float)(((int)maxY / _frameHeight+1) * _frameHeight);
                                _dragStartTex._x = (float)_selectedFrameX;
                                _dragStartTex._y = (float)_selectedFrameY;
                                _hasDragSelection = true; // 드래그 상태 
                            }
                        }
                        float snapMinX, snapMinY, snapMaxX, snapMaxY;
                        // 드래그 중이면 드래그 영역 표시 (파란 사각형)
                        if (_isDragging || _hasDragSelection)
                        {
                            // 드래그 중: 마우스 위치 그대로 표시 (스냅 없음)
                            snapMinX = min(_dragStartTex._x, _dragEndTex._x);
                            snapMinY = min(_dragStartTex._y, _dragEndTex._y);
                            snapMaxX = max(_dragStartTex._x, _dragEndTex._x);
                            snapMaxY = max(_dragStartTex._y, _dragEndTex._y);
                        }
                        else 
                        {
                            // 드래그 완료 후 유지: 이미 스냅된 값 사용
                            snapMinX = _dragStartTex._x;
                            snapMinY = _dragStartTex._y;
                            snapMaxX = _dragEndTex._x;
                            snapMaxY = _dragEndTex._y;
                        }
                        ImVec2 dragRectMin(imagePos.x + snapMinX * scaleX, imagePos.y + snapMinY * scaleY);
                        ImVec2 dragRectMax(imagePos.x + snapMaxX * scaleX, imagePos.y + snapMaxY * scaleY);
                        drawList->AddRect(dragRectMin, dragRectMax, IM_COL32(0, 120, 255, 255), 0.f, 0, 2.f);
                        drawList->AddRectFilled(dragRectMin, dragRectMax, IM_COL32(0, 120, 255, 40));
                        
                        // 
                        if(_selectedFrameX >= 0 && _selectedFrameY >= 0)
                        {
                            ImVec2 uv0(
                                (float)_selectedFrameX / texW,
                                (float)_selectedFrameY / texH);

                            ImVec2 uv1(
                                (float)(_selectedFrameX + _frameWidth) / texW,
                                (float)(_selectedFrameY + _frameHeight) / texH);

                            // 선택프레임 | 등록프레임
                            ImGui::Text("Selected: %d, %d", _selectedFrameX, _selectedFrameY);
                            ImGui::Image(texID, ImVec2(64.f, 64.f), uv0, uv1);

                            //AddFrame 옆에 등록된 프레임 미리보기
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

                            // 프레임 추가 하기
                            if (ImGui::Button("Add Frame"))
                            {
                                // 드래그 범위 내의 그리드 셀을 각각 개별 프레임으로 등록
                                int startGX = _selectedFrameX;
                                int startGY = _selectedFrameY;
                                int endGX = (int)_dragEndTex._x;
                                int endGY = (int)_dragEndTex._y;
                                
                                // 드래그 범위가 없으면 (클릭 선택) 현재 셀 1개만
                                if (endGX <= startGX) endGX = startGX + _frameWidth;
                                if (endGY <= startGY) endGY = startGY + _frameHeight;

                                for (int gy = startGY; gy < endGY; gy += _frameHeight)
                                {
                                    for (int gx = startGX; gx < endGX; gx += _frameWidth)
                                    {
                                        tileComp->AddTileFrame(
                                            (float)gx,
                                            (float)gy,
                                            (float)_frameWidth,
                                            (float)_frameHeight);
                                    }
                                }
                                _hasDragSelection = false; // 등록후 드래그 범위 제거
                            }

                            


                            ImGui::SameLine();
                            // Flip X 토글 버튼
                            if (_flipX)
                                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
                            else
                                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
                            if (ImGui::Button("Flip X"))
                                _flipX = !_flipX;
                            ImGui::PopStyleColor();

                            ImGui::SameLine();

                            // Flip Y 토글 버튼
                            if (_flipY)
                                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
                            else
                                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
                            if (ImGui::Button("Flip Y"))
                                _flipY = !_flipY;
                            ImGui::PopStyleColor();
                           

                            
                            ImGui::SeparatorText("Registered Frames");
                            if (_targetTileMap)
                            {
                                int32 frameCount = tileComp->GetTileFrameCount();
                                // --- 프레임 목록 (리스트박스) ---
                                if (frameCount > 0)
                                {
                                    // 리스트박스: 프레임 번호만 텍스트로 나열
                                    ImGui::Text("Frame Count: %d", frameCount);

                                    if (ImGui::BeginListBox("##FrameList", ImVec2(-1, 100)))
                                    {
                                        int32 removeIdx = -1; // 삭제할 인덱스 저장
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
                                                if (_targetTileMap->HasTileSelection())
                                                    _targetTileMap->ApplyFrameToSelection(i, _flipX, _flipY);
                                            }
                                            // 삭제 버튼 (같은 줄)
                                            ImGui::SameLine();
                                            if (ImGui::SmallButton("X"))
                                            {
                                                removeIdx = i; // 루프 안에서 삭제하지 않고 저장만.
                                            }
                                            ImGui::PopID();
                                        }
                                        ImGui::EndListBox();

                                        //루프가 끝난 뒤 삭제 처리
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
                }
            }
        }
    }
	EndWindow();
}

void TileMapUI::Destroy()
{

}


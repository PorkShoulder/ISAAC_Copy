#include "pch.h"
#include "RoomEditor.h"
#include "EditorEngine.h"


#include "../Core/DirectoryManager.h"
#include "../Core/AssetManager.h"
#include "../Core/Texture.h"

// RoomEditor.cpp 상단에 정의
bool RoomEditor::_showCollider = true;
RoomEditor::RoomEditor()
{
}

RoomEditor::~RoomEditor()
{
}

void RoomEditor::RenderSnapOption() //스냅
{
    ImGui::Checkbox("Show Collder", &_showCollider);

    ImGui::Checkbox("snap to Grid", &_snapGrid);
    if (_snapGrid)
    {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(80.f);
        ImGui::InputFloat("Grid Size", &_gridSize, 1.f, 10.f, "%.0f");
    }
}

void RoomEditor::RenderTextureSelect(const std::string& defaultFolder)
{

    ImGui::SeparatorText("Texture Select");

    // 갱신이 필요하면
    if (ImGui::Button("Refresh"))
        _needRefresh = true;

    // 텍스처 찾기
    if (_needRefresh)
    {
        _texFileList.clear();
        _texFolderList.clear();
        _texFolderList.push_back("All");

        std::set<std::string> folders;
        auto resPath = DirectoryManager::Instance().GetCachePath("Resources");
        if (resPath.has_value())
        {
            std::filesystem::path texDir;
            if (DirectoryManager::Instance().GetDirectory(resPath.value(), "Texture", texDir))
            {
                for (auto& entry : std::filesystem::recursive_directory_iterator(texDir))
                {
                    // .png 아니면 무시합니다.
                    if (entry.is_regular_file() && entry.path().extension() == L".png")
                    {
                        auto relPath = std::filesystem::relative(entry.path(), texDir);
                        auto folder = relPath.parent_path().string();
                        if (!defaultFolder.empty())
                        {   
                            std::string prefix = defaultFolder + "\\";
                            
                            bool isSameFolder = (folder == defaultFolder);
                            bool isChildFolder = (folder.find(prefix) == 0);
                            if (!isSameFolder && !isChildFolder)
                                continue;
                        }

                        if (!folder.empty())
                            folders.insert(folder);
                        _texFileList.push_back(relPath.string());
                    }
                }
            }

        }
        for (auto& fPath : folders)
            _texFolderList.push_back(fPath);
        
        // 기본 폴더가 지정되어 있으면 해당 폴더로 자동 선택
        _selectedFolder = 0;
        if (!defaultFolder.empty())
        {
            for (int i = 0; i < (int)_texFolderList.size(); i++)
            {
                if (_texFolderList[i] == defaultFolder)
                {
                    _selectedFolder = i;
                    break;
                }
            
            }

        }
        _needRefresh = false;
    }
    // 폴더 필터 
    if (ImGui::BeginCombo("Folder", _texFolderList[_selectedFolder].c_str()))
    {
        for (int i = 0; i < (int)_texFolderList.size(); i++)
        {
            bool isSelected = (_selectedFolder == i);
            if (ImGui::Selectable(_texFolderList[i].c_str(), isSelected))
                _selectedFolder = i;
        }
        ImGui::EndCombo();
    }
    // 텍스처 목록 (필터)
    if (ImGui::BeginListBox("##TexList", ImVec2(-1, 100)))
    {
        for (int i = 0; i < (int)_texFileList.size(); i++)
        {
            // 폴더 필터링
            if (_selectedFolder > 0)
            {
                std::string folder = std::filesystem::path(_texFileList[i]).parent_path().string();
                if (folder != _texFolderList[_selectedFolder])
                    continue;
            }

            bool isSelected = (_texFileList[i] == _selectedTextureName);
            if (ImGui::Selectable(_texFileList[i].c_str(), isSelected))
            {
                _selectedTextureName = _texFileList[i];
                std::wstring wRelPath(_selectedTextureName.begin(), _selectedTextureName.end());
                _selectedTexturePath = wRelPath;

                TEXTURE_MANAGER->LoadTexture(_selectedTextureName, wRelPath);
                _selectedTexture = TEXTURE_MANAGER->Findtexture(_selectedTextureName);
            }
        }
        ImGui::EndListBox();
    }

    ImGui::Text("Selected: %s", _selectedTextureName.empty() ? "None" : _selectedTextureName.c_str());

    if (_selectedTexture == nullptr)
        return;

    // 프레임 크기
    ImGui::DragInt("Frame Width", &_frameWidth, 1.f, 1, 512);
    ImGui::DragInt("Frame Height", &_frameHeight, 1.f, 1, 512);

    // 반전
    ImGui::Checkbox("Flip X", &_flipX);
    ImGui::SameLine();
    ImGui::Checkbox("Flip Y", &_flipY);
}

void RoomEditor::RenderTexturePreview()
{
    // 미리보기 만들기.
    Ptr<Texture> tex = _selectedTexture;
    if (!tex)
        return;

    const FTextureInfo* texInfo = tex->GetTexture(0);
    if (!texInfo || !texInfo->_srv)
        return;

    ImTextureID texID = (ImTextureID)texInfo->_srv.Get();
    float texW = (float)texInfo->_width;
    float texH = (float)texInfo->_height;

    //미리보기 사이즈 
    ImGui::Text("Texture: %.0f x %.0f", texW, texH);
    ImGui::Text("Frame: %d x %d  |  Cols: %d  Rows: %d",
        _frameWidth, _frameHeight,
        (int)(texW / _frameWidth),
        (int)(texH / _frameHeight));
    
    // 미리보기 최대 크기 기준 
    float previewW = ImGui::GetContentRegionAvail().x;
    float previewH = previewW * (texH / texW);

    // 원본 크기보다 커지지 않도록
    if (previewW > texW) previewW = texW;
    if (previewH > texH) previewH = texH;

    // 비율 유지
    previewH = previewW * (texH / texW);

    ImVec2 imagePos = ImGui::GetCursorScreenPos();
    ImGui::Image(texID, ImVec2(previewW, previewH));
    ImGui::SetCursorScreenPos(imagePos);
    ImGui::InvisibleButton("##texPreviewDrag", ImVec2(previewW, previewH));

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    float scaleX = previewW / texW;
    float scaleY = previewH / texH;

    // 그리드
    ImU32 gridColor = IM_COL32(0, 255, 0, 180);
    for (float x = 0.f; x <= texW; x += _frameWidth)
    {
        float screenX = imagePos.x + x * scaleX;
        drawList->AddLine(
            ImVec2(screenX, imagePos.y),
            ImVec2(screenX, imagePos.y + previewH),
            gridColor, 1.f);
    }
    for (float y = 0.f; y <= texH; y += _frameHeight)
    {
        float screenY = imagePos.y + y * scaleY;
        drawList->AddLine(
            ImVec2(imagePos.x, screenY),
            ImVec2(imagePos.x + previewW, screenY),
            gridColor, 1.f);
    }

    // 프레임 강조
    if (_selectedFrameX >= 0 && _selectedFrameY >= 0)
    {
        ImVec2 rectMin(
            imagePos.x + _selectedFrameX * scaleX,
            imagePos.y + _selectedFrameY * scaleY);
        ImVec2 rectMax(
            imagePos.x + (_selectedFrameX + _frameWidth) * scaleX,
            imagePos.y + (_selectedFrameY + _frameHeight) * scaleY);
        drawList->AddRect(rectMin, rectMax, IM_COL32(255, 0, 0, 255), 0.f, 0, 2.f);
    }

    // 클릭 / 드래그
    if (ImGui::IsItemHovered())
    {
        ImVec2 mousePos = ImGui::GetMousePos();
        float texX = (mousePos.x - imagePos.x) * (texW / previewW);
        float texY = (mousePos.y - imagePos.y) * (texH / previewH);

        if (ImGui::IsMouseClicked(0))
        {
            _isDragging = true;
            _dragStartTex = FVector2D(texX, texY);
            _dragEndTex = _dragStartTex;
        }
        if (_isDragging && ImGui::IsMouseDown(0))
        {
            _dragEndTex = FVector2D(texX, texY);
        }
    }

    if (_isDragging && ImGui::IsMouseReleased(0))
    {
        _isDragging = false;
        float minX = min(_dragStartTex._x, _dragEndTex._x);
        float minY = min(_dragStartTex._y, _dragEndTex._y);
        float maxX = max(_dragStartTex._x, _dragEndTex._x);
        float maxY = max(_dragStartTex._y, _dragEndTex._y);
        float dragW = maxX - minX;
        float dragH = maxY - minY;

        if (dragW < 3.f && dragH < 3.f)
        {
            _selectedFrameX = ((int)_dragStartTex._x / _frameWidth) * _frameWidth;
            _selectedFrameY = ((int)_dragStartTex._y / _frameHeight) * _frameHeight;
        }
        else
        {
            _selectedFrameX = ((int)minX / _frameWidth) * _frameWidth;
            _selectedFrameY = ((int)minY / _frameHeight) * _frameHeight;
            _dragEndTex._x = (float)(((int)maxX / _frameWidth + 1) * _frameWidth);
            _dragEndTex._y = (float)(((int)maxY / _frameHeight + 1) * _frameHeight);
            _dragStartTex._x = (float)_selectedFrameX;
            _dragStartTex._y = (float)_selectedFrameY;
            _hasDragSelection = true;
        }
    }

    // 드래그 영역 표시
    float snapMinX, snapMinY, snapMaxX, snapMaxY;
    if (_isDragging || _hasDragSelection)
    {
        snapMinX = min(_dragStartTex._x, _dragEndTex._x);
        snapMinY = min(_dragStartTex._y, _dragEndTex._y);
        snapMaxX = max(_dragStartTex._x, _dragEndTex._x);
        snapMaxY = max(_dragStartTex._y, _dragEndTex._y);
    }
    else
    {
        snapMinX = _dragStartTex._x;
        snapMinY = _dragStartTex._y;
        snapMaxX = _dragEndTex._x;
        snapMaxY = _dragEndTex._y;
    }
    ImVec2 dragRectMin(imagePos.x + snapMinX * scaleX, imagePos.y + snapMinY * scaleY);
    ImVec2 dragRectMax(imagePos.x + snapMaxX * scaleX, imagePos.y + snapMaxY * scaleY);
    drawList->AddRect(dragRectMin, dragRectMax, IM_COL32(0, 120, 255, 255), 0.f, 0, 2.f);
    drawList->AddRectFilled(dragRectMin, dragRectMax, IM_COL32(0, 120, 255, 40));

    // 선택 프레임 미리보기
    if (_selectedFrameX >= 0 && _selectedFrameY >= 0)
    {
        ImGui::Spacing();
        ImVec2 uv0((float)_selectedFrameX / texW, (float)_selectedFrameY / texH);
        ImVec2 uv1(
            (float)(_selectedFrameX + _frameWidth) / texW,
            (float)(_selectedFrameY + _frameHeight) / texH);

        ImGui::Text("Selected: %d, %d", _selectedFrameX, _selectedFrameY);
        ImGui::Image(texID, ImVec2(64.f, 64.f), uv0, uv1);
    }


}

bool RoomEditor::Init(const std::string& name)
{
    return EditorUI::Init(name);
}

void RoomEditor::Render(float deltaTime)
{
}

void RoomEditor::Destroy()
{
}

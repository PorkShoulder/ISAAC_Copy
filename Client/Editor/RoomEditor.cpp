#include "pch.h"
#include "RoomEditor.h"
#include "EditorEngine.h"

#include "../Core/DirectoryManager.h"
#include "../Core/AssetManager.h"
#include "../Core/Texture.h"

RoomEditor::RoomEditor()
{
}

RoomEditor::~RoomEditor()
{
}

void RoomEditor::RenderSnapOption() //스냅
{

    ImGui::Checkbox("snap to Grid", &_snapGrid);
    if (_snapGrid)
    {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(80.f);
        ImGui::InputFloat("Grid Size", &_gridSize, 1.f, 10.f, "%.0f");
    }
}

void RoomEditor::RenderTextureSelect()
{
    // 텍스처 목록 보여주기
    // 사용자가 하나 선택하기
    // 선택한 텍스처 로드하기
    ImGui::SeparatorText("Texture Select");
    
    static std::vector<std::string> texFileList;
    static bool needRefresh = true;
    // 갱신이 필요하면
    if (ImGui::Button("Refresh"))
        needRefresh = true;

    // 텍스처 찾기
    if (needRefresh)
    {
        texFileList.clear();
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
                        auto rel = std::filesystem::relative(entry.path(), texDir);
                        texFileList.push_back(rel.string());
                    }
                }
            }

        }
        needRefresh = false;
    }
    // 텍스처 목록
    if (ImGui::BeginListBox("##TexList", ImVec2(-1, 100))) //ImVec -> 박사 사이즈 
    {
        for (int i = 0; i < (int)texFileList.size(); i++)
        {
            bool isSelected = (texFileList[i] == _selectedTextureName);
            if (ImGui::Selectable(texFileList[i].c_str(), isSelected))
            {
                _selectedTextureName = texFileList[i];
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

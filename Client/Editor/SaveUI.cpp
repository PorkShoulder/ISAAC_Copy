#include "pch.h"
#include "SaveUI.h"

#include "EditorEngine.h"

#include "Core/DirectoryManager.h"
#include "Core/GameEngine.h"

#include "World/Level.h"
#include "World/World.h"


SaveUI::SaveUI()
{}

SaveUI::~SaveUI()
{}

bool SaveUI::Init(const std::string & name)
{
    EditorUI::Init(name);

    SetEnable(false);

    return true;
}

void SaveUI::Render(float deltaTime)
{
    EditorUI::Render(deltaTime);

    ImGui::Begin("SAVE");

    ImGui::InputText("NewName", _fileName, FILE_NAME_STR);

    if (ImGui::Button("SAVE_BUTTON"))
    {
        Save(_fileName);
        SetEnable(false);
        memset(_fileName, 0, FILE_NAME_STR);
    }

    ImGui::End();
}

void SaveUI::Destroy()
{}

void SaveUI::Save(const std::string& fileName)
{
    // 경로 확인 + 파일 이름 검증
    auto cachePath = DirectoryManager::Instance().GetCachePath("Resources\\Level");
    if (!cachePath.has_value() || fileName.empty())
    {
        EditorEngine::Instance().ShowError("InVaild FileName");
        return;
    }

    // 중복 파일 확인.
    std::filesystem::path fullPath;
    if (DirectoryManager::Instance().GetFile(cachePath.value(), fileName, OUT fullPath))
    {
        //이미 똑같은 이름의 파일이 있다.
        EditorEngine::Instance().ShowError("Duplicate FileName");
        return;
    }
    // 파일 열기
    std::ofstream saveFile(fullPath, std::ios::binary);
    if (saveFile.fail())
    {
        EditorEngine::Instance().ShowError("File Open Error");
        return;
    }
    // Level 확인 + 저장
    Ptr<Level> level = GET_WORLD->GetCurLevel();
    if (!level)
    {
        EditorEngine::Instance().ShowError("InValid Level");
        return;
    }

    level->Save(saveFile);
    
    saveFile.close();
}

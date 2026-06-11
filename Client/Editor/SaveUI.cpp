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
    auto cachePath = DirectoryManager::Instance().GetCachePath("Resources");
    if (!cachePath.has_value() || fileName.empty())
    {
        EditorEngine::Instance().ShowError("InValid FileName");
        return;
    }

    std::filesystem::path levelDir;
    if (!DirectoryManager::Instance().GetDirectory(cachePath.value(), "Level", levelDir))
    {
        EditorEngine::Instance().ShowError("Level Directory Not Found");
        return;
    }

    std::filesystem::path fullPath = levelDir / fileName;
    if (std::filesystem::exists(fullPath))
    {
        EditorEngine::Instance().ShowError("Duplicate FileName");
        return;
    }

    std::ofstream saveFile(fullPath, std::ios::binary);
    if (saveFile.fail())
    {
        EditorEngine::Instance().ShowError("File Open Error");
        return;
    }

    Ptr<Level> level = GET_WORLD->GetCurLevel();
    if (!level)
    {
        EditorEngine::Instance().ShowError("InValid Level");
        return;
    }

    level->Save(saveFile);
    saveFile.close();
}

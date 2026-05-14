#include "pch.h"
#include "LoadUI.h"

#include "EditorEngine.h"

#include "Core/DirectoryManager.h"
#include "Core/GameEngine.h"

#include "World/Level.h"
#include "World/World.h"

LoadUI::LoadUI()
{}

LoadUI::~LoadUI()
{}

bool LoadUI::Init(const std::string& name)
{
    EditorUI::Init(name);

    SetEnable(false);

    return true;
}

void LoadUI::Render(float deltaTime)
{
    ImGui::Begin("SAVE");

    ImGui::InputText("NewName", _fileName, 54);

    if (ImGui::Button("SAVE_BUTTON"))
    {
        Load(_fileName);
        SetEnable(false);
        memset(_fileName, 0, 64);
    }

    ImGui::End();
}

void LoadUI::Destroy()
{

}

void LoadUI::Load(const std::string & fileName)
{
    auto cachePath = DirectoryManager::Instance().GetCachePath("Resources\\Level");
    if (!cachePath.has_value() || fileName.empty())
    {
        EditorEngine::Instance().ShowError("InVaild FileName");
        return;
    }

    std::filesystem::path fullPath;
    if (!DirectoryManager::Instance().GetFile(cachePath.value(), fileName, OUT fullPath))
    {
        //이미 똑같은 이름의 파일이 있다.
        EditorEngine::Instance().ShowError("InValid FileName");
        return;
    }

    std::ifstream loadFile(fullPath, std::ios::binary);
    if (loadFile.fail())
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

    level->Load(loadFile);

    loadFile.close();
}

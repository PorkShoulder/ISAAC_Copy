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
    bool open = true;
    ImGui::Begin("Load", &open);

    // x 버튼!
    if (!open)
    {
        SetEnable(false);
        _selectedIdx - 1;
        ImGui::End();
        return;
    }

    // 창 열릴 때 파일 목록 갱신
    if (_fileList.empty())
        RefreshFileList();
    
    // 새로고침 버튼 
    if (ImGui::Button("Refresh"))
        RefreshFileList();

    ImGui::Separator();
    ImGui::BeginChild("FileList", ImVec2(0, 200), true);
    for(int32 i = 0; i< (int32)_fileList.size(); i++)
    {
        // 선택된 항목 강조 표시
        bool selected = (_selectedIdx == i);
        if (ImGui::Selectable(_fileList[i].c_str(), selected))
        {
            _selectedIdx = i;
            // 선택한 파일명을 입력창에 자동 복사
            memset(_fileName, 0, 64);
            strncpy_s(_fileName, _fileList[i].c_str(), 63);
        }

    
    }
    ImGui::EndChild();
    ImGui::Separator();


    // 파일명 직접 입력하기. 
    ImGui::InputText("NewName", _fileName, 54);

    if (ImGui::Button("Load_BUTTON"))
    {
        Load(_fileName);
        SetEnable(false);
        _selectedIdx =- 1;
        _fileList.clear();
        memset(_fileName, 0, 64);
    }

    ImGui::End();
}

void LoadUI::Destroy()
{

}

void LoadUI::Load(const std::string & fileName)
{
    // 1) Resources 경로 가져오기 + 파일 이름 유효성 검사
    auto cachePath = DirectoryManager::Instance().GetCachePath("Resources");
    if (!cachePath.has_value() || fileName.empty())
    {
        EditorEngine::Instance().ShowError("InValid FileName");
        return;
    }

    // 2) Resources/Level 폴더 존재 확인
    std::filesystem::path levelDir;
    if (!DirectoryManager::Instance().GetDirectory(cachePath.value(), "Level", levelDir))
    {
        EditorEngine::Instance().ShowError("Level Directory Not Found");
        return;
    }

    // 3) 불러올 파일이 존재하는지 확인
    std::filesystem::path fullPath = levelDir / fileName;
    if (!std::filesystem::exists(fullPath))
    {
        EditorEngine::Instance().ShowError("File Not Found");
        return;
    }

    // 4) 바이너리 모드로 파일 열기
    std::ifstream loadFile(fullPath, std::ios::binary);
    if (loadFile.fail())
    {
        EditorEngine::Instance().ShowError("File Open Error");
        return;
    }

    // 5) 현재 레벨에 데이터 로드 (기존 액터 정리 후 TileMap + 액터 복원)
    Ptr<Level> level = GET_WORLD->GetCurLevel();
    if (!level)
    {
        EditorEngine::Instance().ShowError("InValid Level");
        return;
    }

    level->Load(loadFile);
    loadFile.close();
}

void LoadUI::RefreshFileList()
{
    _fileList.clear();
    _selectedIdx = -1;

    // Resources/Level 폴더의 파일 목록 수집
    auto cachePath = DirectoryManager::Instance().GetCachePath("Resources");
    if (!cachePath.has_value())
        return;

    std::filesystem::path levelDir;
    if (!DirectoryManager::Instance().GetDirectory(cachePath.value(), "Level", levelDir))
        return;

    for (auto& entry : std::filesystem::directory_iterator(levelDir))
    {
        if (entry.is_regular_file())
            _fileList.push_back(entry.path().filename().string());
    }
}

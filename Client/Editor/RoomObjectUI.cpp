#include "pch.h"
#include "EditorEngine.h"
#include "RoomObjectUI.h"

namespace
{
    std::vector<std::string> CategoryNames{ "Obstacle", "Door", "Item", "Monster" };

    std::vector<std::string> ObstacleType{ "Rock", "Pit", "Spike" };
    std::vector<std::string> DoorType{ "NextDoor", "LockDoor", "HiddenDoor" };
    std::vector<std::string> ItemType{ "Heart", "Key", "Bomb", "Passive", "Active" };
    std::vector<std::string> MonsterType{ "Fly", "Gaper", "Spider" };

    eActorType CategoryToType[] = {
        eActorType::Obstacle,
        eActorType::Door,
        eActorType::Item,
        eActorType::Monster
    };

    std::vector<std::string>* SubTypeTable[] = {
        &ObstacleType,
        &DoorType,
        &ItemType,
        &MonsterType
    };

    static bool ComboFromVector(const char* label, int* index, const std::vector<std::string>& items)
    {
        std::vector<const char*> ptrs;
        for (auto& s : items)
            ptrs.push_back(s.c_str());
        return ImGui::Combo(label, index, ptrs.data(), (int)ptrs.size());
    }
}

RoomObjectUI::RoomObjectUI()
{
}

RoomObjectUI::~RoomObjectUI()
{
}

bool RoomObjectUI::Init(const std::string& name)
{
    RoomEditor::Init(name);
    _placeType = eActorType::Obstacle;
    _currentIndex = 0;
    return true;
}

void RoomObjectUI::Render(float deltaTime)
{
    // 텍스처 선택 + 미리보기

    RenderTextureSelect();       
    RenderTexturePreview();

    ImGui::Separator();

    // 카테고리 선택
    int categoryIndex = 0;
    for (int i = 0; i < (int)CategoryNames.size(); i++)
    {
        if (CategoryToType[i] == _placeType)
        {
            categoryIndex = i;
            break;
        }
    }

    if (ComboFromVector("Category", &categoryIndex, CategoryNames))
    {
        _placeType = CategoryToType[categoryIndex];
        _currentIndex = 0;
    }

    // 서브타입 선택
    ComboFromVector("Type", &_currentIndex, *SubTypeTable[categoryIndex]);

    ImGui::Separator();

    // 스냅 기능
    RenderSnapOption();


    ImGui::Separator();
    ImGui::Text("Click on tilemap to place object");
}

void RoomObjectUI::Destroy()
{
}
#include "pch.h"
#include "EditorUI.h"
#include "imgui.h"




EditorUI::EditorUI()
{}

EditorUI::~EditorUI()
{}

bool EditorUI::BeginWindow()
{
    if (!_open)
        return false;
    

    return ImGui::Begin(_name.c_str(), &_open);
}

void EditorUI::EndWindow()
{
    
    ImGui::End();

    if(!_open)
        SetEnable(false);
    
}

bool EditorUI::Init(const std::string & name)
{
    _name = name;

    SetActive(true);
    SetEnable(true);
    
    return true;
}

void EditorUI::Tick(float deltaTime)
{}

void EditorUI::Render(float deltaTime)
{}

void EditorUI::Destroy()
{}

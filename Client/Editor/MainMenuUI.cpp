#include "pch.h"
#include "MainMenuUI.h"

#include "EditorEngine.h"
#include "EditorMode.h"
#include "OutLinerUI.h"
#include "InspectorUI.h"
#include "TileMapUI.h"

MainMenuUI::MainMenuUI()
{}

MainMenuUI::~MainMenuUI()
{}

bool MainMenuUI::Init(const std::string & name)
{
    EditorUI::Init(name);

    return true;
}

void MainMenuUI::Render(float deltaTime)
{
    if (ImGui::BeginMainMenuBar())
    {
        File();
        Editor();

        ImGui::EndMainMenuBar();
    }
}

void MainMenuUI::Destroy()
{}

void MainMenuUI::File()
{
    if (ImGui::BeginMenu("File"))
    {

        if (ImGui::MenuItem("New"))
        {
            //todo : func
        }

        if (ImGui::MenuItem("Save"))
        {
            //todo : func
        }

        if (ImGui::MenuItem("Load"))
        {
            //todo : func
        }

        ImGui::EndMenu();
    }
}

void MainMenuUI::Editor()   // To do 타일맵 구현하기.
{
    if (ImGui::BeginMenu("Editor"))
    {
        if (ImGui::MenuItem("OutLiner"))
        {
            Ptr<OutLinerUI> outliner = EditorEngine::Instance().FindEditorUI<OutLinerUI>("OutLiner");
            if (outliner)
            {
                outliner->SetEnable(true);
                outliner->SetOpen(true);
            }
             

        }

        if (ImGui::MenuItem("Inspector"))
        {
            Ptr<InspectorUI> inspector = EditorEngine::Instance().FindEditorUI<InspectorUI>("Inspector");
            if (inspector)
            {
                inspector->SetEnable(true);
                inspector->SetOpen(true);
            
            }    

            
        }

        if (ImGui::MenuItem("EditorMode"))
        {
            Ptr<EditorMode>editor = EditorEngine::Instance().FindEditorUI<EditorMode>("EditorMode");
            if (editor)
            {
                editor->SetEnable(true);
                editor->SetOpen(true);
            }
             
            
        }
        if (ImGui::MenuItem("Save/Load"))
        {

        }




        ImGui::EndMenu();
    }
}

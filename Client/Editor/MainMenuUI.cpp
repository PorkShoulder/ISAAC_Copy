#include "pch.h"
#include "MainMenuUI.h"
#include "EditorEngine.h"
#include "OutLinerUI.h"
#include "InspectorUI.h"
#include "EditorTool.h"

#include "../Core/GameEngine.h"
#include "../World/World.h"
#include "../World/Level.h"
#include "../World/CameraManager.h"
#include "../Component/CameraComponent.h"

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
        Camera();

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

        if (ImGui::MenuItem("EditorTool"))
        {
            Ptr<EditorTool>editor = EditorEngine::Instance().FindEditorUI<EditorTool>("EditorTool");
            if (editor)
            {
                editor->SetEnable(true);
                editor->SetOpen(true);
            }
             
            
        }
        ImGui::EndMenu();
    }
}

void MainMenuUI::Camera() // 카메라 모드 전환
{
    if (ImGui::BeginMenu("Camera"))
    {
        Ptr<Level>  level = GameEngine::Instance().GetWorld()->GetCurLevel();
        if (level)
        {
            Ptr<CameraManager> camMgr = level->GetCameraManager();
            if (camMgr)
            {
                bool on = camMgr->IsEditorCameraMode();
                if (ImGui::MenuItem("Camera Mode", "C", on))   // 체크표시 + 클릭 감지
                    camMgr->SetEditorCameraMode(!on);

                if (camMgr->IsEditorCameraMode())
                {
                    FVector3D pos = camMgr->GetEditorCamPos();
                    if (ImGui::DragFloat3("Cam Pos", &pos._x, 1.f))
                        camMgr->SetEditorCamPos(pos);
                }
            }
        }
        ImGui::EndMenu();
    }
}

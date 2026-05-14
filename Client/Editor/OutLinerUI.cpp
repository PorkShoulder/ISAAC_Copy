#include "pch.h"
#include "OutLinerUI.h"
#include "EditorEngine.h"
#include "InspectorUI.h"

#include "Core/GameEngine.h"

#include "Object/Actor.h"

#include "World/World.h"
#include "World/Level.h"

#include "Component/SceneComponent.h"
#include "Component/ActorComponent.h"

OutLinerUI::OutLinerUI()
{}

OutLinerUI::~OutLinerUI()
{}

bool OutLinerUI::Init(const std::string & name)
{
    EditorUI::Init(name);   // 부모 Init 호출 -> _name 설정, Active/Enable = true;
    SetEnable(false);       // 바로 꺼둠 -> MainMenuUI에서 켜줘야 보임.

    return true;
}

void OutLinerUI::Render(float deltaTime)
{
    EditorUI::Render(deltaTime);

    Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();    // GameEngine -> world -> CurLevel에서 현재 레벨을 가져온다.
    if (nullptr == level)
        return;

    ImGui::Begin(_name.c_str());    //OutLiner 이름의 ImGui 윈도우를 연다.

    auto& actors = level->GetActors();
    for (auto& actor : actors)
        DrawActor(actor.second);

    ImGui::End();
}


void OutLinerUI::Destroy()
{}

void OutLinerUI::DrawActor(Ptr<class Actor> actor) // 
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

    std::string label = std::format("{} | {}", actor->GetActorID(), actor->GetName());
    //std::format("형식 문자열", 값1, 값2.....)

    Ptr<InspectorUI> inspector = EditorEngine::Instance().FindEditorUI<InspectorUI>("Inspector");
    if (!inspector) 
        return;

    if (inspector->GetTarget() == actor)
        flags |= ImGuiTreeNodeFlags_Selected;

    bool open = ImGui::TreeNodeEx(label.c_str(), flags);

    if (ImGui::IsItemClicked())
        inspector->SetTarget(actor);

    if (open)
    {
        DrawSceneComponent(actor->GetRoot());
        ImGui::TreePop();
    }
}

void OutLinerUI::DrawSceneComponent(Ptr<class SceneComponent> comp)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

    std::string label = comp->GetName();

    auto& childs = comp->GetChilds();
    if (childs.size() < 1)
        flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

    bool open = ImGui::TreeNodeEx(label.c_str(), flags);

    Ptr<InspectorUI> inspector = EditorEngine::Instance().FindEditorUI<InspectorUI>("Inspector");
    if (!inspector)
        return;

    if (inspector->GetTarget() == comp)
        flags |= ImGuiTreeNodeFlags_Selected;

    if (ImGui::IsItemClicked())
        inspector->SetTarget(comp);

    if (open && childs.size() > 0)
    {
        for (auto& child : childs)
            DrawSceneComponent(child.second);

        ImGui::TreePop();
    }
}

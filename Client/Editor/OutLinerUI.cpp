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
    EditorUI::Init(name);
    SetEnable(false);

    return true;
}

void OutLinerUI::Render(float deltaTime)
{
    EditorUI::Render(deltaTime);

    Ptr<Level> level = GameEngine::Instance().GetWorld()->GetCurLevel();
    if (nullptr == level)
        return;

    ImGui::Begin(_name.c_str());

    auto& actors = level->GetActors();
    for (auto& actor : actors)
        DrawActor(actor.second);

    ImGui::End();
}


void OutLinerUI::Destroy()
{}

void OutLinerUI::DrawActor(Ptr<class Actor> actor)
{
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

    std::string label = std::to_string(actor->GetActorID()) ;

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

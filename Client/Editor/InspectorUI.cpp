#include "pch.h"
#include "InspectorUI.h"

#include "EditorEngine.h"

#include "../Object/Actor.h"

InspectorUI::InspectorUI()
{}

InspectorUI::~InspectorUI()
{}

bool InspectorUI::Init(const std::string& name)
{
    EditorUI::Init(name);

    SetEnable(false);

    return true;
}

void InspectorUI::Render(float deltaTime)
{
    EditorUI::Render(deltaTime);

    ImGui::Begin(_name.c_str());

    if (!_target)
    {
        ImGui::Text("Actor is Null");

        ImGui::End();

        return;
    }

    //DrawWorldTransform();

    _target->DrawInspector();

    ImGui::End();
}

void InspectorUI::Destroy()
{}

void InspectorUI::SetTarget(Ptr<class Object> target)
{
    _target = target;
}

Ptr<class Object> InspectorUI::GetTarget() const
{
    return _target;
}

void InspectorUI::DrawWorldTransform()
{
    //ImGui::SeparatorText("World Transform");

    //FVector3D pos = _target->GetWorldPosition();
    //if (ImGui::DragFloat3("World Position", &pos._x, 0.1f))
    //    _target->SetWorldPosition(pos);

    //FVector3D rot = _target->GetWorldRotation();
    //if (ImGui::DragFloat3("World Rotation", &rot._x, 0.1f))
    //    _target->SetWorldPosition(rot);

    //FVector3D scale = _target->GetWorldScale();
    //if (ImGui::DragFloat3("World Scale", &scale._x, 0.1f))
    //    _target->SetWorldPosition(scale);
}

void InspectorUI::DrawRelativeTransform()
{
    //ImGui::SeparatorText("Relative Transform");
}

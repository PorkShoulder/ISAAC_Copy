#include "pch.h"
#include "WidgetComponent.h"
// #include "../UI/Widget.h"

WidgetComponent::WidgetComponent()
{
    _isRender = true;
}

WidgetComponent::~WidgetComponent()
{
}

void WidgetComponent::SetWidget(Ptr<class Widget> widget)
{
    _widget = widget;
    _widget->SetOwnerActor(_owner);
}

bool WidgetComponent::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    SceneComponent::Init(id, name, owner);

    SetRenderLayer("UI");

    return true;
}

void WidgetComponent::Tick(float deltaTime)
{
    SceneComponent::Tick(deltaTime);

    if (_widget)
        _widget->Tick(deltaTime);
}

void WidgetComponent::Render(float deltaTime)
{
    SceneComponent::Render(deltaTime);

    if (_widget)
        _widget->Render(_world._position);
}

void WidgetComponent::Destroy()
{

}

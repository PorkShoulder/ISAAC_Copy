#include "pch.h"
#include "UIManager.h"

#include "../Render/RenderManager.h"
#include "../Render/BlendState.h"
#include "../Render/DepthStencilState.h"

#include "../Input/InputSystem.h"

UIManager::UIManager()
{}

UIManager::~UIManager()
{}

void UIManager::MouseHovered(const FVector2D& pos)
{
    //todo : UIButton
    Ptr<Widget> currentHit = nullptr;

    //나중에 생성된 UI가 가장 위에
    for (auto it = _rootWidgets.rbegin(); it != _rootWidgets.rend(); ++it)
    {
        if (auto hit = it->second->MouseHit(pos))
        {
            currentHit = hit;
            break;
        }
    }

    //호버 상태 갱신
    Ptr<Widget> prevHorved = Lock<Widget>(_hoveredWidget);

    if (currentHit != prevHorved)
    {
        if (prevHorved)
            prevHorved->OnUnHovered();

        if (currentHit)
            currentHit->OnHovered();

        _hoveredWidget = currentHit;
    }
}

void UIManager::MousePressed(const FVector2D & pos)
{
    Ptr<Widget> widget = Lock<Widget>(_hoveredWidget);
    if (widget)
        widget->OnPressed();

    _pressedWidget = widget;
}

void UIManager::MouseReleased(const FVector2D & pos)
{
    Ptr<Widget> pressed = Lock<Widget>(_pressedWidget);
    if (!pressed)
        return;

    if (pressed->IsInSide(pos))
        pressed->OnClick();
    else
        pressed->OnUnHovered();

    _pressedWidget.reset();
}

bool UIManager::Init(Ptr<class Level> level)
{
    _level = level;

    return true;
}

void UIManager::Tick(float deltaTime)
{
    for (auto it : _removeWidgets)
    {
        auto foundIt = _rootWidgets.find(it);
        if (_rootWidgets.end() == foundIt)
            continue;

        DESTROY(foundIt->second);
        _rootWidgets.erase(it);
    }

    _removeWidgets.clear();

    for (auto& it : _rootWidgets)
        it.second->Tick(deltaTime);
}

void UIManager::Render()
{
    RenderManager::Instance().GetAlphaBlend()->SetState();
    RenderManager::Instance().GetDepthStencilState()->SetState();

    for (auto& it : _rootWidgets)
        it.second->Render();

    RenderManager::Instance().GetAlphaBlend()->ResetState();
    RenderManager::Instance().GetDepthStencilState()->ResetState();
}

void UIManager::MouseEvent()
{
    FVector2D mousePos = InputSystem::Instance().GetMousePos();

    MouseHovered(mousePos);

    if (InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton))
        MousePressed(mousePos);

    if (InputSystem::Instance().GetMouseUp(MOUSE_BUTTON_TYPE::LButton))
        MouseReleased(mousePos);
}

void UIManager::AddRemove(int32 remove)
{
    _removeWidgets.push_back(remove);
}

void UIManager::AddToViewport(Ptr<class Widget> widget)
{
    _rootWidgets[widget->GetWidgetID()] = widget;
}

void UIManager::Destroy()
{
    for (auto& it : _rootWidgets)
    {
        DESTROY(it.second);
    }

    _rootWidgets.clear();
}

Ptr<class Level> UIManager::GetLevel() const
{
    return Lock<Level>(_level);
}

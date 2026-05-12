#pragma once
#include "Core/Object.h"
#include "Widget.h"


class UIManager : public Object
{
public:
    UIManager();
    virtual ~UIManager();

private:
    int32 _widgetID = 0;
    Weak<class Level> _level;

    //UI 중 가장 최상위 부모들만
    std::map<int32, Ptr<class Widget>> _rootWidgets;
    std::vector<int32> _removeWidgets;

    Weak<class Widget> _hoveredWidget; //지금 마우스가 위에 올라간 위젯
    Weak<class Widget> _pressedWidget; //마우스가 누른 위젯

private:
    void MouseHovered(const FVector2D& pos);
    void MousePressed(const FVector2D& pos);
    void MouseReleased(const FVector2D& pos);

public:
    bool Init(Ptr<class Level> level);
    void Tick(float deltaTime);
    void Render();
    void MouseEvent();

    void AddRemove(int32 remove);

    void AddToViewport(Ptr<class Widget> widget);

    virtual void Destroy() override;
    

public:
    Ptr<class Level> GetLevel() const;

    template<typename T>
    Ptr<T> CreateWidget(const std::string& name)
    {
        int32 id = _widgetID++;
        Ptr<T> widget = New<T>();
        if (false == widget->Init(id, name, _level))
        {
            DESTROY(widget);
            return nullptr;
        }

        return widget;
    }


};


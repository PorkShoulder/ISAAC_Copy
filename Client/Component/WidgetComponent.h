#pragma once
#include "SceneComponent.h"
//#include "UI/TextBlock.h"
//#include "UI/Button.h"
//#include "UI/ProgressBar.h"
//#include "UI/Image.h"

//======================================
// UI 위젯을 오브젝트에 붙이는 컴포넌트.
//======================================
;


class WidgetComponent : public SceneComponent
{
public:
    WidgetComponent();
    virtual ~WidgetComponent();

protected:
    Ptr<class Widget> _widget;

public:
    void SetWidget(Ptr<class Widget> widget);
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;


};


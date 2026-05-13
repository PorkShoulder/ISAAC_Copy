#pragma once
#include "EditorUI.h"


//=======================================================
// 현재 Level에 존재하는 
// 모든 Actor와 컴포넌트를 트리 구조로 보여주는 UI
//=======================================================




class OutLinerUI : public EditorUI
{
public:
    OutLinerUI();
    virtual ~OutLinerUI();

public:
    virtual bool Init(const std::string& name);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;

private:
    // 현재 Level의 _actors를 가져와 
    // DrawActor() -> DrawSceneComponent() 순서로 ImGui 트리노드를 그립니다. 
    void DrawActor(Ptr<class Actor> actor);                 
    void DrawSceneComponent(Ptr<class SceneComponent> comp);
};


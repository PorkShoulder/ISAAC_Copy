#pragma once
#include "EditorUI.h"

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
    void DrawActor(Ptr<class Actor> actor);
    void DrawSceneComponent(Ptr<class SceneComponent> comp);
};


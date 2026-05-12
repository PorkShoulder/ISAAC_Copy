#pragma once
#include "EditorUI.h"

class InspectorUI : public EditorUI
{
public:
    InspectorUI();
    virtual ~InspectorUI();

private:
    Ptr<class Object> _target;

public:
    virtual bool Init(const std::string& name);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;


    void SetTarget(Ptr<class Object> target);
    Ptr<class Object> GetTarget() const;

private:
    void DrawWorldTransform();
    void DrawRelativeTransform();

};


#pragma once
#include "Core/Object.h"

class EditorUI : public Object
{
public:
    EditorUI();
    virtual ~EditorUI();

protected:
    std::string _name;
    
public:
    virtual bool Init(const std::string& name);
    virtual void Tick(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;
};


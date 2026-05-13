#pragma once
#include "Core/Object.h"

//=================================
// 모든 에디터 UI의 부모 클래스입니다.
//=================================

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


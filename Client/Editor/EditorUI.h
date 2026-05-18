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
    bool _open = true;
protected:
    bool BeginWindow();
    void EndWindow();
    
    
public:
    virtual bool Init(const std::string& name);
    virtual void Tick(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;

    void SetOpen(bool open) { _open = open; }
    bool IsOpen()const { return _open; }
    const std::string& GetName() const { return _name; }

};


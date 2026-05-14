#pragma once
#include "EditorUI.h"
#include "EditorEngine.h"

class PopUpUI : public EditorUI
{
public:
    PopUpUI();
    virtual ~PopUpUI();

private:
    bool _open = false;
    std::string _msg;
    ePopUpType _type;

public:
    virtual bool Init(const std::string& name);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;

    void Show(ePopUpType type, const std::string& msg);
    void ShowError(const std::string& msg);
    void ShowWarning(const std::string& msg);
    void ShowInfo(const std::string& msg);
    void ShowConfirm(const std::string& msg);
};


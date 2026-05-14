#pragma once
#include "Common/Singleton.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <unordered_map>
#include <functional>

enum class ePopUpType
{
    Error,
    Warning,
    Info,
    Confirm,
    End
};



class EditorEngine : public Singleton<EditorEngine>
{
    SINGLETON(EditorEngine)

private:
    float _clearColor[4] = { 1, 1, 1, 1 }; //바탕을 어떤색으로 초기화 시킬 지
    std::unordered_map<std::string, Ptr<class EditorUI>> _edits;
    std::unordered_map<std::string, std::function<void()>> _actorCreateFuncs;
    Ptr<class PopUpUI> _popUp;

public:
    bool Init();

    virtual void Destroy() override;

private:
    void Logic();
    void Tick(float deltaTime);
    void Collision(float deltaTime);
    void Render(float deltaTime);
     
public:
    bool Run();
    void Show(ePopUpType type, const std::string& msg);
    void ShowError(const std::string& msg);
    void ShowWarning(const std::string& msg);
    void ShowInfo(const std::string& msg);
    void ShowConfirm(const std::string& msg);

private:
    template<typename T>
    void CreateEditorUI(const std::string& name)
    {
        Ptr<T> it = FindEditorUI<T>(name);
        if (it)
            return;

        it = New<T>();
        if (!it->Init(name))
        {
            DESTROY(it);
            return;
        }

        _edits[name] = it;
    }

public:
    template<typename T>
    Ptr<T> FindEditorUI(const std::string& name)
    {
        auto it = _edits.find(name);
        if (_edits.end() == it)
            return nullptr;

        return Cast<EditorUI, T>(it->second);
    }


};


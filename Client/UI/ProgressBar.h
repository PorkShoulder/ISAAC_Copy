#pragma once
#include "Widget.h"


namespace PROGRESS_BAR_IMAGE
{
    enum Type
    {
        BACK,
        FILL,
        END
    };
}

enum class eProgressBarDir
{
    RIGHT_LEFT,
    TOP_DOWN,
    END
};

class ProgressBar : public Widget
{
public:
    ProgressBar();
    virtual ~ProgressBar();

protected:
    FUIBrush _brush[PROGRESS_BAR_IMAGE::END];
    eProgressBarDir _barDir = eProgressBarDir::RIGHT_LEFT;
    float _percent = 1.f;

    //프로그래스바 안에 막대용 쉐이더
    Ptr<class Shader> _fillShader;

    //막대용 상수버퍼
    Ptr<class ProgressBarCBuffer> _fillCBuffer;

public:
    void SetTexture(PROGRESS_BAR_IMAGE::Type type, const std::string& name);
    void SetTexture(PROGRESS_BAR_IMAGE::Type type, const std::string& name, const std::wstring& fileName);
    void SetTexture(PROGRESS_BAR_IMAGE::Type type, Ptr<Texture> texture);

    void SetOpacity(PROGRESS_BAR_IMAGE::Type type, float a);
    void SetTint(PROGRESS_BAR_IMAGE::Type type, float r, float g, float b, float a);
    void SetBrushAnimEnable(PROGRESS_BAR_IMAGE::Type type, bool anim);
    void AddBrushFrame(PROGRESS_BAR_IMAGE::Type type, const FVector2D& start, const FVector2D& size);
    void AddBrushFrame(PROGRESS_BAR_IMAGE::Type type, float startX, float startY, float sizeX, float sizeY);
    void SetCurrentFrame(PROGRESS_BAR_IMAGE::Type type, int32 frame);
    void SetAnimationPlayTime(PROGRESS_BAR_IMAGE::Type type, float time);
    void SetAnimationPlayRate(PROGRESS_BAR_IMAGE::Type type, float rate);
    void SetPercent(float percent);
    void SetBarDir(eProgressBarDir dir);

public:
    virtual bool Init(int32 id, const std::string& name, Weak<class Level> level);
    virtual void Tick(float deltaTime);
    virtual void Render();
    virtual void Render(const FVector3D& pos); //나중에 액터에 붙어야 할때
    virtual void Destroy() override;

private:
    void RenderBrush(PROGRESS_BAR_IMAGE::Type type);
    void RenderBrush(PROGRESS_BAR_IMAGE::Type type, const FVector2D& pos);

};


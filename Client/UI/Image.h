#pragma once
#include "Widget.h"

class Image : public Widget 
{
public:
    Image();
    virtual ~Image();

protected:
    FUIBrush _brush;

public:
    void SetTexture(const std::string& name);
    void SetTexture(const std::string& name, const std::wstring& fileName);
    void SetTexture(Ptr<Texture> texture);

    void SetOpacity(float a);
    void SetTint(float r, float g, float b, float a);

    void SetBrushAnimEnable(bool enable);
    void AddBrushFrame(const FVector2D& start, const FVector2D& size);
    void AddBrushFrame(float startX, float startY, float sizeX, float sizeY);
    void SetCurrentFrame(int32 frame);
    void SetAnimationPlayTime(float time);
    void SetAnimationPlayRate(float rate);

public:
    virtual bool Init(int32 id, const std::string& name, Weak<class Level> level);
    virtual void Tick(float deltaTime);
    virtual void Render();
    virtual void Render(const FVector3D& pos); //나중에 액터에 붙어야 할때
    virtual void Destroy() override;

};


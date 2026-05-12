#include "pch.h"
#include "ProgressBar.h"

#include "Shader/ShaderManager.h"
#include "Shader/TranformCBuffer.h"
#include "Shader/UICBuffer.h"
#include "Shader/ProgressBarShader.h"
#include "Shader/ProgressBarCBuffer.h"

#include "World/Level.h"

#include "Core/Mesh.h"
#include "Core/AssetManager.h"
#include "Core/Device.h"

ProgressBar::ProgressBar()
{}

ProgressBar::~ProgressBar()
{}

void ProgressBar::SetTexture(PROGRESS_BAR_IMAGE::Type type, const std::string& name)
{
    SetTexture(type, TEXTURE_MANAGER->Findtexture(name));
}

void ProgressBar::SetTexture(PROGRESS_BAR_IMAGE::Type type, const std::string & name, const std::wstring & fileName)
{
    if (!TEXTURE_MANAGER->LoadTexture(name, fileName))
        return;

    SetTexture(type, TEXTURE_MANAGER->Findtexture(name));
}

void ProgressBar::SetTexture(PROGRESS_BAR_IMAGE::Type type, Ptr<Texture> texture)
{
    _brush[type]._texture = texture;
}

void ProgressBar::SetOpacity(PROGRESS_BAR_IMAGE::Type type, float a)
{
    _brush[type]._tint._w = a;
}

void ProgressBar::SetTint(PROGRESS_BAR_IMAGE::Type type, float r, float g, float b, float a)
{
    _brush[type]._tint._x = r;
    _brush[type]._tint._y = g;
    _brush[type]._tint._z = b;
    _brush[type]._tint._w = a;
}

void ProgressBar::SetBrushAnimEnable(PROGRESS_BAR_IMAGE::Type type, bool anim)
{
    _brush[type]._animEnable = anim;
}

void ProgressBar::AddBrushFrame(PROGRESS_BAR_IMAGE::Type type, const FVector2D & start, const FVector2D & size)
{
    AddBrushFrame(type, start._x, start._y, size._x, size._y);
}

void ProgressBar::AddBrushFrame(PROGRESS_BAR_IMAGE::Type type, float startX, float startY, float sizeX, float sizeY)
{
    FAnimationFrame frame;
    frame._start._x = startX;
    frame._start._y = startY;

    frame._size._x = sizeX;
    frame._size._y = sizeY;

    _brush[type]._frames.push_back(frame);
    _brush[type]._frameTime = _brush[type]._playTime / _brush[type]._frames.size();
}

void ProgressBar::SetCurrentFrame(PROGRESS_BAR_IMAGE::Type type, int32 frame)
{
    _brush[type]._frame = frame;
}

void ProgressBar::SetAnimationPlayTime(PROGRESS_BAR_IMAGE::Type type, float time)
{
    _brush[type]._playTime = time;

    _brush[type]._frameTime = _brush[type]._playTime / _brush[type]._frames.size();
}

void ProgressBar::SetAnimationPlayRate(PROGRESS_BAR_IMAGE::Type type, float rate)
{
    _brush[type]._playRate = rate;
}

void ProgressBar::SetPercent(float percent)
{
    _percent = percent;
}

void ProgressBar::SetBarDir(eProgressBarDir dir)
{
    _barDir = dir;
}

bool ProgressBar::Init(int32 id, const std::string & name, Weak<class Level> level)
{
    Widget::Init(id, name, level);

    _fillCBuffer = FIND_CBUFFER("ProgressBar", ProgressBarCBuffer);

    _fillShader = FIND_SHADER("ProgressBar");

    return true;
}

void ProgressBar::Tick(float deltaTime)
{
    Widget::Tick(deltaTime);
}

void ProgressBar::Render()
{
    RenderBrush(PROGRESS_BAR_IMAGE::BACK);
    
    RenderBrush(PROGRESS_BAR_IMAGE::FILL);

    Widget::Render();
}

void ProgressBar::Render(const FVector3D & pos)
{
    RenderBrush(PROGRESS_BAR_IMAGE::BACK, FVector2D(pos._x, pos._y));

    RenderBrush(PROGRESS_BAR_IMAGE::FILL, FVector2D(pos._x, pos._y));

    Widget::Render();
}

void ProgressBar::Destroy()
{}

void ProgressBar::RenderBrush(PROGRESS_BAR_IMAGE::Type type)
{
    if (!IsEnable() || !IsActive())
        return;

    auto level = Lock<Level>(_level);
    if (!level)
        return;

    ShaderManager::Instance().SetSample(eTextureSampleType::TEXTURE_SAMPLE_LINEAR);

    FMatrix matScale, matRot, matTrans, matWorld;

    matScale.Scaling(_size);
    matRot.Indentity();
    matTrans.Translation(_renderPos);

    matWorld = matScale * matRot * matTrans;

    FMatrix matView;
    matView.Indentity();

    _transformCBuffer->SetWorldMatrix(matWorld);
    _transformCBuffer->SetViewMatrix(matView);
    _transformCBuffer->SetProjMatrix(level->GetUIProjMatrix());
    _transformCBuffer->Update();

    _uiCBuffer->SetTint(_brush[type]._tint);

    if (_brush[type]._texture)
    {
        _uiCBuffer->SetTextureEnable(true);
        _brush[type]._texture->SetShader(0, SHADER_TYPE::PIXEL, 0);
    }
    else
    {
        _uiCBuffer->SetTextureEnable(false);
    }

    if (_brush[type]._animEnable)
    {
        _uiCBuffer->SetAnimEnable(true);
        int32 frame = _brush[type]._frame;

        FAnimationFrame frameData = _brush[type]._frames[frame];

        _uiCBuffer->SetUV(frameData._start, frameData._start + frameData._size);
    }
    else
    {
        _uiCBuffer->SetAnimEnable(false);
    }

    _uiCBuffer->Update();

    if (type == PROGRESS_BAR_IMAGE::FILL)
    {
        _fillCBuffer->SetDir(static_cast<int32>(_barDir));
        _fillCBuffer->SetPercent(_percent);

        _fillCBuffer->Update();

        _fillShader->SetShader();
    }
    else
    {
        _shader->SetShader();
    }

    _mesh->Render();
}

void ProgressBar::RenderBrush(PROGRESS_BAR_IMAGE::Type type, const FVector2D & pos)
{
    if (!IsEnable() || !IsActive())
        return;

    ShaderManager::Instance().SetSample(eTextureSampleType::TEXTURE_SAMPLE_LINEAR);

    auto level = Lock<Level>(_level);
    if (!level)
        return;

    FResolution rs = Device::Instance().GetRS();

    const FVector3D& camPos = level->GetCameraWorldPos();

    FVector2D renderPos = _renderPos;
    renderPos._x += pos._x;
    renderPos._y += pos._y;

    renderPos._x -= camPos._x;
    renderPos._y -= camPos._y;

    renderPos._x -= (rs._width * 0.5);
    renderPos._y -= (rs._height * 0.5);

    FMatrix matScale, matRot, matTrans, matWorld;

    matScale.Scaling(_size);
    matRot.Indentity();
    matTrans.Translation(renderPos);

    matWorld = matScale * matRot * matTrans;

    FMatrix matView;
    matView.Indentity();

    _transformCBuffer->SetWorldMatrix(matWorld);
    _transformCBuffer->SetViewMatrix(matView);
    _transformCBuffer->SetProjMatrix(level->GetUIProjMatrix());
    _transformCBuffer->Update();

    _uiCBuffer->SetTint(_brush[type]._tint);

    if (_brush[type]._texture)
    {
        _uiCBuffer->SetTextureEnable(true);
        _brush[type]._texture->SetShader(0, SHADER_TYPE::PIXEL, 0);
    }
    else
    {
        _uiCBuffer->SetTextureEnable(false);
    }

    if (_brush[type]._animEnable)
    {
        _uiCBuffer->SetAnimEnable(true);
        int32 frame = _brush[type]._frame;

        FAnimationFrame frameData = _brush[type]._frames[frame];

        _uiCBuffer->SetUV(frameData._start, frameData._start + frameData._size);
    }
    else
    {
        _uiCBuffer->SetAnimEnable(false);
    }

    _uiCBuffer->Update();

    if (type == PROGRESS_BAR_IMAGE::FILL)
    {
        _fillCBuffer->SetDir(static_cast<int32>(_barDir));
        _fillCBuffer->SetPercent(_percent);

        _fillCBuffer->Update();

        _fillShader->SetShader();
    }
    else
    {
        _shader->SetShader();
    }

    _mesh->Render();
}

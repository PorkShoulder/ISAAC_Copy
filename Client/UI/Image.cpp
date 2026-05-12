#include "pch.h"
#include "Image.h"

#include "Core/AssetManager.h"
#include "Core/Mesh.h"
#include "Core/Device.h"

#include "Shader/ShaderManager.h"
#include "Shader/TranformCBuffer.h"
#include "Shader/UIShader.h"
#include "Shader/UICBuffer.h"

#include "World/Level.h"

Image::Image()
{}

Image::~Image()
{}

void Image::SetTexture(const std::string& name)
{
    SetTexture(TEXTURE_MANAGER->Findtexture(name));
}

void Image::SetTexture(const std::string & name, const std::wstring & fileName)
{
    if (!TEXTURE_MANAGER->LoadTexture(name, fileName))
        return;

    SetTexture(TEXTURE_MANAGER->Findtexture(name));
}

void Image::SetTexture(Ptr<Texture> texture)
{
    _brush._texture = texture;
}

void Image::SetOpacity(float a)
{
    _brush._tint._w = a;
}

void Image::SetTint(float r, float g, float b, float a)
{
    _brush._tint._x = r;
    _brush._tint._y = g;
    _brush._tint._z = b;
    _brush._tint._w = a;
}

void Image::SetBrushAnimEnable(bool enable)
{
    _brush._animEnable = enable;
}

void Image::AddBrushFrame(const FVector2D & start, const FVector2D & size)
{
    AddBrushFrame(start._x, start._y, size._x, size._y);
}

void Image::AddBrushFrame(float startX, float startY, float sizeX, float sizeY)
{
    FAnimationFrame frame;
    frame._start._x = startX;
    frame._start._y = startY;

    frame._size._x = sizeX;
    frame._size._y = sizeY;

    _brush._frames.push_back(frame);

    _brush._frameTime = _brush._playTime / _brush._frames.size();
}

void Image::SetCurrentFrame(int32 frame)
{
    _brush._frame = frame;
}

void Image::SetAnimationPlayTime(float time)
{
    _brush._playTime = time;

    _brush._frameTime = _brush._playTime / _brush._frames.size();
}

void Image::SetAnimationPlayRate(float rate)
{
    _brush._playRate = rate;
}

bool Image::Init(int32 id, const std::string& name, Weak<class Level> level)
{
    Widget::Init(id, name, level);

    return true;
}

void Image::Tick(float deltaTime)
{
    Widget::Tick(deltaTime);
}

void Image::Render()
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

    _uiCBuffer->SetTint(_brush._tint);

    if (_brush._texture)
    {
        _uiCBuffer->SetTextureEnable(true);
        _brush._texture->SetShader(0, SHADER_TYPE::PIXEL, 0);
    }
    else
    {
        _uiCBuffer->SetTextureEnable(false);
    }

    if (_brush._animEnable)
    {
        _uiCBuffer->SetAnimEnable(true);
        int32 frame = _brush._frame;

        FAnimationFrame frameData = _brush._frames[frame];

        _uiCBuffer->SetUV(frameData._start, frameData._start + frameData._size);
    }
    else
    {
        _uiCBuffer->SetAnimEnable(false);
    }

    _uiCBuffer->Update();

    _shader->SetShader();
    _mesh->Render();

    Widget::Render();
}

void Image::Render(const FVector3D & pos)
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

    _uiCBuffer->SetTint(_brush._tint);

    if (_brush._texture)
    {
        _uiCBuffer->SetTextureEnable(true);
        _brush._texture->SetShader(0, SHADER_TYPE::PIXEL, 0);
    }
    else
    {
        _uiCBuffer->SetTextureEnable(false);
    }

    if (_brush._animEnable)
    {
        _uiCBuffer->SetAnimEnable(true);
        int32 frame = _brush._frame;

        FAnimationFrame frameData = _brush._frames[frame];

        _uiCBuffer->SetUV(frameData._start, frameData._start + frameData._size);
    }
    else
    {
        _uiCBuffer->SetAnimEnable(false);
    }

    _uiCBuffer->Update();

    _shader->SetShader();
    _mesh->Render();

    Widget::Render();
}

void Image::Destroy()
{}

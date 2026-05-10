#pragma once
#include "Shader.h"

class MaterialPixelShader : public Shader
{
public:
    MaterialPixelShader();
    virtual ~MaterialPixelShader();

private:
    //픽셀쉐이더 객체
    ComPtr<ID3D11PixelShader> _ps = nullptr;

    //픽셀 쉐이더 코드를 컴파일하고 나온 바이트 코드를 저장하기 위한 버퍼
    ComPtr<ID3DBlob> _psBlob = nullptr;

public:
    virtual bool Init() override;
    virtual void SetShader() final;
    bool LoadPixelShader(const char* entryName, const char* fileName);

    virtual void Destroy() override;

};


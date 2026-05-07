#pragma once
#include "CBuffer.h"
#include "CBufferData.h"

//======================================================
// 머테리얼 관련 데이터 GPU쉐이더로 보내기 위한 상수버퍼
//======================================================


class MaterialCBuffer : public CBuffer
{
public:
    MaterialCBuffer();
    virtual ~MaterialCBuffer();

private:
    FMaterialCBufferData _data;

public:
    virtual void Update();

    void SetBaseColor(float r, float g, float b, float a);  // 색상설정
    void SetBaseColor(const FVector4D& color);              // 
    void SetOpacity(float op);                              // 투명도 설정
    void SetTextureWidth(int32 w);  // 크기설정
    void SetTextureHeight(int32 h);
};


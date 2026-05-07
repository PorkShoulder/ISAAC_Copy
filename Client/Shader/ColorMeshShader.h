#pragma once
#include "GraphicShader.h"

//=======================================
// 색상 정점을 그리는 전용 쉐이더 클래스 
//=======================================

class ColorMeshShader : public GraphicShader
{
public:
    ColorMeshShader();
    ~ColorMeshShader();

public:
    virtual bool Init() override;
};


#pragma once
#include "GraphicShader.h"


//=================================
// 테두리를 그리는 쉐이더 
//=================================

class FrameMeshShader : public GraphicShader
{
public:
    FrameMeshShader();
    ~FrameMeshShader();

public:
    virtual bool Init() override;
};


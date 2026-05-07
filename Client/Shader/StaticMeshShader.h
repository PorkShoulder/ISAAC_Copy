#pragma once
#include "GraphicShader.h"

class StaticMeshShader : public GraphicShader
{
public:
    StaticMeshShader();
    virtual ~StaticMeshShader();

public:
    virtual bool Init() override;
    virtual void Destroy() override;

};


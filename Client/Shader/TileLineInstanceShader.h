#pragma once
#include "GraphicShader.h"

class TileLineInstanceShader : public GraphicShader
{
public:
    TileLineInstanceShader();
    virtual ~TileLineInstanceShader();

public:
    virtual bool Init() override;
    virtual void Destroy() override;
};


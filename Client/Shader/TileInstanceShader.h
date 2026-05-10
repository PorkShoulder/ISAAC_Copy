#pragma once
#include "GraphicShader.h"

class TileInstanceShader : public GraphicShader
{
public:
    TileInstanceShader();
    virtual ~TileInstanceShader();

public:
    virtual bool Init() override;
};


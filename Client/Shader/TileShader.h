#pragma once
#include "GraphicShader.h"

class TileShader : public GraphicShader
{
public:
    TileShader();
    virtual ~TileShader();

public:
    virtual bool Init() override;
};


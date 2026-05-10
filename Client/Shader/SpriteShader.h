#pragma once
#include "GraphicShader.h"

class SpriteShader : public GraphicShader
{
public:
    SpriteShader();
    virtual ~SpriteShader();

public:
    virtual bool Init() override;
    virtual void Destroy() override;
};


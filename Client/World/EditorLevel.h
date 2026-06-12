#pragma once
#include "Level.h"

class EditorLevel : public Level
{
public:
    virtual bool Init(const std::string& path) override;
};
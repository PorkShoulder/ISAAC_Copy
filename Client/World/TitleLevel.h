#pragma once
#include "Level.h"


class TitleLevel :public Level
{
	virtual bool Init(const std::string& path) override;
};


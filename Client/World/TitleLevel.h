#pragma once
#include "Level.h"


class TitleLevel :public Level
{
	

public:
	virtual bool Init(const std::string& path) override;
	virtual void Tick(float deltaTime) override;

	bool _f1Pressed = false;
};


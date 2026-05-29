#pragma once
#include "EditorUI.h"


class ObstacleUI : public EditorUI
{
public:
	ObstacleUI();
	virtual ~ObstacleUI();

public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float delteTime) override;
	virtual void Destroy() override;
};


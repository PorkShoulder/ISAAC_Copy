#pragma once
#include "EditorEngine.h"
#include "ObjectEditorUI.h"

class ObstacleUI : public ObjectEditorUI
{
public:
	ObstacleUI();
	virtual ~ObstacleUI();

public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float delteTime) override;
	virtual void Destroy() override;
};


#pragma once
#include "EditorUI.h"


class ItemUI : public EditorUI
{
public:
	ItemUI();
	virtual ~ItemUI();

public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;

};
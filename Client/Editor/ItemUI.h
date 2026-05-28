#pragma once
#include "EditorEngine.h"
#include "ObjectEditorUI.h"

class ItemUI : public ObjectEditorUI
{
public:
	ItemUI();
	virtual ~ItemUI();

public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;

};
#pragma once
#include "EditorEngine.h"
#include "ObjectEditorUI.h"


class NpcUI : public ObjectEditorUI
{
public:
	NpcUI();
	virtual ~NpcUI();

public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;

};
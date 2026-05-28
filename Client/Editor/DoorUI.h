#pragma once
#include "EditorEngine.h"
#include "ObjectEditorUI.h"

class DoorUI : public ObjectEditorUI
{
public:
	DoorUI();
	virtual ~DoorUI();
public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float delteTime) override;
	virtual void Destroy() override;

};

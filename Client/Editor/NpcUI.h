#pragma once
#include "EditorUI.h"



class NpcUI : public EditorUI
{
public:
	NpcUI();
	virtual ~NpcUI();

public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;

};
#pragma once
#include "EditorUI.h"

class MonsterUI : public EditorUI
{

public:
	MonsterUI();
	virtual ~MonsterUI();
public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;


};


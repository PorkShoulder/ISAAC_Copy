#pragma once
#include "EditorUI.h"
#include "../Common/Info.h"

class EditorMode : public EditorUI
{
public:
	EditorMode();
	virtual ~EditorMode();
public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime)  override;
	virtual void Destroy() override;

	eActorType GetEditMode() const { return _mode; }
private:
	
	// 추가되는 UI (자료구조 연습해보기)
	eActorType _mode = eActorType::End;
	std::map<eActorType, Ptr<EditorUI>> _findUIs;
};


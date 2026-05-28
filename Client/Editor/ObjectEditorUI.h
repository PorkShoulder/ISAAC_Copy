#pragma once
#include"EditorUI.h"

// EditorMode로 전달하기 전 중간관리자
// 맵에 사용되는 모든 오브젝틑 관리함.

class ObjectEditorUI : public EditorUI
{
public:
	ObjectEditorUI();
	virtual ~ObjectEditorUI();

public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;
private:
	Ptr<class ObstacleUI>	_obstacleUI;
	Ptr<class DoorUI>		_doorUI;
	Ptr<class NpcUI>		_npcUI;
	Ptr<class ItemUI>		_itemUI;
	


};


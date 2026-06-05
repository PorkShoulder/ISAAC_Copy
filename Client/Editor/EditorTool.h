#pragma once
#include "EditorUI.h"
#include "RoomEditor.h"
#include "../Common/Info.h"

// 에디터들의 탭 관리 

class EditorTool : public EditorUI
{
public:
	EditorTool();
	virtual ~EditorTool();
public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime)  override;
	virtual void Destroy() override;
	eActorType GetEditMode() const { return _mode; }

private:
	// 추가되는 UI (자료구조 연습해보기)
	eActorType _mode = eActorType::End;
	std::map<eActorType, Ptr<EditorUI>> _findUIs;

	Ptr<class TileMap> _editingTileMap;

private:
	// UI 등록하기
	template<typename T>
	void RegisterMode(eActorType type, const std::string& name)
	{
		Ptr<T> ui = New<T>();
		ui->Init(name);
		_findUIs[type] = ui;
	}
};


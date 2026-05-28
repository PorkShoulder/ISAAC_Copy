#include "pch.h"
#include "ObjectEditorUI.h"
#include "EditorEngine.h"

// Object 해당 하는 UI 목록
#include "ObstacleUI.h"
#include "DoorUI.h"
#include "NpcUI.h"
#include "ItemUI.h"

ObjectEditorUI::ObjectEditorUI()
{
}

ObjectEditorUI::~ObjectEditorUI()
{
}

bool ObjectEditorUI::Init(const std::string& name)
{
	EditorUI::Init(name);

    _obstacleUI = New<ObstacleUI>();
    _obstacleUI->Init("ObstacleEdit");

    _doorUI = New<DoorUI>();
    _doorUI->Init("DoorEdit");

    _npcUI = New<NpcUI>();
    _npcUI->Init("NpcEdit");

    _itemUI = New<ItemUI>();
    _itemUI->Init("ItemEdit");

    return true;
}

void ObjectEditorUI::Render(float deltaTime)
{
}

void ObjectEditorUI::Destroy()
{
}

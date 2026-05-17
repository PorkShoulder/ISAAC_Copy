#pragma once
#include "EditorUI.h"

class TileMapUI : public EditorUI
{
public:
	TileMapUI();
	virtual ~TileMapUI();
protected:
	
	int _countX = 1;
	int _countY = 1;
	float _tileSize[2] = { 52.f, 52.f };
	Ptr<class TileMap> _targetTileMap;

public:
	

	virtual bool Init(const std::string& name) override; 
	virtual void Render(float deltaTime)override;
	virtual void Destroy() override;

	




};


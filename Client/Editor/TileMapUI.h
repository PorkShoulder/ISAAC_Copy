#pragma once
#include "EditorUI.h"

class TileMapUI : public EditorUI
{
public:
	TileMapUI();
	virtual ~TileMapUI();
protected:
	
	int _countX;
	int _countY;
	float _tileSize[2];


public:
	virtual bool Init(const std::string& name) override; 
	virtual void Render(float deltaTime)override;
	virtual void Destroy() override;




};


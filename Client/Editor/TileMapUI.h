#pragma once
#include "RoomEditor.h"


// 타일맵 배치용 UI

class TileMapUI : public RoomEditor
{
	//class TileMap;

public:
	TileMapUI();
	virtual ~TileMapUI();
protected:

    // 타일맵 생성 
    int _countX = 15;
    int _countY = 9;
    float _tileSize[2] = { 52.f, 52.f };

    // 그리드 레이아웃 (ㄱ/ㄴ형 빈칸 편집용)
    int32 _gridW = 1;
    int32 _gridH = 1;
    std::vector<std::pair<int32, int32>> _emptyCells;

    // 이름
    int _roomNameCounter = 1;
    char _roomName[64] = "";


public:
	virtual bool Init(const std::string& name) override; 
	virtual void Render(float deltaTime)override;
	virtual void Destroy() override;

};


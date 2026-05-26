#pragma once
#include "EditorUI.h"

class TileMapUI : public EditorUI
{
	//class TileMap;

public:
	TileMapUI();
	virtual ~TileMapUI();
protected:
	// 만들 타일맵 크기/ 선택된 타일맵
	int _countX = 1;
	int _countY = 1;
	float _tileSize[2] = { 52.f, 52.f }; // 타일 크기 정수로 바꿔야하나? 
	//std::vector<Ptr<class TileMap>> _targetTileMap;
	Ptr<class TileMap> _targetTileMap;

	// 선택한 텍스처 및 프레임
	std::string _selectedTextureName;
	std::wstring _selectedTexturePath;

	// 선택한 텍스처 객체
	Ptr<class Texture> _selectedTexture;
	//텍스처 아틀라스에서 선택한 프레임의 시작 좌표
	int _selectedFrameX = -1;
	int _selectedFrameY = -1;
	//한 프레임의 크기
	int _frameWidth = 52;
	int _frameHeight = 52;
	// 현재 선택된 프레임 번호
	int _selectedFrameIndex = 0;

	//드래그 기능 추가
	bool _isDragging = false;
	FVector2D _dragStartTex; // 텍스처 좌표 기준 시작점.
	FVector2D _dragEndTex; // 텍스처 좌표 기준 끝점.

	//드래그 선택 범위
	bool _hasDragSelection = false;

	//반전 토글
	bool _flipX = false;
	bool _flipY = false;

	
	// 이름 설정
	int _roomNameCounter = 1;

	char _roomName[64] = "Room_01";





public:
	virtual bool Init(const std::string& name) override; 
	virtual void Render(float deltaTime)override;
	virtual void Destroy() override;

	




};


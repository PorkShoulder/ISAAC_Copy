#pragma once
#include "EditorUI.h"


// 정상적 커밋 확인용 글입니다.

class TileMapUI : public EditorUI
{
	//class TileMap;

public:
	TileMapUI();
	virtual ~TileMapUI();
protected:
	// 만들 타일맵 크기/ 선택된 타일맵
	int _countX = 15;
	int _countY = 9;
	float _tileSize[2] = { 52.f, 52.f }; //  
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
	char _roomName[64] = "";

	// 그리드 레이아웃 (ㄱ/ㄴ형 빈칸 편집용)
	int32 _gridW = 1;
	int32 _gridH = 1;
	std::vector<std::pair<int32, int32>> _emptyCells;
public:
	


public:
	virtual bool Init(const std::string& name) override; 
	virtual void Render(float deltaTime)override;
	virtual void Destroy() override;

	




};


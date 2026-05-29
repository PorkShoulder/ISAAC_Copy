#pragma once
#include "EditorUI.h"

// Editor UI의 공통기능 정리
// Editor 관련된 UI들은 전부 여기서 상속됨.

class RoomEditor : public EditorUI
{
public:
	RoomEditor();
	virtual ~RoomEditor();
protected:

	Ptr<class TileMap> _targetTileMap;
	// 공통 기능 정리
	// 텍스처
	Ptr<class Texture> _selectedTexture;
	std::string _selectedTextureName;
	std::wstring _selectedTexturePath;

	// 프레임
	int _frameWidth = 52;
	int _frameHeight = 52;
	int _selectedFrameIndex = 0;
	int _selectedFrameX = -1;
	int _selectedFrameY = -1;

	// 스냅
	bool _snapGrid = true;
	float _grideSize = 52.f;

	// 반전
	bool _flipX = false;
	bool _flipY = false;

	// 드래그 
	bool _isDragging = false;
	bool _hasDragSelection = false;
	FVector2D _dragStartTex;	// 텍스트 좌표 기준 시작점.
	FVector2D _dragEndTex;		// 텍스처 좌표 기준 끝점.

	// 이름
	int _nameCounter = 1;
	char _objectName[64] = "";

protected:
	// 렌더 함수
	void RenderSnapOption();
	void RenderTextureBrowser();
public:
	virtual bool Init(const std::string& name) override;
	virtual void Render(float deltaTime) override;
	virtual void Destroy() override;



};


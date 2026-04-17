#pragma once
#include "Client.h"
#include "framework.h"
#include "../Common/Singleton.h"




class GameEngine : public Singleton<GameEngine>
{
	SINGLETON(GameEngine)

private:
	HINSTANCE	hInst = nullptr;					// 운영체제가 발급해주는 ID
	HWND		_hWnd = nullptr;					// 창마다 생기는 핸들 (복수 가능)
	HDC			_hdc  = nullptr;					// 그리기 위한 핸들
	TCHAR		_className[256] = {};				// 클래스 이름
	TCHAR		_titleName[256] = {};				// 윈도우 창 타이틀 이름
	float		_clearColor[4] = { 1,1,1,1 };		// 바탕을 초기화 할 색상


private:
	bool Create();									// 윈도우 창 생성
	void RegisterWindowClass();						// 윈도우 클래스 생성 함수
	void Logic();									// 로직 실행 함수
	void Tick();									// 매 프레임 들어오는 함수
	void Collision();								// 
	void Render();									// 그림그리기 

	bool InitManager();								//


	static LRESULT CALLBACK WndProc(HWND hWnd,		//	
									UINT message,	//	
									WPARAM wParam,	//
									LPARAM lParam);	//




};


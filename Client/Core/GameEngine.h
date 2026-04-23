#pragma once
#include "Client.h"
#include "framework.h"
#include "../Common/Singleton.h"




//============================
//
//============================



class GameEngine : public Singleton<GameEngine>
{
	SINGLETON(GameEngine)

private:
	HINSTANCE	m_hInst = nullptr;					// 운영체제가 발급해주는 ID
	HWND		m_hWnd = nullptr;					// 창마다 생기는 핸들 (복수 가능)
	HDC			m_hdc  = nullptr;					// 그리기 위한 핸들
	TCHAR		m_className[256] = {};				// 클래스 이름
	TCHAR		m_titleName[256] = {};				// 윈도우 창 타이틀 이름
	float		m_clearColor[4] = { 1,1,1,1 };		// 바탕을 초기화 할 색상

	static bool m_bIsRun;

public:
	void Destroy() override;					// Singleton::Destroy() 구현 — 내부 리소스 해제

	// 초기화 함수
	bool Init(HINSTANCE inst, const wchar_t* name);

	// 실행 함수
	int Run();

	// 여기서 const -> 읽기만 가능 
	HDC GetHDC() const { return m_hdc; }					// 프로그램 자체의 핸들->윈도우가 이 프로그램이 누구인지 식별하는 ID
	HINSTANCE GetHINSTANCE() const { return m_hInst; }	// 윈도우(창) 핸들 특정 창을 가리키는 ID 창크기,메세지, 다이렉트 초기화때 스왑체인에 알려주는 역할
	HWND GetHWND() const { return m_hWnd; }				// Device Context 핸들 창의 그리기 도구 Dx사용시 거의 안쓰임.




private:
	bool Create();									// 윈도우 창 생성
	void RegisterWindowClass();						// 윈도우 클래스 생성 함수
	void Logic();									// 로직 실행 함수
	void Tick();									// 매 프레임 들어오는 함수
	void Collision();								// 오브젝트 간 충돌 검사 및 처리
	void Render();									// 화면에 오브젝트 그리기 (DirectX Draw 호출)

	bool InitManager();								// 각종 매니저 초기화


	// static 인 이유 -> 특정 객체 없이 함수 자체만 필요로 하기 때문에.
	static LRESULT CALLBACK WndProc(HWND _hWnd,		// 메시지가 발생한 창 핸들
									UINT _message,	// 메시지 종류 (WM_CLOSE, WM_KEYDOWN)
									WPARAM _wParam,	// 메시지 부가 정보 1 (눌린 키 값)
									LPARAM _lParam);	// 메시지 부가 정보 2 (마우스 좌표)




};


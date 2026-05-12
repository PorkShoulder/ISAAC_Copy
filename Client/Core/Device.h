#pragma once
#include "../Core/Defines.h"
#include "../Common/Vector2D.h"
#include "../Common/Info.h"
#include "../Common/Singleton.h"

//==============================
// Dx11 초기화 및 dx 객체 관리자.
// DX11 랜더링 중심 관리자.
//==============================



class Device : public Singleton<Device>			
{
	SINGLETON(Device)
private:
	ComPtr<ID3D11Device> _device = nullptr;				 // 렌더링에 필요한 DX11 리소스를 생성하는 대표 객체
	ComPtr<ID3D11DeviceContext> _context = nullptr;		 // GPU에 랜더링 명령을 내리는 객체
	ComPtr<IDXGISwapChain> _swapChain = nullptr;			 // 랜더링 된 결과물을 화면에 출력하기 위함 깜빡임 방지
	ComPtr<ID3D11RenderTargetView> _targetView = nullptr;	 // 특정 리소스를 랜더링의 목적지로 정하는 인터페이스
	ComPtr<ID3D11DepthStencilView> _depthStencil = nullptr; // 앞뒤 순서와 특수 마스크 처리를 위한 버퍼

	HWND _hWnd = nullptr;									 // 이 디바이스가 출력할 대상 창.
	FResolution _resolution = {};							 // 윈도우 해상도를 기억하는 용도.
	bool _windowMode = false;								 // 전체화면, 창모드인지를 저장하는 플래그 
	ComPtr<ID2D1RenderTarget>	_target2D;					 // 
	ComPtr<ID2D1Factory>		_factory2D;				 //

public:
	// const가 뒤에 붙는 함수는 이 객체를 안 바꾼다 라는  뜻임.
	ComPtr<ID2D1RenderTarget> GetTarget2D() { return _target2D; }
	ComPtr<ID3D11Device> GetDevice() const { return _device; }						 // 다른 클래스가 버퍼/텍스처/뷰를 만들때 필요함.
	ComPtr<ID3D11DeviceContext> GetContext() const { return _context; }			 // 다른 클래스가 바인딩/드로우/업데이트 할 때 필요하다.
	ComPtr<IDXGISwapChain> GetSwapchain() const { return _swapChain; }				 // 다른 클래스가 백버퍼얻기/화면 모드 변경처리/Present호출 할 때 필요하다.
	ComPtr<ID3D11RenderTargetView> GetTargetView() const { return _targetView; }	 // 렌더 타겟을 설정하거나 확인할 떄 사용가능.
	ComPtr<ID3D11DepthStencilView> GetDepthStencil() const { return _depthStencil; }// 깊이 스텐실 반환 뷰
	//
	bool GetWindowMode() { return _windowMode; }
	const FResolution& GetRS() const { return _resolution; }				// 복사하지 않고 원본을 읽기 전용으로 넘기기 위함.
	//=> 다른곳에서 해상도 크기를 저장하고 그 값을 필요한곳에 써먹음 
	FVector2D GetRSRate() const;													// 윈도우 창 크기와 해상도의 비율을 구하는 함수.
public:
	bool Init(HWND hwnd, uint32 widht, uint32 height, bool windowMode);
	void ClearBackBuffer(float clearColor[4]);								// 매 프레임 시작할 떄 화면을 지정한 색으로 밀어버림.
	void ClearDepthStencil(float depth, uint8 stencil);					// 깊이 버퍼도 매프레임 초기화함.
	void SetTarget();		// 렌더타겟과 스텐실을 GPU 파이프라인에 연결 -> "여기에 그려라"
	void Render();			// 스왑체인의 Present()를 호출해서 백버퍼에 그린 그림을 실제 화면에 출력


	virtual void Destroy() override;
};	
#define DEVICE Device::Instance().GetDevice()
#define CONTEXT Device::Instance().GetContext()


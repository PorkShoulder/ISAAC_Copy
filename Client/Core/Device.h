#pragma once
#include "../Core/Defines.h"
#include "../Common/Singleton.h"

//==============================
// dx11 초기화 및 dx 객체 관리자.
//==============================


class Device : public Singleton<Device>			
{
	SINGLETON(Device)
private:

	// dx11 관련된 리소스를 만드는데 사용되는 가상 어댑터
	// 이를 통해 대부분의 랜더링에 필요한 리소스를 생성함.
	ComPtr<ID3D11Device>			_device = nullptr;

	// GPU에 랜더링 명령을 내리는 객체
	// 랜더링 파이프라인 각 단계에 리소스를 바인딩 합니다.
	// 랜더링 파이프라인 DX11에서 그림그리기 위한 단계
	ComPtr<ID3D11DeviceContext>		_context = nullptr;

	// 페이지 플리핑 처리
	// 랜더링 된 결과물을 화면에 출력하기 위함.
	// 스왑체인
	// 화면에 이미지를 부드럽게 표시하기 위한 버퍼 교체 시스템
	ComPtr<IDXGISwapChain>			_swapChain = nullptr;
	




};


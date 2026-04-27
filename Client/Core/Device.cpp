#include "pch.h"
#include "Device.h"
// #include "../Common/" 로그매니저

//======================================
// DX11 디바이스 생성, 스왑체인, 렌더타겟, 
// 깊이버퍼 초기화 및 매 프레임 렌더링 처리.
//======================================


FVector2D Device::GetRSRate() const
{   // 해상도 비율 계산.


    return FVector2D();
}

bool Device::Init(HWND hwnd, uint32 widht, uint32 height, bool windowMode)
{
    // Dx  관련 초기화 작업
    _hwnd               = hwnd;
    _resolution._width  = widht;
    _resolution._height = height;
    _windowMode         = windowMode;

    // Dx11 BGRA 텍스처 포맷을 사용할 수 있게 해주는 플래그 
    uint32 flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    // D3D_FEATURE_LEVEL 
    // 특정 그래픽 카드가 지원하는 기능의 묶음을 정의하는 열거체
    // D3D_FEATURE_LEVEL_11_0 : DX의 모든 기능을 사용 할 수 있다.
    D3D_FEATURE_LEVEL fLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL fLevelResult;

    // D3D11CrateDevice : Device, Context 객체 생성
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, &fLevel, 1, 
        D3D11_SDK_VERSION, _device.GetAddressOf(), &fLevelResult, _context.GetAddressOf())))
    {
        //LogManager::Instance().Fatal("Create Device Error..!");
        return false;
    }
    
    // 멀티 샘플링
    uint32 sampleCount  = 4;
    uint32 check        = 0;
    
    _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &check);
    if (check < 1)
        sampleCount = 1;
    
    // 스왑 체인 생성
    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferDesc.Width   = widht;                   // 백버퍼 정의 및  가로 세로 해상도
    swapDesc.BufferDesc.Height  = height;                  // 백버퍼 정의 및  가로 세로 해상도
    swapDesc.BufferDesc.Format  = DXGI_FORMAT_R8G8_UNORM;   // 백버퍼의 픽셀 포맷을 정의
    swapDesc.BufferDesc.RefreshRate.Numerator   = 60;   // 화면 주사율
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;    // 화면 주사율
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;                 // 백버퍼랑 화면이 다를 때 스케일링을 어떻게 할지
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 이미지를 화면에 그릴때 가로 한줄을 어떻게 그릴지.
    swapDesc.BufferCount = 1;                               // 백버퍼를 몇개 사용할지
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 백버퍼 용도
    swapDesc.OutputWindow = _hwnd;                         // 출력할 윈도우 핸들
    swapDesc.SampleDesc.Quality = 0;                        // 멀티 샘플링
    swapDesc.SampleDesc.Count   = sampleCount;              // ""
    swapDesc.Windowed = _windowMode;                       // 창모드 or 전체화면
    // 스왑체인이 버퍼를 전환 할 때 사용하는 동작을 정의
    // DXGI_SWAP_EFFECT_DISCARD : 교체 시 이전 내용을 버림
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    
    // IDXGIDvice 얻어오기
    // Device를 DXGI 관점에서 바라본 인터페이스
    ComPtr<IDXGIDevice> dxgiDevice = nullptr;
    _device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());

    // 실제 그래픽 어댑터, 그래픽 카드 또는 GPU
    ComPtr<IDXGIAdapter> adapter = nullptr;
    dxgiDevice->GetParent(__uuidof(IDXGIFactory), (void**)adapter.GetAddressOf());

    //  어댑터, 출력장치, 스왑체인 같은 DXGI 객체를 만드는 공장
    ComPtr<IDXGIFactory> factory = nullptr;
    adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());

    // Dx11 에서 화면에 그릴 화면 준비
    // 스왑체인을 만들 고, 그 안의 백버퍼를 꺼내서 Directx가 실제로 그림을 그릴 렌더 타겟 뷰를 만드는 코드
    if(FAILED(factory->CreateSwapChain(_device.Get(), &swapDesc, _swapChain.GetAddressOf())))
    {
        //화면에 출력할 버퍼 시스템을 생성하는 함수.
        return false;
    }
    // 스왑체인이 가지고 있는 화면 출력용 2D 텍스처를 받을 변수.
    ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf())))
    {
        //화면에 나갈 최종 그림이 저장되는 2D텍스처
        return false;
    }
    // 특정 텍스처를 렌더링 결과를 기록할 대상으로 사용할 수 있게 View를 생성하는 함수입니다.
    if (FAILED(_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _targetView.GetAddressOf())))
    {
        // 백버퍼 텍스처에 바로 그림을 그리지 않는다.
        //LogManager::Instance().Fatal("CreateRenderTargetView Error!");
        return false;
    }
    
    
    
    D3D11_TEXTURE2D_DESC depthDesc = {};    // 깊이/스텐실용 텍스처 설정
    depthDesc.Width     = widht;           // 백버퍼와 같은 가로크기  
    depthDesc.Height    = height;          // 백버퍼와 같은 세로크기
    depthDesc.ArraySize = 1;                // 텍스처 1장만 사용
    depthDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;    // 깊이 값 24비트 + 스텐실 값 8비트형식으로 만든다.
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;         // 깊이/스텐실 용도로 쓰겠다.
    depthDesc.Usage     = D3D11_USAGE_DEFAULT;              // GPU가 읽고 쓰기 가능.
    depthDesc.SampleDesc.Count = sampleCount;               // 멀티 샘플링(위에서 체크한 값)
    depthDesc.SampleDesc.Quality = 0;                         
    depthDesc.MipLevels = 1;                                //미니맵 안씀 (축소 텍스처 필요없음)
    
    ComPtr<ID3D11Texture2D> depthBuffer = nullptr;
    if (FAILED(_device->CreateTexture2D(&depthDesc, nullptr, depthBuffer.GetAddressOf())))
    {
        //LogManager::Instance().Fatal("CreateTexture2D Error!");
        return false;
    }

    if (FAILED(_device->CreateDepthStencilView(depthBuffer.Get(), nullptr, _depthStencil.GetAddressOf())))
    {
        //LogManager::Instance().Fatal("CreateDepthStencilView Error!");
        return false;
    }

    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<float>(widht);      // 화면 가로
    vp.Height = static_cast<float>(height);    // 화면 세로
    vp.MaxDepth = 1;                            // 깊이 범위 최대값
    _context->RSSetViewports(1, &vp);          // 래스터라이저에 설정.
    return true;
}

// 화면 지우기
void Device::ClearBackBuffer(float _clearColor[4])
{
    
    _context->ClearRenderTargetView(_targetView.Get(), _clearColor);
}

// 깊이 스텐실 버퍼 초기화
void Device::ClearDepthStencil(float _depth, uint8 _stencil)
{
    
    _context->ClearDepthStencilView(_depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, _depth, _stencil);
}
//렌더링 목적지 지정
void Device::SetTarget()
{
    _context->OMSetRenderTargets(1, _targetView.GetAddressOf(), _depthStencil.Get());
}

void Device::Render()
{
    //랜더링 파이프라인
    //IA(input assembler)
    //랜더링에 필요한 리소스 입력

    //vertex shader
    //정점(vertex) 당 연산 수행

    //레스터 라이저
    //정점쉐이더에서 반환한 정점의 위치를 기록
    //정점 내부에 들어오는 픽셀을 계산해서 픽셀 쉐이더로 연계

    //픽셀 쉐이더
    //레스터 라이저를 거쳐서, 호출 될 픽셀마다 실행 됨.
    //레스터 라이저를 거친 정점들의 위치에 있는 픽셀에 색깔이 나옴

    //OM(Outpuy merge)
    //픽셀쉐이더 에서 리턴한 값이 , OM단계에 있는 랜더타겟, 뎁스스탠실타겟에 기록

    _swapChain->Present(0, 0);
}

void Device::Destroy()
{
}

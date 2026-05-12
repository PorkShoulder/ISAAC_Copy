#include "pch.h"
#include "Device.h"
#include "Common/LogManager.h"

FVector2D Device::GetRSRate() const
{
    RECT windowRC;
    GetClientRect(_hWnd, &windowRC);

    //가로
    float width = static_cast<float>(windowRC.right - windowRC.left);

    //세로
    float height = static_cast<float>(windowRC.bottom - windowRC.top);

    return FVector2D(_resolution._width / width, _resolution._height / height);
}

void Device::Destroy()
{
}

bool Device::Init(HWND hwnd, uint32 widht, uint32 height, bool windowMode)
{
    //dx 관련 초기화 작업
    _hWnd = hwnd;
    _resolution._width = widht;
    _resolution._height = height;
    _windowMode = windowMode;

    //DX11이 BGRA 텍스쳐 포맷을 사용할 수 있게 해주는 플래그
    uint32 flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
    flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    //D3D_FEATURE_LEVEL 
    //특정 그래픽 카드가 지원하는 기능의 묶음을 정의하는 열거체
    //D3D_FEATURE_LEVEL_11_0 : DX11의 모든 기능을 사용할 수 있다.
    D3D_FEATURE_LEVEL fLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL fLevelResult;

    //D3D11CreateDevice : Device, context 객체 생성
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, flag, &fLevel, 1, D3D11_SDK_VERSION, _device.GetAddressOf(), &fLevelResult, _context.GetAddressOf())))
    {
        LogManager::Instance().Fatal("Create Device Error..!");
        return false;
    }

    //멀티샘플링
    uint32 sampleCount = 4;
    uint32 check = 0;
    _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &check);

    if (check < 1)
        sampleCount = 1;

    //swapChain 생성
    DXGI_SWAP_CHAIN_DESC swapDesc = {};

    //BufferDesc : 백버퍼에 대한 정의
    //BufferDesc.Width  : 백버퍼 가로 해상도
    swapDesc.BufferDesc.Width = widht;

    //BufferDesc.Width  : 백버퍼 세로 해상도
    swapDesc.BufferDesc.Height = height;

    //백버퍼의 픽셀 포맷을 정의
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    //화면 주사율
    swapDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapDesc.BufferDesc.RefreshRate.Denominator = 1;

    //백버퍼랑 화면이 다를 때 스케일링을 어떻게 할지
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //이미지를 화면에 그릴때 가로 한줄을 어떻게 그릴지
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    //백버퍼를 몇개 사용할 지
    swapDesc.BufferCount = 1;

    //백버퍼 용도
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    //출력할 윈도우 핸들
    swapDesc.OutputWindow = _hWnd;

    //멀티 샘플링
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.SampleDesc.Count = sampleCount;

    //창모드로 할껀지, 전체 화면 할껀지
    swapDesc.Windowed = _windowMode;

    //스왑체인이 버퍼를 전환 할 때 사용하는 동작을 정의
    //DXGI_SWAP_EFFECT_DISCARD : 이전 내용을 무시하고 전환
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    //IDXGIDevice 얻어오기
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = nullptr;
    _device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice.GetAddressOf());

    //물리적인 그래픽 하드웨어를 가져온다.
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter = nullptr;
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)adapter.GetAddressOf());

    //그래픽 하드웨어와 관련된 dxgi를 만들어줌
    Microsoft::WRL::ComPtr<IDXGIFactory> factory = nullptr;
    adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf());

    if (FAILED(factory->CreateSwapChain(_device.Get(), &swapDesc, _swapChain.GetAddressOf())))
    {
        LogManager::Instance().Fatal("CreateSwapChain Error..!");
        return false;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
    if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf())))
    {
        LogManager::Instance().Fatal("_swapChain->GetBuffer Error!");
        return false;
    }

    if (FAILED(_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _targetView.GetAddressOf())))
    {
        LogManager::Instance().Fatal("CreateRenderTargetView Error!");
        return false;
    }

    //depth stencil 버퍼
    //gpu메모리에 존재하는 2d텍스쳐
    //깊이 : 각 픽셀이 카메라부터 얼마나 떨어져있는지 (0~1)
    //스텐실 : 각 픽셀마다 정수 값을 저장(0~255)
    //스텐실을 값을 통해 프로그래머가 해당 픽셀을 어떻게 다룰 지 정합니다.

    //깊이, 스텐실 버퍼를 만들어주기 위한 texture 생성
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = widht;
    depthDesc.Height = height;
    depthDesc.ArraySize = 1;

    //D24 : 깊이에 24비트를 0~1로
    //S8  : 스텐실에 8비트를 (0~255)
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;

    depthDesc.SampleDesc.Count = sampleCount;
    depthDesc.SampleDesc.Quality = 0;
    depthDesc.MipLevels = 1;

    ComPtr<ID3D11Texture2D> depthBuffer = nullptr;
    if (FAILED(_device->CreateTexture2D(&depthDesc, nullptr, depthBuffer.GetAddressOf())))
    {
        LogManager::Instance().Fatal("CreateTexture2D Error!");
        return false;
    }

    if (FAILED(_device->CreateDepthStencilView(depthBuffer.Get(), nullptr, _depthStencil.GetAddressOf())))
    {
        LogManager::Instance().Fatal("CreateDepthStencilView Error!");
        return false;
    }

    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<float>(widht);
    vp.Height = static_cast<float>(height);
    vp.MaxDepth = 1;

    _context->RSSetViewports(1, &vp);


    //font 출력을 위한 2D 초기화
    //D2D1CreateFactory : 2D팩토리 생성
    //D2D1_FACTORY_TYPE_MULTI_THREADED : 멀티쓰레드 환경
    //D2D1_FACTORY_TYPE_SINGLE_THREADED : 싱글쓰레드 환경
    if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, _factory2D.GetAddressOf())))
        return false;

    //위에서 생성한 3D backbuffer와 2D backbuffer를 연결
    ComPtr<IDXGISurface> backSurface = nullptr;

    _swapChain->GetBuffer(0, IID_PPV_ARGS(backSurface.GetAddressOf()));

    //랜더타겟을 생성
    D2D1_RENDER_TARGET_PROPERTIES prob = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

    if (FAILED(_factory2D->CreateDxgiSurfaceRenderTarget(backSurface.Get(), prob, _target2D.GetAddressOf())))
        return false;

    return true;
}

void Device::ClearBackBuffer(float clearColor[4])
{
    //백버퍼 매개변수로 들어온 색상으로 만듭니다.
    _context->ClearRenderTargetView(_targetView.Get(), clearColor);
}

void Device::ClearDepthStencil(float depth, uint8 stencil)
{
    //뎁스/스탠실 뷰를 들어온 매개변수로 밀어줍니다.
    _context->ClearDepthStencilView(_depthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void Device::SetTarget()
{
    //랜더링 파이프라인 과정에서 마지막에 그림을 출력시킬 목적지를 지정
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

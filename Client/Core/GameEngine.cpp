#include "pch.h"
#include "GameEngine.h"
#include "DirectoryManager.h"
#include "TimeManager.h"
#include "Device.h"
#include "AssetManager.h"
#include "Defines.h"

#include "../Input/Input.h"
#include "../Input/InputSystem.h"
#include "../Shader/ShaderManager.h"
#include "../Common/LogManager.h"
#include "../World/World.h"
#include "../Collision/CollisionProfileManager.h"

#include "../Render/RenderManager.h"
#include "Editor/imgui_impl_win32.h"












void GameEngine::Destroy()
{
	_world->Destroy();

	AssetManager::Instance().Destroy();
	ShaderManager::Instance().Destroy();

#ifdef _USE_MEMORY_POOL
	MemoryPool::Instance().Destroy();
#elif defined(_USE_OBJECT_POOL)
	ObjectPool::Instance().Destroy();
#endif

	_input->Destroy();
	InputSystem::Instance().Destroy();


	LogManager::Instance().Destroy();
	DirectoryManager::Instance().Destroy();

}

bool GameEngine::Init(HINSTANCE inst, const wchar_t* name)
{
	_hInst = inst;


	// lstrcpy(복사대상, 원본문자열);
	lstrcpy(_className, name);                  // 클래스 이름
	lstrcpy(_titleName, L"ISAAC_Copy");         // 윈도우 타이틀 이름 (이름 바꿨음)

	RegisterWindowClass();                      // 클래스 정보 등록 
	if (!Create())
		return false;

	_hdc = GetDC(_hWnd);                     // Dx에서 그릴때 필요할 수도 있으니까 가져옴
	g_isRun = InitManager();

	_world = New<World>();
	_world->Init("");


	return true;
}

int GameEngine::Run()
{
	MSG msg = {};

	while (g_isRun)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}
	return (int)msg.wParam;
}

Ptr<class World> GameEngine::GetWorld() const
{
	return _world;
}

// 레지스터에 등록한 정보를 바탕으로 실제 창을 만든다.
bool GameEngine::Create()
{
	// 창 만들기 
	_hWnd = CreateWindowW(_className, _titleName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, _hInst, nullptr);

	//창을 만들기에 실패 할경우 진입 -> 안전장치
	if (!_hWnd)
	{
		return false;
	}

	// 윈도우 창 크기설정
	RECT windowRect = { 0, 0, 1280, 720 };

	// (윈도우 창 크기설정, 메뉴바) 이를 바탕으로 제외된 크기 계산해서 다시 넣어줌
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	// 계산된 크기를 기반으로 크기를 조절함.
	SetWindowPos(_hWnd, HWND_TOPMOST, 0, 0,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		SWP_NOMOVE | SWP_NOZORDER);
	//NOMOVE : 위치를 안바꾼다. NOZORDER : 창 순서는 안바꾼다.

// 최종적으로 생성한 윈도우 창을 화면에 보이도록 설정
	ShowWindow(_hWnd, SW_SHOW);
	UpdateWindow(_hWnd);

	return true;
}



// 앞으로 만들 창이 어떤 성격의 창인지 운영체제(Windows)에 등록하는 함수 .
// 창 자체를 만드는 함수가 아니라 창을 만들기 전에 창의 설계도(클래스 정보)를 등록하는 단계

void GameEngine::RegisterWindowClass()
{

	WNDCLASSEXW wcex;                       // 생성시킬 윈도우(창)의 옵션 설정
	wcex.cbSize = sizeof(WNDCLASSEX);       // 윈도우 클래스 구조체 크기 -> 반드시 지정되어야함.
	wcex.style = CS_HREDRAW | CS_VREDRAW;   // 화면에 출력 가능한 영역을 클라이언트 영역, 클라이언트 영역이 변경될 경우 다시 그려준다.
	wcex.lpfnWndProc = WndProc;             // 함수 포인터로 윈도우 메시지 처리함수
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInst;                // 운영체제가 부여해준 ID
	wcex.hIcon = LoadIcon(_hInst, MAKEINTRESOURCE(IDI_CLIENT)); // 리소스 아이콘
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);              // 마우스 포인터 변경
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;                                // 메뉴 리소스 이름 -> 창 상단 메뉴바를 자동으로 붙일 수 있음(파일,편집,도움말)
	wcex.lpszClassName = _className;                            // 윈도우 클래스 이름.
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));    // 작은 크기의 아이콘 


	RegisterClassExW(&wcex);

}
void GameEngine::Logic()
{
	//logic
	//time
	//dir
	//input
	//log
	float deltaTime = TimeManager::Instance().Tick();
	Tick(deltaTime);
	Collision(deltaTime);
	Render(deltaTime);

	/* float fps = TimeManager::Instance().GetFPS();
	 LogManager::Instance().Debug("FPS : ", fps);*/
}

void GameEngine::Tick(float deltaTime)
{
	_input->Tick(deltaTime); //실제 물리적인 키가 어떤 상태인지 갱신

	InputSystem::Instance().Tick(deltaTime); //활성화된 입력컨텍스트의 키 상태를 갱신(down? hold? up?)

	SOUND_MANAGER->Tick();

	_world->Tick(deltaTime);
	RenderManager::Instance().Tick(deltaTime);
}

void GameEngine::Collision(float deltaTime)
{
	_world->Collision(deltaTime);
}

void GameEngine::Render(float deltaTime)
{
	Device::Instance().ClearBackBuffer(_clearColor);
	Device::Instance().ClearDepthStencil(1.f, 0);
	Device::Instance().SetTarget();

	//Device::Instance().GetTarget2D()->BeginDraw();

	_world->Render(deltaTime);
	RenderManager::Instance().Render(deltaTime);
	_world->RenderUI(deltaTime);

	//Device::Instance().GetTarget2D()->EndDraw();

	Device::Instance().Render();
}

bool GameEngine::InitManager()
{
	DirectoryManager::Instance().Init();
	LogManager::Instance().Init();
	if (false == TimeManager::Instance().Init())
	{
		LogManager::Instance().Fatal("TimeManager Init Error!");
		return false;
	}

	if (false == Device::Instance().Init(_hWnd, 1280, 720, true))
	{
		LogManager::Instance().Fatal("Device Init Error!");
		return false;
	}

	if (false == ShaderManager::Instance().Init())
		return false;

	if (false == AssetManager::Instance().Init())
		return false;

	_input = New<Input>();
	_input->Init();

	if (false == InputSystem::Instance().Init(_input))
		return false;

	if (false == CollisionProfileManager::Instance().Init())
		return false;

	if (false == RenderManager::Instance().Init())
		return false;

	return true;
}

LRESULT GameEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		g_isRun = false;
		PostQuitMessage(0);
	}
	break;
	case WM_SIZE:
	{
		int width = LOWORD(lParam);
		int height = HIWORD(lParam);
		if (_onWindowSizeChanged)
			_onWindowSizeChanged(width, height);
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

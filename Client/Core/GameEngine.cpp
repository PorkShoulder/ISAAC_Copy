#include "pch.h"
#include "GameEngine.h"


bool GameEngine::m_bIsRun = false;

void GameEngine::Destroy()
{

}

bool GameEngine::Init(HINSTANCE inst, const wchar_t* name)
{
    m_hInst = inst;

    
    // lstrcpy(복사대상, 원본문자열);
    lstrcpy(m_className, name);                  // 클래스 이름
    lstrcpy(m_titleName, L"ISAAC_Copy");         // 윈도우 타이틀 이름 (이름 바꿨음)
    
    RegisterWindowClass();                      // 클래스 정보 등록 
    if (!Create())
        return false;
        
    m_hdc    = GetDC(m_hWnd);                     // Dx에서 그릴때 필요할 수도 있으니까 가져옴
    m_bIsRun = InitManager();                    // 
    //_world = New<world>();
    //_world->Init("")


    return Run();
}

int GameEngine::Run()
{
    MSG msg = {};
    
    while (m_bIsRun)
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

// 레지스터에 등록한 정보를 바탕으로 실제 창을 만든다.
bool GameEngine::Create()
{
    // 창 만들기 
    m_hWnd = CreateWindowW(m_className, m_titleName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);
    
    //창을 만들기에 실패 할경우 진입 -> 안전장치
    if (!m_hWnd)
    {
        return false;
    }

    // 윈도우 창 크기설정
    RECT windowRect = { 0, 0, 1280, 720 };                      
    
    // (윈도우 창 크기설정, 메뉴바) 이를 바탕으로 제외된 크기 계산해서 다시 넣어줌
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);
    
    // 계산된 크기를 기반으로 크기를 조절함.
    SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0,
                 windowRect.right - windowRect.left,
                 windowRect.bottom - windowRect.top,
                 SWP_NOMOVE | SWP_NOZORDER);        
                //NOMOVE : 위치를 안바꾼다. NOZORDER : 창 순서는 안바꾼다.
    
    // 최종적으로 생성한 윈도우 창을 화면에 보이도록 설정
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

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
    wcex.hInstance = m_hInst;                // 운영체제가 부여해준 ID
    wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_CLIENT)); // 리소스 아이콘
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);              // 마우스 포인터 변경
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);             
    wcex.lpszMenuName = nullptr;                                // 메뉴 리소스 이름 -> 창 상단 메뉴바를 자동으로 붙일 수 있음(파일,편집,도움말)
    wcex.lpszClassName = m_className;                            // 윈도우 클래스 이름.
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));    // 작은 크기의 아이콘 


    RegisterClassExW(&wcex);

}

void GameEngine::Logic()
{
}

void GameEngine::Tick()
{
}

void GameEngine::Collision()
{
}

void GameEngine::Render()
{
}

bool GameEngine::InitManager()
{
    return true;
}

LRESULT GameEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:                          // 창을 다시 그리는 메시지창.  
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);    // 창을 그리기 시작.
        EndPaint(hWnd, &ps);                // 창을 그리기 끝.
    }
    break;
    case WM_DESTROY:                        // 창이 닫혔다 라는 메시지 X버튼을 누리면 false로 게임루프를 멈춤.
    {
        m_bIsRun = false;                    // 게임루프 종료
        PostQuitMessage(0);                 // OS한테 프로그램 종료한다 고 알려줌.
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


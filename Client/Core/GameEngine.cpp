#include "pch.h"
#include "GameEngine.h"


bool GameEngine::_bIsRun = false;

void GameEngine::Destroy()
{

}

bool GameEngine::Init(HINSTANCE inst, const wchar_t* name)
{
    
    
    // lstrcpy(복사대상, 원본문자열);
    lstrcpy(_className, name);                  // 클래스 이름
    lstrcpy(_titleName, L"ISAAC_Copy");         // 윈도우 타이틀 이름 (이름 바꿨음)

        

}

int GameEngine::Run()
{
    MSG msg = {};
    
    while (_bIsRun)
    {
        
    }
}

bool GameEngine::Create()
{
    
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
    return false;
}

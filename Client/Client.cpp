
#include "pch.h"
#include "framework.h"
#include "Client.h"



#include "Core/Defines.h"
#include "Core/GameEngine.h"

#include "Editor/EditorEngine.h"

#define MAX_LOADSTRING 100

bool g_isRun = false;




//EntryPoint : 진입점
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GameEngine::Instance().Init(hInstance, L"ISAAC");
    
#ifdef _EDITOR
    EditorEngine::Instance().Init();
    EditorEngine::Instance().Run();
#else 
    GameEngine::Instance().Run();
#endif // _EDITOR 
    
    
    GameEngine::Instance().Destroy();

#ifdef _EDITOR
    EditorEngine::Instance().Destroy();
#endif

    return TRUE;
}

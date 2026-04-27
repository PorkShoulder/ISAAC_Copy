#include "pch.h"
#include "framework.h"
#include "Client.h"

#include "Core/GameEngine.h"


int APIENTRY wWinMain(_In_ HINSTANCE    hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR        lpCmdLine,
                     _In_ int           nCmdShow)

{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    GameEngine::Instance().Init(hInstance, L" ");
    GameEngine::Instance().Destroy();
    
    
    return TRUE;
}


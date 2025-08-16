#pragma once

#define BUILD_DDRAW 0
#define BUILD_WINMM 1
#define BUILD_DSOUND 0
#define BUILD_WS2_32 0

#if BUILD_WINMM
#include "pragma/winmm.h"
#elif BUILD_DDRAW
#include "pragma/ddraw.h"
#elif BUILD_DSOUND
#include "pragma/dsound.h"
#elif BUILD_WS2_32
#include "pragma/ws2_32.h"
#else
#error "Unspecified build mode"
#endif


#include "windows.h"
#include <Psapi.h>
#include "game-manager.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: 
        //Pin self to prevent unloading
        char path[MAX_PATH];
        if (GetModuleFileNameA(hModule, path, MAX_PATH)) {
            LoadLibraryA(path);
        }

        if(!gameManager.init())
        {
            MessageBox(NULL, L"Failed to initialize BuggerUp", L"Fatal Error", MB_ICONERROR);
            ExitProcess(0);
        }
        break;
    case DLL_PROCESS_DETACH:
        gameManager.shutdown();
        break;
    }

    return TRUE;
}

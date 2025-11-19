#pragma once

#include <Windows.h>
#include <Psapi.h>
#include "game-manager.h"

#include "proxy/winmm.h"
#include "proxy/dsound.h"
#include "proxy/ws2_32.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		winmm_InitProxies();
		dsound_InitProxies();
		ws2_32_InitProxies();

        //Pin self to prevent unloading
        char path[MAX_PATH];
        if (GetModuleFileNameA(hModule, path, MAX_PATH)) {
            LoadLibraryA(path);
        }

        if(!gameManager.init())
        {
            MessageBoxA(NULL, "Failed to initialize BuggerUp", "Fatal Error", MB_ICONERROR);
            ExitProcess(0);
        }
        break;
    case DLL_PROCESS_DETACH:
        gameManager.shutdown();
        break;
    }

    return TRUE;
}

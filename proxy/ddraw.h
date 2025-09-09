// Auto-generated proxy for ddraw.dll

static HMODULE ddraw_hOriginal = NULL;
static FARPROC* ddraw_JumpTable = NULL;
static const char** ddraw_FunctionNames = NULL;
static int ddraw_ExportCount = 0;

void ddraw_InitProxies() {
    if (ddraw_hOriginal) return;

    char path[MAX_PATH];
    GetSystemDirectoryA(path, MAX_PATH);
    strcat_s(path, "\\ddraw.dll");
    ddraw_hOriginal = LoadLibraryA(path);

    ddraw_ExportCount = 20;
    ddraw_FunctionNames = new const char*[ddraw_ExportCount];
    ddraw_JumpTable = new FARPROC[ddraw_ExportCount];
    ddraw_FunctionNames[0] = "AcquireDDThreadLock";
    ddraw_FunctionNames[1] = "CompleteCreateSysmemSurface";
    ddraw_FunctionNames[2] = "D3DParseUnknownCommand";
    ddraw_FunctionNames[3] = "DDGetAttachedSurfaceLcl";
    ddraw_FunctionNames[4] = "DDInternalLock";
    ddraw_FunctionNames[5] = "DDInternalUnlock";
    ddraw_FunctionNames[6] = "DSoundHelp";
    ddraw_FunctionNames[7] = "DirectDrawCreate";
    ddraw_FunctionNames[8] = "DirectDrawCreateClipper";
    ddraw_FunctionNames[9] = "DirectDrawCreateEx";
    ddraw_FunctionNames[10] = "DirectDrawEnumerateA";
    ddraw_FunctionNames[11] = "DirectDrawEnumerateExA";
    ddraw_FunctionNames[12] = "DirectDrawEnumerateExW";
    ddraw_FunctionNames[13] = "DirectDrawEnumerateW";
    ddraw_FunctionNames[14] = "GetDDSurfaceLocal";
    ddraw_FunctionNames[15] = "GetOLEThunkData";
    ddraw_FunctionNames[16] = "GetSurfaceFromDC";
    ddraw_FunctionNames[17] = "RegisterSpecialCase";
    ddraw_FunctionNames[18] = "ReleaseDDThreadLock";
    ddraw_FunctionNames[19] = "SetAppCompatData";

    for (int i = 0; i < ddraw_ExportCount; i++) {
        ddraw_JumpTable[i] = GetProcAddress(ddraw_hOriginal, ddraw_FunctionNames[i]);
    }
}

extern "C" {
__declspec(naked) void ddraw_proxy_AcquireDDThreadLock() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 0
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_CompleteCreateSysmemSurface() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 1
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_D3DParseUnknownCommand() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 2
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DDGetAttachedSurfaceLcl() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 3
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DDInternalLock() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 4
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DDInternalUnlock() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 5
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DSoundHelp() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 6
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DirectDrawCreate() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 7
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DirectDrawCreateClipper() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 8
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DirectDrawCreateEx() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 9
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DirectDrawEnumerateA() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 10
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DirectDrawEnumerateExA() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 11
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DirectDrawEnumerateExW() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 12
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_DirectDrawEnumerateW() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 13
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_GetDDSurfaceLocal() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 14
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_GetOLEThunkData() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 15
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_GetSurfaceFromDC() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 16
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_RegisterSpecialCase() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 17
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_ReleaseDDThreadLock() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 18
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void ddraw_proxy_SetAppCompatData() {
    __asm {
        mov eax, ddraw_JumpTable
        mov ecx, 19
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
}
#pragma comment(linker,"/export:AcquireDDThreadLock=_ddraw_proxy_AcquireDDThreadLock")
#pragma comment(linker,"/export:CompleteCreateSysmemSurface=_ddraw_proxy_CompleteCreateSysmemSurface")
#pragma comment(linker,"/export:D3DParseUnknownCommand=_ddraw_proxy_D3DParseUnknownCommand")
#pragma comment(linker,"/export:DDGetAttachedSurfaceLcl=_ddraw_proxy_DDGetAttachedSurfaceLcl")
#pragma comment(linker,"/export:DDInternalLock=_ddraw_proxy_DDInternalLock")
#pragma comment(linker,"/export:DDInternalUnlock=_ddraw_proxy_DDInternalUnlock")
#pragma comment(linker,"/export:DSoundHelp=_ddraw_proxy_DSoundHelp")
#pragma comment(linker,"/export:DirectDrawCreate=_ddraw_proxy_DirectDrawCreate")
#pragma comment(linker,"/export:DirectDrawCreateClipper=_ddraw_proxy_DirectDrawCreateClipper")
#pragma comment(linker,"/export:DirectDrawCreateEx=_ddraw_proxy_DirectDrawCreateEx")
#pragma comment(linker,"/export:DirectDrawEnumerateA=_ddraw_proxy_DirectDrawEnumerateA")
#pragma comment(linker,"/export:DirectDrawEnumerateExA=_ddraw_proxy_DirectDrawEnumerateExA")
#pragma comment(linker,"/export:DirectDrawEnumerateExW=_ddraw_proxy_DirectDrawEnumerateExW")
#pragma comment(linker,"/export:DirectDrawEnumerateW=_ddraw_proxy_DirectDrawEnumerateW")
#pragma comment(linker,"/export:GetDDSurfaceLocal=_ddraw_proxy_GetDDSurfaceLocal")
#pragma comment(linker,"/export:GetOLEThunkData=_ddraw_proxy_GetOLEThunkData")
#pragma comment(linker,"/export:GetSurfaceFromDC=_ddraw_proxy_GetSurfaceFromDC")
#pragma comment(linker,"/export:RegisterSpecialCase=_ddraw_proxy_RegisterSpecialCase")
#pragma comment(linker,"/export:ReleaseDDThreadLock=_ddraw_proxy_ReleaseDDThreadLock")
#pragma comment(linker,"/export:SetAppCompatData=_ddraw_proxy_SetAppCompatData")

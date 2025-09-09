// Auto-generated proxy for dsound.dll

static HMODULE dsound_hOriginal = NULL;
static FARPROC* dsound_JumpTable = NULL;
static const char** dsound_FunctionNames = NULL;
static int dsound_ExportCount = 0;

void dsound_InitProxies() {
    if (dsound_hOriginal) return;

    char path[MAX_PATH];
    GetSystemDirectoryA(path, MAX_PATH);
    strcat_s(path, "\\dsound.dll");
    dsound_hOriginal = LoadLibraryA(path);

    dsound_ExportCount = 10;
    dsound_FunctionNames = new const char*[dsound_ExportCount];
    dsound_JumpTable = new FARPROC[dsound_ExportCount];
    dsound_FunctionNames[0] = "DirectSoundCaptureCreate";
    dsound_FunctionNames[1] = "DirectSoundCaptureCreate8";
    dsound_FunctionNames[2] = "DirectSoundCaptureEnumerateA";
    dsound_FunctionNames[3] = "DirectSoundCaptureEnumerateW";
    dsound_FunctionNames[4] = "DirectSoundCreate";
    dsound_FunctionNames[5] = "DirectSoundCreate8";
    dsound_FunctionNames[6] = "DirectSoundEnumerateA";
    dsound_FunctionNames[7] = "DirectSoundEnumerateW";
    dsound_FunctionNames[8] = "DirectSoundFullDuplexCreate";
    dsound_FunctionNames[9] = "GetDeviceID";

    for (int i = 0; i < dsound_ExportCount; i++) {
        dsound_JumpTable[i] = GetProcAddress(dsound_hOriginal, dsound_FunctionNames[i]);
    }
}

extern "C" {
__declspec(naked) void dsound_proxy_DirectSoundCaptureCreate() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 0
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_DirectSoundCaptureCreate8() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 1
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_DirectSoundCaptureEnumerateA() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 2
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_DirectSoundCaptureEnumerateW() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 3
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_DirectSoundCreate() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 4
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_DirectSoundCreate8() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 5
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_DirectSoundEnumerateA() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 6
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_DirectSoundEnumerateW() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 7
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_DirectSoundFullDuplexCreate() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 8
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void dsound_proxy_GetDeviceID() {
    __asm {
        mov eax, dsound_JumpTable
        mov ecx, 9
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
}
#pragma comment(linker,"/export:DirectSoundCaptureCreate=_dsound_proxy_DirectSoundCaptureCreate")
#pragma comment(linker,"/export:DirectSoundCaptureCreate8=_dsound_proxy_DirectSoundCaptureCreate8")
#pragma comment(linker,"/export:DirectSoundCaptureEnumerateA=_dsound_proxy_DirectSoundCaptureEnumerateA")
#pragma comment(linker,"/export:DirectSoundCaptureEnumerateW=_dsound_proxy_DirectSoundCaptureEnumerateW")
#pragma comment(linker,"/export:DirectSoundCreate=_dsound_proxy_DirectSoundCreate")
#pragma comment(linker,"/export:DirectSoundCreate8=_dsound_proxy_DirectSoundCreate8")
#pragma comment(linker,"/export:DirectSoundEnumerateA=_dsound_proxy_DirectSoundEnumerateA")
#pragma comment(linker,"/export:DirectSoundEnumerateW=_dsound_proxy_DirectSoundEnumerateW")
#pragma comment(linker,"/export:DirectSoundFullDuplexCreate=_dsound_proxy_DirectSoundFullDuplexCreate")
#pragma comment(linker,"/export:GetDeviceID=_dsound_proxy_GetDeviceID")

// Auto-generated proxy for winmm.dll

static HMODULE winmm_hOriginal = NULL;
static FARPROC* winmm_JumpTable = NULL;
static const char** winmm_FunctionNames = NULL;
static int winmm_ExportCount = 0;

void winmm_InitProxies() {
    if (winmm_hOriginal) return;

    char path[MAX_PATH];
    GetSystemDirectoryA(path, MAX_PATH);
    strcat_s(path, "\\winmm.dll");
    winmm_hOriginal = LoadLibraryA(path);

    winmm_ExportCount = 192;
    winmm_FunctionNames = new const char*[winmm_ExportCount];
    winmm_JumpTable = new FARPROC[winmm_ExportCount];
    winmm_FunctionNames[0] = "CloseDriver";
    winmm_FunctionNames[1] = "DefDriverProc";
    winmm_FunctionNames[2] = "DriverCallback";
    winmm_FunctionNames[3] = "DrvGetModuleHandle";
    winmm_FunctionNames[4] = "GetDriverModuleHandle";
    winmm_FunctionNames[5] = "NotifyCallbackData";
    winmm_FunctionNames[6] = "OpenDriver";
    winmm_FunctionNames[7] = "PlaySound";
    winmm_FunctionNames[8] = "PlaySoundA";
    winmm_FunctionNames[9] = "PlaySoundW";
    winmm_FunctionNames[10] = "SendDriverMessage";
    winmm_FunctionNames[11] = "WOW32DriverCallback";
    winmm_FunctionNames[12] = "WOW32ResolveMultiMediaHandle";
    winmm_FunctionNames[13] = "WOWAppExit";
    winmm_FunctionNames[14] = "aux32Message";
    winmm_FunctionNames[15] = "auxGetDevCapsA";
    winmm_FunctionNames[16] = "auxGetDevCapsW";
    winmm_FunctionNames[17] = "auxGetNumDevs";
    winmm_FunctionNames[18] = "auxGetVolume";
    winmm_FunctionNames[19] = "auxOutMessage";
    winmm_FunctionNames[20] = "auxSetVolume";
    winmm_FunctionNames[21] = "joy32Message";
    winmm_FunctionNames[22] = "joyConfigChanged";
    winmm_FunctionNames[23] = "joyGetDevCapsA";
    winmm_FunctionNames[24] = "joyGetDevCapsW";
    winmm_FunctionNames[25] = "joyGetNumDevs";
    winmm_FunctionNames[26] = "joyGetPos";
    winmm_FunctionNames[27] = "joyGetPosEx";
    winmm_FunctionNames[28] = "joyGetThreshold";
    winmm_FunctionNames[29] = "joyReleaseCapture";
    winmm_FunctionNames[30] = "joySetCapture";
    winmm_FunctionNames[31] = "joySetThreshold";
    winmm_FunctionNames[32] = "mci32Message";
    winmm_FunctionNames[33] = "mciDriverNotify";
    winmm_FunctionNames[34] = "mciDriverYield";
    winmm_FunctionNames[35] = "mciExecute";
    winmm_FunctionNames[36] = "mciFreeCommandResource";
    winmm_FunctionNames[37] = "mciGetCreatorTask";
    winmm_FunctionNames[38] = "mciGetDeviceIDA";
    winmm_FunctionNames[39] = "mciGetDeviceIDFromElementIDA";
    winmm_FunctionNames[40] = "mciGetDeviceIDFromElementIDW";
    winmm_FunctionNames[41] = "mciGetDeviceIDW";
    winmm_FunctionNames[42] = "mciGetDriverData";
    winmm_FunctionNames[43] = "mciGetErrorStringA";
    winmm_FunctionNames[44] = "mciGetErrorStringW";
    winmm_FunctionNames[45] = "mciGetYieldProc";
    winmm_FunctionNames[46] = "mciLoadCommandResource";
    winmm_FunctionNames[47] = "mciSendCommandA";
    winmm_FunctionNames[48] = "mciSendCommandW";
    winmm_FunctionNames[49] = "mciSendStringA";
    winmm_FunctionNames[50] = "mciSendStringW";
    winmm_FunctionNames[51] = "mciSetDriverData";
    winmm_FunctionNames[52] = "mciSetYieldProc";
    winmm_FunctionNames[53] = "mid32Message";
    winmm_FunctionNames[54] = "midiConnect";
    winmm_FunctionNames[55] = "midiDisconnect";
    winmm_FunctionNames[56] = "midiInAddBuffer";
    winmm_FunctionNames[57] = "midiInClose";
    winmm_FunctionNames[58] = "midiInGetDevCapsA";
    winmm_FunctionNames[59] = "midiInGetDevCapsW";
    winmm_FunctionNames[60] = "midiInGetErrorTextA";
    winmm_FunctionNames[61] = "midiInGetErrorTextW";
    winmm_FunctionNames[62] = "midiInGetID";
    winmm_FunctionNames[63] = "midiInGetNumDevs";
    winmm_FunctionNames[64] = "midiInMessage";
    winmm_FunctionNames[65] = "midiInOpen";
    winmm_FunctionNames[66] = "midiInPrepareHeader";
    winmm_FunctionNames[67] = "midiInReset";
    winmm_FunctionNames[68] = "midiInStart";
    winmm_FunctionNames[69] = "midiInStop";
    winmm_FunctionNames[70] = "midiInUnprepareHeader";
    winmm_FunctionNames[71] = "midiOutCacheDrumPatches";
    winmm_FunctionNames[72] = "midiOutCachePatches";
    winmm_FunctionNames[73] = "midiOutClose";
    winmm_FunctionNames[74] = "midiOutGetDevCapsA";
    winmm_FunctionNames[75] = "midiOutGetDevCapsW";
    winmm_FunctionNames[76] = "midiOutGetErrorTextA";
    winmm_FunctionNames[77] = "midiOutGetErrorTextW";
    winmm_FunctionNames[78] = "midiOutGetID";
    winmm_FunctionNames[79] = "midiOutGetNumDevs";
    winmm_FunctionNames[80] = "midiOutGetVolume";
    winmm_FunctionNames[81] = "midiOutLongMsg";
    winmm_FunctionNames[82] = "midiOutMessage";
    winmm_FunctionNames[83] = "midiOutOpen";
    winmm_FunctionNames[84] = "midiOutPrepareHeader";
    winmm_FunctionNames[85] = "midiOutReset";
    winmm_FunctionNames[86] = "midiOutSetVolume";
    winmm_FunctionNames[87] = "midiOutShortMsg";
    winmm_FunctionNames[88] = "midiOutUnprepareHeader";
    winmm_FunctionNames[89] = "midiStreamClose";
    winmm_FunctionNames[90] = "midiStreamOpen";
    winmm_FunctionNames[91] = "midiStreamOut";
    winmm_FunctionNames[92] = "midiStreamPause";
    winmm_FunctionNames[93] = "midiStreamPosition";
    winmm_FunctionNames[94] = "midiStreamProperty";
    winmm_FunctionNames[95] = "midiStreamRestart";
    winmm_FunctionNames[96] = "midiStreamStop";
    winmm_FunctionNames[97] = "mixerClose";
    winmm_FunctionNames[98] = "mixerGetControlDetailsA";
    winmm_FunctionNames[99] = "mixerGetControlDetailsW";
    winmm_FunctionNames[100] = "mixerGetDevCapsA";
    winmm_FunctionNames[101] = "mixerGetDevCapsW";
    winmm_FunctionNames[102] = "mixerGetID";
    winmm_FunctionNames[103] = "mixerGetLineControlsA";
    winmm_FunctionNames[104] = "mixerGetLineControlsW";
    winmm_FunctionNames[105] = "mixerGetLineInfoA";
    winmm_FunctionNames[106] = "mixerGetLineInfoW";
    winmm_FunctionNames[107] = "mixerGetNumDevs";
    winmm_FunctionNames[108] = "mixerMessage";
    winmm_FunctionNames[109] = "mixerOpen";
    winmm_FunctionNames[110] = "mixerSetControlDetails";
    winmm_FunctionNames[111] = "mmDrvInstall";
    winmm_FunctionNames[112] = "mmGetCurrentTask";
    winmm_FunctionNames[113] = "mmTaskBlock";
    winmm_FunctionNames[114] = "mmTaskCreate";
    winmm_FunctionNames[115] = "mmTaskSignal";
    winmm_FunctionNames[116] = "mmTaskYield";
    winmm_FunctionNames[117] = "mmioAdvance";
    winmm_FunctionNames[118] = "mmioAscend";
    winmm_FunctionNames[119] = "mmioClose";
    winmm_FunctionNames[120] = "mmioCreateChunk";
    winmm_FunctionNames[121] = "mmioDescend";
    winmm_FunctionNames[122] = "mmioFlush";
    winmm_FunctionNames[123] = "mmioGetInfo";
    winmm_FunctionNames[124] = "mmioInstallIOProcA";
    winmm_FunctionNames[125] = "mmioInstallIOProcW";
    winmm_FunctionNames[126] = "mmioOpenA";
    winmm_FunctionNames[127] = "mmioOpenW";
    winmm_FunctionNames[128] = "mmioRead";
    winmm_FunctionNames[129] = "mmioRenameA";
    winmm_FunctionNames[130] = "mmioRenameW";
    winmm_FunctionNames[131] = "mmioSeek";
    winmm_FunctionNames[132] = "mmioSendMessage";
    winmm_FunctionNames[133] = "mmioSetBuffer";
    winmm_FunctionNames[134] = "mmioSetInfo";
    winmm_FunctionNames[135] = "mmioStringToFOURCCA";
    winmm_FunctionNames[136] = "mmioStringToFOURCCW";
    winmm_FunctionNames[137] = "mmioWrite";
    winmm_FunctionNames[138] = "mmsystemGetVersion";
    winmm_FunctionNames[139] = "mod32Message";
    winmm_FunctionNames[140] = "mxd32Message";
    winmm_FunctionNames[141] = "sndPlaySoundA";
    winmm_FunctionNames[142] = "sndPlaySoundW";
    winmm_FunctionNames[143] = "tid32Message";
    winmm_FunctionNames[144] = "timeBeginPeriod";
    winmm_FunctionNames[145] = "timeEndPeriod";
    winmm_FunctionNames[146] = "timeGetDevCaps";
    winmm_FunctionNames[147] = "timeGetSystemTime";
    winmm_FunctionNames[148] = "timeGetTime";
    winmm_FunctionNames[149] = "timeKillEvent";
    winmm_FunctionNames[150] = "timeSetEvent";
    winmm_FunctionNames[151] = "waveInAddBuffer";
    winmm_FunctionNames[152] = "waveInClose";
    winmm_FunctionNames[153] = "waveInGetDevCapsA";
    winmm_FunctionNames[154] = "waveInGetDevCapsW";
    winmm_FunctionNames[155] = "waveInGetErrorTextA";
    winmm_FunctionNames[156] = "waveInGetErrorTextW";
    winmm_FunctionNames[157] = "waveInGetID";
    winmm_FunctionNames[158] = "waveInGetNumDevs";
    winmm_FunctionNames[159] = "waveInGetPosition";
    winmm_FunctionNames[160] = "waveInMessage";
    winmm_FunctionNames[161] = "waveInOpen";
    winmm_FunctionNames[162] = "waveInPrepareHeader";
    winmm_FunctionNames[163] = "waveInReset";
    winmm_FunctionNames[164] = "waveInStart";
    winmm_FunctionNames[165] = "waveInStop";
    winmm_FunctionNames[166] = "waveInUnprepareHeader";
    winmm_FunctionNames[167] = "waveOutBreakLoop";
    winmm_FunctionNames[168] = "waveOutClose";
    winmm_FunctionNames[169] = "waveOutGetDevCapsA";
    winmm_FunctionNames[170] = "waveOutGetDevCapsW";
    winmm_FunctionNames[171] = "waveOutGetErrorTextA";
    winmm_FunctionNames[172] = "waveOutGetErrorTextW";
    winmm_FunctionNames[173] = "waveOutGetID";
    winmm_FunctionNames[174] = "waveOutGetNumDevs";
    winmm_FunctionNames[175] = "waveOutGetPitch";
    winmm_FunctionNames[176] = "waveOutGetPlaybackRate";
    winmm_FunctionNames[177] = "waveOutGetPosition";
    winmm_FunctionNames[178] = "waveOutGetVolume";
    winmm_FunctionNames[179] = "waveOutMessage";
    winmm_FunctionNames[180] = "waveOutOpen";
    winmm_FunctionNames[181] = "waveOutPause";
    winmm_FunctionNames[182] = "waveOutPrepareHeader";
    winmm_FunctionNames[183] = "waveOutReset";
    winmm_FunctionNames[184] = "waveOutRestart";
    winmm_FunctionNames[185] = "waveOutSetPitch";
    winmm_FunctionNames[186] = "waveOutSetPlaybackRate";
    winmm_FunctionNames[187] = "waveOutSetVolume";
    winmm_FunctionNames[188] = "waveOutUnprepareHeader";
    winmm_FunctionNames[189] = "waveOutWrite";
    winmm_FunctionNames[190] = "wid32Message";
    winmm_FunctionNames[191] = "wod32Message";

    for (int i = 0; i < winmm_ExportCount; i++) {
        winmm_JumpTable[i] = GetProcAddress(winmm_hOriginal, winmm_FunctionNames[i]);
    }
}

extern "C" {
__declspec(naked) void winmm_proxy_CloseDriver() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 0
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_DefDriverProc() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 1
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_DriverCallback() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 2
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_DrvGetModuleHandle() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 3
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_GetDriverModuleHandle() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 4
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_NotifyCallbackData() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 5
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_OpenDriver() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 6
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_PlaySound() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 7
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_PlaySoundA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 8
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_PlaySoundW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 9
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_SendDriverMessage() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 10
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_WOW32DriverCallback() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 11
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_WOW32ResolveMultiMediaHandle() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 12
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_WOWAppExit() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 13
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_aux32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 14
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_auxGetDevCapsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 15
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_auxGetDevCapsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 16
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_auxGetNumDevs() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 17
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_auxGetVolume() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 18
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_auxOutMessage() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 19
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_auxSetVolume() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 20
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joy32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 21
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joyConfigChanged() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 22
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joyGetDevCapsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 23
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joyGetDevCapsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 24
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joyGetNumDevs() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 25
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joyGetPos() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 26
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joyGetPosEx() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 27
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joyGetThreshold() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 28
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joyReleaseCapture() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 29
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joySetCapture() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 30
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_joySetThreshold() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 31
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mci32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 32
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciDriverNotify() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 33
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciDriverYield() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 34
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciExecute() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 35
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciFreeCommandResource() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 36
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetCreatorTask() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 37
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetDeviceIDA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 38
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetDeviceIDFromElementIDA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 39
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetDeviceIDFromElementIDW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 40
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetDeviceIDW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 41
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetDriverData() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 42
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetErrorStringA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 43
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetErrorStringW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 44
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciGetYieldProc() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 45
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciLoadCommandResource() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 46
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciSendCommandA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 47
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciSendCommandW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 48
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciSendStringA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 49
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciSendStringW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 50
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciSetDriverData() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 51
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mciSetYieldProc() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 52
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mid32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 53
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiConnect() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 54
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiDisconnect() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 55
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInAddBuffer() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 56
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInClose() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 57
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInGetDevCapsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 58
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInGetDevCapsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 59
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInGetErrorTextA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 60
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInGetErrorTextW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 61
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInGetID() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 62
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInGetNumDevs() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 63
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInMessage() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 64
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInOpen() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 65
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInPrepareHeader() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 66
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInReset() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 67
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInStart() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 68
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInStop() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 69
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiInUnprepareHeader() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 70
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutCacheDrumPatches() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 71
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutCachePatches() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 72
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutClose() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 73
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutGetDevCapsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 74
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutGetDevCapsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 75
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutGetErrorTextA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 76
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutGetErrorTextW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 77
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutGetID() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 78
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutGetNumDevs() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 79
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutGetVolume() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 80
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutLongMsg() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 81
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutMessage() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 82
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutOpen() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 83
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutPrepareHeader() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 84
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutReset() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 85
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutSetVolume() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 86
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutShortMsg() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 87
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiOutUnprepareHeader() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 88
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiStreamClose() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 89
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiStreamOpen() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 90
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiStreamOut() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 91
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiStreamPause() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 92
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiStreamPosition() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 93
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiStreamProperty() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 94
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiStreamRestart() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 95
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_midiStreamStop() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 96
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerClose() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 97
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetControlDetailsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 98
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetControlDetailsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 99
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetDevCapsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 100
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetDevCapsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 101
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetID() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 102
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetLineControlsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 103
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetLineControlsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 104
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetLineInfoA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 105
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetLineInfoW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 106
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerGetNumDevs() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 107
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerMessage() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 108
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerOpen() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 109
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mixerSetControlDetails() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 110
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmDrvInstall() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 111
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmGetCurrentTask() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 112
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmTaskBlock() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 113
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmTaskCreate() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 114
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmTaskSignal() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 115
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmTaskYield() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 116
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioAdvance() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 117
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioAscend() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 118
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioClose() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 119
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioCreateChunk() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 120
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioDescend() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 121
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioFlush() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 122
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioGetInfo() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 123
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioInstallIOProcA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 124
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioInstallIOProcW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 125
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioOpenA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 126
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioOpenW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 127
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioRead() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 128
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioRenameA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 129
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioRenameW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 130
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioSeek() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 131
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioSendMessage() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 132
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioSetBuffer() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 133
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioSetInfo() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 134
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioStringToFOURCCA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 135
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioStringToFOURCCW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 136
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmioWrite() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 137
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mmsystemGetVersion() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 138
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mod32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 139
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_mxd32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 140
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_sndPlaySoundA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 141
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_sndPlaySoundW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 142
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_tid32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 143
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_timeBeginPeriod() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 144
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_timeEndPeriod() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 145
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_timeGetDevCaps() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 146
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_timeGetSystemTime() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 147
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_timeGetTime() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 148
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_timeKillEvent() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 149
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_timeSetEvent() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 150
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInAddBuffer() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 151
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInClose() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 152
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInGetDevCapsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 153
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInGetDevCapsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 154
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInGetErrorTextA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 155
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInGetErrorTextW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 156
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInGetID() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 157
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInGetNumDevs() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 158
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInGetPosition() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 159
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInMessage() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 160
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInOpen() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 161
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInPrepareHeader() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 162
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInReset() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 163
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInStart() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 164
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInStop() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 165
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveInUnprepareHeader() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 166
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutBreakLoop() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 167
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutClose() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 168
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetDevCapsA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 169
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetDevCapsW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 170
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetErrorTextA() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 171
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetErrorTextW() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 172
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetID() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 173
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetNumDevs() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 174
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetPitch() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 175
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetPlaybackRate() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 176
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetPosition() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 177
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutGetVolume() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 178
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutMessage() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 179
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutOpen() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 180
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutPause() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 181
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutPrepareHeader() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 182
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutReset() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 183
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutRestart() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 184
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutSetPitch() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 185
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutSetPlaybackRate() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 186
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutSetVolume() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 187
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutUnprepareHeader() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 188
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_waveOutWrite() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 189
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_wid32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 190
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
__declspec(naked) void winmm_proxy_wod32Message() {
    __asm {
        mov eax, winmm_JumpTable
        mov ecx, 191
        mov eax, [eax + ecx*4]
        jmp eax
    }
}
}
#pragma comment(linker,"/export:CloseDriver=_winmm_proxy_CloseDriver")
#pragma comment(linker,"/export:DefDriverProc=_winmm_proxy_DefDriverProc")
#pragma comment(linker,"/export:DriverCallback=_winmm_proxy_DriverCallback")
#pragma comment(linker,"/export:DrvGetModuleHandle=_winmm_proxy_DrvGetModuleHandle")
#pragma comment(linker,"/export:GetDriverModuleHandle=_winmm_proxy_GetDriverModuleHandle")
#pragma comment(linker,"/export:NotifyCallbackData=_winmm_proxy_NotifyCallbackData")
#pragma comment(linker,"/export:OpenDriver=_winmm_proxy_OpenDriver")
#pragma comment(linker,"/export:PlaySound=_winmm_proxy_PlaySound")
#pragma comment(linker,"/export:PlaySoundA=_winmm_proxy_PlaySoundA")
#pragma comment(linker,"/export:PlaySoundW=_winmm_proxy_PlaySoundW")
#pragma comment(linker,"/export:SendDriverMessage=_winmm_proxy_SendDriverMessage")
#pragma comment(linker,"/export:WOW32DriverCallback=_winmm_proxy_WOW32DriverCallback")
#pragma comment(linker,"/export:WOW32ResolveMultiMediaHandle=_winmm_proxy_WOW32ResolveMultiMediaHandle")
#pragma comment(linker,"/export:WOWAppExit=_winmm_proxy_WOWAppExit")
#pragma comment(linker,"/export:aux32Message=_winmm_proxy_aux32Message")
#pragma comment(linker,"/export:auxGetDevCapsA=_winmm_proxy_auxGetDevCapsA")
#pragma comment(linker,"/export:auxGetDevCapsW=_winmm_proxy_auxGetDevCapsW")
#pragma comment(linker,"/export:auxGetNumDevs=_winmm_proxy_auxGetNumDevs")
#pragma comment(linker,"/export:auxGetVolume=_winmm_proxy_auxGetVolume")
#pragma comment(linker,"/export:auxOutMessage=_winmm_proxy_auxOutMessage")
#pragma comment(linker,"/export:auxSetVolume=_winmm_proxy_auxSetVolume")
#pragma comment(linker,"/export:joy32Message=_winmm_proxy_joy32Message")
#pragma comment(linker,"/export:joyConfigChanged=_winmm_proxy_joyConfigChanged")
#pragma comment(linker,"/export:joyGetDevCapsA=_winmm_proxy_joyGetDevCapsA")
#pragma comment(linker,"/export:joyGetDevCapsW=_winmm_proxy_joyGetDevCapsW")
#pragma comment(linker,"/export:joyGetNumDevs=_winmm_proxy_joyGetNumDevs")
#pragma comment(linker,"/export:joyGetPos=_winmm_proxy_joyGetPos")
#pragma comment(linker,"/export:joyGetPosEx=_winmm_proxy_joyGetPosEx")
#pragma comment(linker,"/export:joyGetThreshold=_winmm_proxy_joyGetThreshold")
#pragma comment(linker,"/export:joyReleaseCapture=_winmm_proxy_joyReleaseCapture")
#pragma comment(linker,"/export:joySetCapture=_winmm_proxy_joySetCapture")
#pragma comment(linker,"/export:joySetThreshold=_winmm_proxy_joySetThreshold")
#pragma comment(linker,"/export:mci32Message=_winmm_proxy_mci32Message")
#pragma comment(linker,"/export:mciDriverNotify=_winmm_proxy_mciDriverNotify")
#pragma comment(linker,"/export:mciDriverYield=_winmm_proxy_mciDriverYield")
#pragma comment(linker,"/export:mciExecute=_winmm_proxy_mciExecute")
#pragma comment(linker,"/export:mciFreeCommandResource=_winmm_proxy_mciFreeCommandResource")
#pragma comment(linker,"/export:mciGetCreatorTask=_winmm_proxy_mciGetCreatorTask")
#pragma comment(linker,"/export:mciGetDeviceIDA=_winmm_proxy_mciGetDeviceIDA")
#pragma comment(linker,"/export:mciGetDeviceIDFromElementIDA=_winmm_proxy_mciGetDeviceIDFromElementIDA")
#pragma comment(linker,"/export:mciGetDeviceIDFromElementIDW=_winmm_proxy_mciGetDeviceIDFromElementIDW")
#pragma comment(linker,"/export:mciGetDeviceIDW=_winmm_proxy_mciGetDeviceIDW")
#pragma comment(linker,"/export:mciGetDriverData=_winmm_proxy_mciGetDriverData")
#pragma comment(linker,"/export:mciGetErrorStringA=_winmm_proxy_mciGetErrorStringA")
#pragma comment(linker,"/export:mciGetErrorStringW=_winmm_proxy_mciGetErrorStringW")
#pragma comment(linker,"/export:mciGetYieldProc=_winmm_proxy_mciGetYieldProc")
#pragma comment(linker,"/export:mciLoadCommandResource=_winmm_proxy_mciLoadCommandResource")
#pragma comment(linker,"/export:mciSendCommandA=_winmm_proxy_mciSendCommandA")
#pragma comment(linker,"/export:mciSendCommandW=_winmm_proxy_mciSendCommandW")
#pragma comment(linker,"/export:mciSendStringA=_winmm_proxy_mciSendStringA")
#pragma comment(linker,"/export:mciSendStringW=_winmm_proxy_mciSendStringW")
#pragma comment(linker,"/export:mciSetDriverData=_winmm_proxy_mciSetDriverData")
#pragma comment(linker,"/export:mciSetYieldProc=_winmm_proxy_mciSetYieldProc")
#pragma comment(linker,"/export:mid32Message=_winmm_proxy_mid32Message")
#pragma comment(linker,"/export:midiConnect=_winmm_proxy_midiConnect")
#pragma comment(linker,"/export:midiDisconnect=_winmm_proxy_midiDisconnect")
#pragma comment(linker,"/export:midiInAddBuffer=_winmm_proxy_midiInAddBuffer")
#pragma comment(linker,"/export:midiInClose=_winmm_proxy_midiInClose")
#pragma comment(linker,"/export:midiInGetDevCapsA=_winmm_proxy_midiInGetDevCapsA")
#pragma comment(linker,"/export:midiInGetDevCapsW=_winmm_proxy_midiInGetDevCapsW")
#pragma comment(linker,"/export:midiInGetErrorTextA=_winmm_proxy_midiInGetErrorTextA")
#pragma comment(linker,"/export:midiInGetErrorTextW=_winmm_proxy_midiInGetErrorTextW")
#pragma comment(linker,"/export:midiInGetID=_winmm_proxy_midiInGetID")
#pragma comment(linker,"/export:midiInGetNumDevs=_winmm_proxy_midiInGetNumDevs")
#pragma comment(linker,"/export:midiInMessage=_winmm_proxy_midiInMessage")
#pragma comment(linker,"/export:midiInOpen=_winmm_proxy_midiInOpen")
#pragma comment(linker,"/export:midiInPrepareHeader=_winmm_proxy_midiInPrepareHeader")
#pragma comment(linker,"/export:midiInReset=_winmm_proxy_midiInReset")
#pragma comment(linker,"/export:midiInStart=_winmm_proxy_midiInStart")
#pragma comment(linker,"/export:midiInStop=_winmm_proxy_midiInStop")
#pragma comment(linker,"/export:midiInUnprepareHeader=_winmm_proxy_midiInUnprepareHeader")
#pragma comment(linker,"/export:midiOutCacheDrumPatches=_winmm_proxy_midiOutCacheDrumPatches")
#pragma comment(linker,"/export:midiOutCachePatches=_winmm_proxy_midiOutCachePatches")
#pragma comment(linker,"/export:midiOutClose=_winmm_proxy_midiOutClose")
#pragma comment(linker,"/export:midiOutGetDevCapsA=_winmm_proxy_midiOutGetDevCapsA")
#pragma comment(linker,"/export:midiOutGetDevCapsW=_winmm_proxy_midiOutGetDevCapsW")
#pragma comment(linker,"/export:midiOutGetErrorTextA=_winmm_proxy_midiOutGetErrorTextA")
#pragma comment(linker,"/export:midiOutGetErrorTextW=_winmm_proxy_midiOutGetErrorTextW")
#pragma comment(linker,"/export:midiOutGetID=_winmm_proxy_midiOutGetID")
#pragma comment(linker,"/export:midiOutGetNumDevs=_winmm_proxy_midiOutGetNumDevs")
#pragma comment(linker,"/export:midiOutGetVolume=_winmm_proxy_midiOutGetVolume")
#pragma comment(linker,"/export:midiOutLongMsg=_winmm_proxy_midiOutLongMsg")
#pragma comment(linker,"/export:midiOutMessage=_winmm_proxy_midiOutMessage")
#pragma comment(linker,"/export:midiOutOpen=_winmm_proxy_midiOutOpen")
#pragma comment(linker,"/export:midiOutPrepareHeader=_winmm_proxy_midiOutPrepareHeader")
#pragma comment(linker,"/export:midiOutReset=_winmm_proxy_midiOutReset")
#pragma comment(linker,"/export:midiOutSetVolume=_winmm_proxy_midiOutSetVolume")
#pragma comment(linker,"/export:midiOutShortMsg=_winmm_proxy_midiOutShortMsg")
#pragma comment(linker,"/export:midiOutUnprepareHeader=_winmm_proxy_midiOutUnprepareHeader")
#pragma comment(linker,"/export:midiStreamClose=_winmm_proxy_midiStreamClose")
#pragma comment(linker,"/export:midiStreamOpen=_winmm_proxy_midiStreamOpen")
#pragma comment(linker,"/export:midiStreamOut=_winmm_proxy_midiStreamOut")
#pragma comment(linker,"/export:midiStreamPause=_winmm_proxy_midiStreamPause")
#pragma comment(linker,"/export:midiStreamPosition=_winmm_proxy_midiStreamPosition")
#pragma comment(linker,"/export:midiStreamProperty=_winmm_proxy_midiStreamProperty")
#pragma comment(linker,"/export:midiStreamRestart=_winmm_proxy_midiStreamRestart")
#pragma comment(linker,"/export:midiStreamStop=_winmm_proxy_midiStreamStop")
#pragma comment(linker,"/export:mixerClose=_winmm_proxy_mixerClose")
#pragma comment(linker,"/export:mixerGetControlDetailsA=_winmm_proxy_mixerGetControlDetailsA")
#pragma comment(linker,"/export:mixerGetControlDetailsW=_winmm_proxy_mixerGetControlDetailsW")
#pragma comment(linker,"/export:mixerGetDevCapsA=_winmm_proxy_mixerGetDevCapsA")
#pragma comment(linker,"/export:mixerGetDevCapsW=_winmm_proxy_mixerGetDevCapsW")
#pragma comment(linker,"/export:mixerGetID=_winmm_proxy_mixerGetID")
#pragma comment(linker,"/export:mixerGetLineControlsA=_winmm_proxy_mixerGetLineControlsA")
#pragma comment(linker,"/export:mixerGetLineControlsW=_winmm_proxy_mixerGetLineControlsW")
#pragma comment(linker,"/export:mixerGetLineInfoA=_winmm_proxy_mixerGetLineInfoA")
#pragma comment(linker,"/export:mixerGetLineInfoW=_winmm_proxy_mixerGetLineInfoW")
#pragma comment(linker,"/export:mixerGetNumDevs=_winmm_proxy_mixerGetNumDevs")
#pragma comment(linker,"/export:mixerMessage=_winmm_proxy_mixerMessage")
#pragma comment(linker,"/export:mixerOpen=_winmm_proxy_mixerOpen")
#pragma comment(linker,"/export:mixerSetControlDetails=_winmm_proxy_mixerSetControlDetails")
#pragma comment(linker,"/export:mmDrvInstall=_winmm_proxy_mmDrvInstall")
#pragma comment(linker,"/export:mmGetCurrentTask=_winmm_proxy_mmGetCurrentTask")
#pragma comment(linker,"/export:mmTaskBlock=_winmm_proxy_mmTaskBlock")
#pragma comment(linker,"/export:mmTaskCreate=_winmm_proxy_mmTaskCreate")
#pragma comment(linker,"/export:mmTaskSignal=_winmm_proxy_mmTaskSignal")
#pragma comment(linker,"/export:mmTaskYield=_winmm_proxy_mmTaskYield")
#pragma comment(linker,"/export:mmioAdvance=_winmm_proxy_mmioAdvance")
#pragma comment(linker,"/export:mmioAscend=_winmm_proxy_mmioAscend")
#pragma comment(linker,"/export:mmioClose=_winmm_proxy_mmioClose")
#pragma comment(linker,"/export:mmioCreateChunk=_winmm_proxy_mmioCreateChunk")
#pragma comment(linker,"/export:mmioDescend=_winmm_proxy_mmioDescend")
#pragma comment(linker,"/export:mmioFlush=_winmm_proxy_mmioFlush")
#pragma comment(linker,"/export:mmioGetInfo=_winmm_proxy_mmioGetInfo")
#pragma comment(linker,"/export:mmioInstallIOProcA=_winmm_proxy_mmioInstallIOProcA")
#pragma comment(linker,"/export:mmioInstallIOProcW=_winmm_proxy_mmioInstallIOProcW")
#pragma comment(linker,"/export:mmioOpenA=_winmm_proxy_mmioOpenA")
#pragma comment(linker,"/export:mmioOpenW=_winmm_proxy_mmioOpenW")
#pragma comment(linker,"/export:mmioRead=_winmm_proxy_mmioRead")
#pragma comment(linker,"/export:mmioRenameA=_winmm_proxy_mmioRenameA")
#pragma comment(linker,"/export:mmioRenameW=_winmm_proxy_mmioRenameW")
#pragma comment(linker,"/export:mmioSeek=_winmm_proxy_mmioSeek")
#pragma comment(linker,"/export:mmioSendMessage=_winmm_proxy_mmioSendMessage")
#pragma comment(linker,"/export:mmioSetBuffer=_winmm_proxy_mmioSetBuffer")
#pragma comment(linker,"/export:mmioSetInfo=_winmm_proxy_mmioSetInfo")
#pragma comment(linker,"/export:mmioStringToFOURCCA=_winmm_proxy_mmioStringToFOURCCA")
#pragma comment(linker,"/export:mmioStringToFOURCCW=_winmm_proxy_mmioStringToFOURCCW")
#pragma comment(linker,"/export:mmioWrite=_winmm_proxy_mmioWrite")
#pragma comment(linker,"/export:mmsystemGetVersion=_winmm_proxy_mmsystemGetVersion")
#pragma comment(linker,"/export:mod32Message=_winmm_proxy_mod32Message")
#pragma comment(linker,"/export:mxd32Message=_winmm_proxy_mxd32Message")
#pragma comment(linker,"/export:sndPlaySoundA=_winmm_proxy_sndPlaySoundA")
#pragma comment(linker,"/export:sndPlaySoundW=_winmm_proxy_sndPlaySoundW")
#pragma comment(linker,"/export:tid32Message=_winmm_proxy_tid32Message")
#pragma comment(linker,"/export:timeBeginPeriod=_winmm_proxy_timeBeginPeriod")
#pragma comment(linker,"/export:timeEndPeriod=_winmm_proxy_timeEndPeriod")
#pragma comment(linker,"/export:timeGetDevCaps=_winmm_proxy_timeGetDevCaps")
#pragma comment(linker,"/export:timeGetSystemTime=_winmm_proxy_timeGetSystemTime")
#pragma comment(linker,"/export:timeGetTime=_winmm_proxy_timeGetTime")
#pragma comment(linker,"/export:timeKillEvent=_winmm_proxy_timeKillEvent")
#pragma comment(linker,"/export:timeSetEvent=_winmm_proxy_timeSetEvent")
#pragma comment(linker,"/export:waveInAddBuffer=_winmm_proxy_waveInAddBuffer")
#pragma comment(linker,"/export:waveInClose=_winmm_proxy_waveInClose")
#pragma comment(linker,"/export:waveInGetDevCapsA=_winmm_proxy_waveInGetDevCapsA")
#pragma comment(linker,"/export:waveInGetDevCapsW=_winmm_proxy_waveInGetDevCapsW")
#pragma comment(linker,"/export:waveInGetErrorTextA=_winmm_proxy_waveInGetErrorTextA")
#pragma comment(linker,"/export:waveInGetErrorTextW=_winmm_proxy_waveInGetErrorTextW")
#pragma comment(linker,"/export:waveInGetID=_winmm_proxy_waveInGetID")
#pragma comment(linker,"/export:waveInGetNumDevs=_winmm_proxy_waveInGetNumDevs")
#pragma comment(linker,"/export:waveInGetPosition=_winmm_proxy_waveInGetPosition")
#pragma comment(linker,"/export:waveInMessage=_winmm_proxy_waveInMessage")
#pragma comment(linker,"/export:waveInOpen=_winmm_proxy_waveInOpen")
#pragma comment(linker,"/export:waveInPrepareHeader=_winmm_proxy_waveInPrepareHeader")
#pragma comment(linker,"/export:waveInReset=_winmm_proxy_waveInReset")
#pragma comment(linker,"/export:waveInStart=_winmm_proxy_waveInStart")
#pragma comment(linker,"/export:waveInStop=_winmm_proxy_waveInStop")
#pragma comment(linker,"/export:waveInUnprepareHeader=_winmm_proxy_waveInUnprepareHeader")
#pragma comment(linker,"/export:waveOutBreakLoop=_winmm_proxy_waveOutBreakLoop")
#pragma comment(linker,"/export:waveOutClose=_winmm_proxy_waveOutClose")
#pragma comment(linker,"/export:waveOutGetDevCapsA=_winmm_proxy_waveOutGetDevCapsA")
#pragma comment(linker,"/export:waveOutGetDevCapsW=_winmm_proxy_waveOutGetDevCapsW")
#pragma comment(linker,"/export:waveOutGetErrorTextA=_winmm_proxy_waveOutGetErrorTextA")
#pragma comment(linker,"/export:waveOutGetErrorTextW=_winmm_proxy_waveOutGetErrorTextW")
#pragma comment(linker,"/export:waveOutGetID=_winmm_proxy_waveOutGetID")
#pragma comment(linker,"/export:waveOutGetNumDevs=_winmm_proxy_waveOutGetNumDevs")
#pragma comment(linker,"/export:waveOutGetPitch=_winmm_proxy_waveOutGetPitch")
#pragma comment(linker,"/export:waveOutGetPlaybackRate=_winmm_proxy_waveOutGetPlaybackRate")
#pragma comment(linker,"/export:waveOutGetPosition=_winmm_proxy_waveOutGetPosition")
#pragma comment(linker,"/export:waveOutGetVolume=_winmm_proxy_waveOutGetVolume")
#pragma comment(linker,"/export:waveOutMessage=_winmm_proxy_waveOutMessage")
#pragma comment(linker,"/export:waveOutOpen=_winmm_proxy_waveOutOpen")
#pragma comment(linker,"/export:waveOutPause=_winmm_proxy_waveOutPause")
#pragma comment(linker,"/export:waveOutPrepareHeader=_winmm_proxy_waveOutPrepareHeader")
#pragma comment(linker,"/export:waveOutReset=_winmm_proxy_waveOutReset")
#pragma comment(linker,"/export:waveOutRestart=_winmm_proxy_waveOutRestart")
#pragma comment(linker,"/export:waveOutSetPitch=_winmm_proxy_waveOutSetPitch")
#pragma comment(linker,"/export:waveOutSetPlaybackRate=_winmm_proxy_waveOutSetPlaybackRate")
#pragma comment(linker,"/export:waveOutSetVolume=_winmm_proxy_waveOutSetVolume")
#pragma comment(linker,"/export:waveOutUnprepareHeader=_winmm_proxy_waveOutUnprepareHeader")
#pragma comment(linker,"/export:waveOutWrite=_winmm_proxy_waveOutWrite")
#pragma comment(linker,"/export:wid32Message=_winmm_proxy_wid32Message")
#pragma comment(linker,"/export:wod32Message=_winmm_proxy_wod32Message")

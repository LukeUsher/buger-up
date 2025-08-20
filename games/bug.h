struct Bug : Game
{
	using DirectDrawCreate_t = HRESULT(__stdcall*)(GUID*, LPDIRECTDRAW*, IUnknown*);
	inline static DirectDrawCreate_t _DirectDrawCreate = nullptr;

	inline static std::string _GameDirectory = "";

	auto applyPatches(std::string hash) -> bool override {
		EnsureDwmMitigationAndRelaunch();

		//Remove hard-coded resolution limit of <1024 pixels wide @ 8bpp
		patchEngine.PatchFunction("IsDisplayModeCompatible", 0x00413530, IsDisplayModeCompatible_Hook);
#if _DEBUG
		//Enable the games stubbed out DebugPrint fuctionalty
		patchEngine.PatchFunction("DebugPrint", 0x00407F30, DebugPrint_Hook);
#endif

		//Hook DirectDrawCreate to enable 8bpp 
		patchEngine.PatchImportedFunction("ddraw.dll", "DirectDrawCreate", DirectDrawCreate_Hook, (void**)&_DirectDrawCreate);

		//Skip DirectIO wait for vblank (the game uses Sleep for timing anyway)
		patchEngine.InjectJump(0x0041158B, 0x004115B9);

		//Disable forced 320x240 fullscreen for FMV playback
		patchEngine.PatchFunction("FMV_SetFullscreen", 0x00411670, FMV_SetFullscreenHook);

		//Implement a proper FMV video blit that actually respects stride
		patchEngine.PatchBinary(0x00411561,    {0xFF, 0x73, 0x14}); // push [ebx+14h]
		patchEngine.InjectCall(0x00411561 + 3, FMV_CopyBuffer);
		patchEngine.PatchBinary(0x00411569, { 0x90, 0x90, 0x90, 0x90, 0x90 }); //nop padding

		//Hook legacy joypad apis
		patchEngine.PatchImportedFunction("winmm.dll", "joyGetDevCapsA", joyGetDevCapsA_Hook);
		patchEngine.PatchImportedFunction("winmm.dll", "joyGetPosEx", joyGetPosEx_Hook);

		//NoCD:Hook IsCorrectDiscInserted to always return true
		patchEngine.PatchFunction("IsCorrectDiscInserted", 0x00411300, IsCorrectDiscInserted_Hook);

		//NoCD:Hook GetDiscPath to always return true
		patchEngine.PatchFunction("GetDiscPath", 0x00411390, GetDiscPath_Hook);
		
		//NoCD:patch loadFmv to use a string we control for cdromPath
		_GameDirectory = GetCurrentGameInstallDirectory();
		patchEngine.PatchBinary<const char*>(0x004116CC, _GameDirectory.c_str());

		//NoCD:Hook CD-DA routines
		patchEngine.PatchFunction("CdOpen", 0x00401000, CdOpen_Hook);
		patchEngine.PatchFunction("CdPlay", 0x00401140, CdPlay_Hook);
		patchEngine.PatchFunction("CdStop", 0x00401240, CdStop_Hook);
		patchEngine.PatchFunction("CdClose", 0x00401280, CdClose_Hook);
		patchEngine.PatchFunction("CdPause", 0x00401440, CdPause_Hook);
		patchEngine.PatchFunction("CdResume", 0x00401490, CdResume_Hook);
		return true;
	}

	static auto __cdecl IsDisplayModeCompatible_Hook(HWND hWnd) -> int {
		return 1;
	}

	static auto __cdecl DebugPrint_Hook(const char* fmt, ...) -> void {
		char buffer[1024];

		va_list args;
		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		va_end(args);
		printf("[Game Debug] %s\n", buffer);
		return;
	}

	static auto __stdcall DirectDrawCreate_Hook(GUID* lpGUID, LPDIRECTDRAW* lplpDD, IUnknown* pUnkOuter) -> HRESULT {
		auto hResult = _DirectDrawCreate(lpGUID, lplpDD, pUnkOuter);
		if (*lplpDD == nullptr) {
			MessageBoxA(NULL, "Failed to get DirectDraw vtbl", "Errot", MB_ICONERROR);
			ExitProcess(0);
		}

		// Do not allow the game to switch display resolution
		// If we skip this call completely, the game doesn't get an 8bpp buffer and renders incorrectly
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		(*lplpDD)->SetDisplayMode(screenWidth, screenHeight, 8);

		return hResult;
	}

	static auto __cdecl FMV_SetFullscreenHook() -> HRESULT {
		return 0;
	}

	static auto __stdcall FMV_CopyBuffer(uint8_t* srcBuffer) -> void
	{
		DDSURFACEDESC* fmvSurfaceDesc = (DDSURFACEDESC*)0x5244E0;

		if (!srcBuffer) return;
		if (!fmvSurfaceDesc->lpSurface || fmvSurfaceDesc->lPitch <= 0) return;

		uint8_t* dst = (uint8_t*)fmvSurfaceDesc->lpSurface;
		const uint8_t* s = (const uint8_t*)srcBuffer;

		const int surfW = fmvSurfaceDesc->dwWidth;
		const int surfH = fmvSurfaceDesc->dwHeight;
		const int pitch = fmvSurfaceDesc->lPitch;

		const int fmvW = 320;
		const int fmvH = 240;

		//Prevent rendering off-centre
		const int srcX = 8;
		const int srcY = 8;
		const int realW = 304;
		const int realH = 232;

		const int copyW = min(surfW, realW);
		const int copyH = min(surfH, realH);

		const uint8_t* srcStart = s + srcY * fmvW + srcX;

		for (int y = 0; y < copyH; ++y) {
			memcpy(dst + y * pitch, srcStart + y * fmvW, copyW);
		}
	}

	static inline DWORD ThumbToJoyRange(LONG val) {
		return (DWORD)((int)val + 32768);
	}

	static inline DWORD MapButtons(WORD xb) {
		DWORD out = 0;
		if (xb & XINPUT_GAMEPAD_A) out |= (1u << 0);
		if (xb & XINPUT_GAMEPAD_B) out |= (1u << 1);
		if (xb & XINPUT_GAMEPAD_X) out |= (1u << 2);
		if (xb & XINPUT_GAMEPAD_Y) out |= (1u << 3);
		if (xb & XINPUT_GAMEPAD_LEFT_SHOULDER)  out |= (1u << 4);
		if (xb & XINPUT_GAMEPAD_RIGHT_SHOULDER) out |= (1u << 5);
		if (xb & XINPUT_GAMEPAD_START) out |= (1u << 6);
		if (xb & XINPUT_GAMEPAD_BACK)  out |= (1u << 7);
		if (xb & XINPUT_GAMEPAD_DPAD_UP)    out |= (1u << 8);
		if (xb & XINPUT_GAMEPAD_DPAD_DOWN)  out |= (1u << 9);
		if (xb & XINPUT_GAMEPAD_DPAD_LEFT)  out |= (1u << 10);
		if (xb & XINPUT_GAMEPAD_DPAD_RIGHT) out |= (1u << 11);
		if (xb & XINPUT_GAMEPAD_LEFT_THUMB)  out |= (1u << 12);
		if (xb & XINPUT_GAMEPAD_RIGHT_THUMB) out |= (1u << 13);
		return out;
	}

	static auto __stdcall joyGetDevCapsA_Hook(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc) -> MMRESULT {
		if (!pjc || cbjc < sizeof(JOYCAPSA)) return MMSYSERR_INVALPARAM;

		if (uJoyID >= 4) return MMSYSERR_NODRIVER;

		ZeroMemory(pjc, cbjc);
		pjc->wMid = 0;
		pjc->wPid = 0;
		strncpy_s(pjc->szPname, "XInput Controller", MAXPNAMELEN);
		pjc->wXmin = 0;    pjc->wXmax = 0xFFFF;
		pjc->wYmin = 0;    pjc->wYmax = 0xFFFF;
		pjc->wZmin = 0;    pjc->wZmax = 0xFFFF;
		pjc->wNumButtons = 14;
		pjc->wPeriodMin = 0;
		pjc->wPeriodMax = 0;

		return JOYERR_NOERROR;
	}

	static auto __stdcall joyGetPosEx_Hook(UINT uJoyID, LPJOYINFOEX pji) -> MMRESULT {
		if (!pji || pji->dwSize < sizeof(JOYINFOEX)) return MMSYSERR_INVALPARAM;

		if (uJoyID >= 4) return MMSYSERR_NODRIVER;

		XINPUT_STATE xi{};
		DWORD res = XInputGetState(uJoyID, &xi);

		pji->dwSize = sizeof(JOYINFOEX);

		if (res == ERROR_SUCCESS) {
			LONG lx = xi.Gamepad.sThumbLX;
			LONG ly = xi.Gamepad.sThumbLY;

			if (xi.Gamepad.wButtons & (XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT | XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN)) {
				lx = 0;
				ly = 0;
				if (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)  lx = -32768;
				if (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) lx = 32767;
				if (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)    ly = 32767;
				if (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  ly = -32768;
			}

			if (pji->dwFlags & JOY_RETURNX) pji->dwXpos = ThumbToJoyRange(lx);
			if (pji->dwFlags & JOY_RETURNY)	pji->dwYpos = ThumbToJoyRange(-ly);
			if (pji->dwFlags & JOY_RETURNZ)	pji->dwZpos = xi.Gamepad.bRightTrigger * 257;
			if (pji->dwFlags & JOY_RETURNR)	pji->dwRpos = xi.Gamepad.bLeftTrigger * 257;
			if (pji->dwFlags & JOY_RETURNBUTTONS) {
				pji->dwButtons = MapButtons(xi.Gamepad.wButtons);
				pji->dwButtonNumber = 0;
				for (int i = 0; i < 32; ++i) {
					if (pji->dwButtons & (1u << i)) {
						pji->dwButtonNumber = i + 1;
						break;
					}
				}
			}

			if (pji->dwFlags & JOY_RETURNPOV) {
				WORD b = xi.Gamepad.wButtons;
				if (b & XINPUT_GAMEPAD_DPAD_UP) {
					if (b & XINPUT_GAMEPAD_DPAD_RIGHT) pji->dwPOV = 4500;
					else if (b & XINPUT_GAMEPAD_DPAD_LEFT) pji->dwPOV = 31500;
					else pji->dwPOV = 0;
				}
				else if (b & XINPUT_GAMEPAD_DPAD_RIGHT) pji->dwPOV = 9000;
				else if (b & XINPUT_GAMEPAD_DPAD_DOWN) {
					if (b & XINPUT_GAMEPAD_DPAD_RIGHT) pji->dwPOV = 13500;
					else if (b & XINPUT_GAMEPAD_DPAD_LEFT) pji->dwPOV = 22500;
					else pji->dwPOV = 18000;
				}
				else if (b & XINPUT_GAMEPAD_DPAD_LEFT) pji->dwPOV = 27000;
				else pji->dwPOV = JOY_POVCENTERED;
			}
		} else {
			pji->dwXpos = 0x8000;
			pji->dwYpos = 0x8000;
			pji->dwZpos = 0;
			pji->dwRpos = 0;
			pji->dwButtons = 0;
			pji->dwButtonNumber = 0;
			pji->dwPOV = JOY_POVCENTERED;
		}

		return JOYERR_NOERROR;
	}

	static auto __cdecl IsCorrectDiscInserted_Hook() -> int {
		return 1;
	}

	static auto __cdecl GetDiscPath_Hook() -> BOOL {
		return 1;
	}

	static auto CdOpen_Hook(HINSTANCE hInstance) -> BOOL {
		if (!cdPlayer.open(97'505)) return FALSE;
		return TRUE;
	}

	static auto CdClose_Hook() -> BOOL {
		cdPlayer.stop();
		cdPlayer.close();
		return TRUE;
	}

	static auto CdPlay_Hook(int trackNumber, int loop) -> BOOL {
		if (trackNumber < 1 || trackNumber >= cdPlayer.trackCount()) return FALSE;
		cdPlayer.playTrack(trackNumber, loop != 0);
		return TRUE;
	}

	static auto CdStop_Hook() -> BOOL {
		cdPlayer.stop();
		return TRUE;
	}

	static auto CdPause_Hook() -> BOOL {
		cdPlayer.pause();
		return TRUE;
	}

	static auto CdResume_Hook() -> BOOL {
		cdPlayer.resume();
		return TRUE;
	}
};
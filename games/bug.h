struct Bug : Game
{
	using DirectDrawCreate_t = HRESULT(__stdcall*)(GUID*, LPDIRECTDRAW*, IUnknown*);
	inline static DirectDrawCreate_t _DirectDrawCreate = nullptr;

	inline static std::string _GameDirectory = "";

	auto applyPatches(std::string hash) -> bool override {
		//Remove hard-coded resolution limit of <1024 pixels wide @ 8bpp
		patchEngine.PatchFunction("IsDisplayModeCompatible", 0x00413530, IsDisplayModeCompatible_Hook);
#if _DEBUG
		//Enable the games stubbed out DebugPrint fuctionalty
		patchEngine.PatchFunction("DebugPrint", 0x00407F30, DebugPrint_Hook);
#endif

		//Skip DirectIO wait for vblank (the game uses Sleep for timing anyway)
		patchEngine.InjectJump(0x0041158B, 0x004115B9);

		//Disable forced 320x240 fullscreen for FMV playback
		patchEngine.PatchFunction("FMV_SetFullscreen", 0x00411670, FMV_SetFullscreen_Hook);
		patchEngine.PatchFunction("FMV_RestoreWindowedMode", 0x00411790, FMV_RestoreWindowedMode_Hook);
		
		//Implement a proper FMV video blit that actually respects stride
		patchEngine.PatchBinary(0x00411561,    {0xFF, 0x73, 0x14}); // push [ebx+14h]
		patchEngine.InjectCall(0x00411561 + 3, FMV_CopyBuffer);
		patchEngine.PatchBinary(0x00411569, { 0x90, 0x90, 0x90, 0x90, 0x90 }); //nop padding

		//Skip FMV out of bounds memset
		patchEngine.PatchBinary(0x00411506, { 0x90, 0x90 });

		//Prevent the game from setting the system palette, force it to set DDRAW palette instead
		patchEngine.PatchBinary(0x00416697, { 0x90, 0x90 });

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

	static auto __cdecl FMV_SetFullscreen_Hook() -> HRESULT {
		return 0;
	}

	static auto __cdecl FMV_RestoreWindowedMode_Hook() -> void {
		return;
	}

	static auto __stdcall FMV_CopyBuffer(uint8_t* srcBuffer) -> void {
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

		const int copyW = std::min(surfW, realW);
		const int copyH = std::min(surfH, realH);

		const uint8_t* srcStart = s + srcY * fmvW + srcX;

		for (int y = 0; y < copyH; ++y) {
			memcpy(dst + y * pitch, srcStart + y * fmvW, copyW);
		}
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
struct BugToo : Game
{
	inline static LPDIRECTDRAW _lpDD = nullptr;
	inline static LPDIRECTDRAWSURFACE _backBuffer = nullptr;

	using DirectDrawCreate_t = HRESULT(__stdcall*)(GUID*, LPDIRECTDRAW*, IUnknown*);
	inline static DirectDrawCreate_t _DirectDrawCreate = nullptr;

	using DirectDraw_SetDisplayMode_t = HRESULT(__stdcall*)(LPDIRECTDRAW, DWORD, DWORD, DWORD);
	inline static DirectDraw_SetDisplayMode_t _DirectDraw_SetDisplayMode = nullptr;

	using DirectDraw_SetCooperativeLevel_t = HRESULT(__stdcall*)(LPDIRECTDRAW, HWND, DWORD);
	inline static DirectDraw_SetCooperativeLevel_t _DirectDraw_SetCooperativeLevel = nullptr;

	using DirectDraw_CreateSurface_t = HRESULT(__stdcall*)(LPDIRECTDRAW, LPDDSURFACEDESC, LPDIRECTDRAWSURFACE*, IUnknown*);
	inline static DirectDraw_CreateSurface_t _DirectDraw_CreateSurface = nullptr;

	using DirectDraw_GetAttachedSurface_t = HRESULT(__stdcall*)(IDirectDrawSurface*, LPDDSCAPS, LPDIRECTDRAWSURFACE*);
	inline static DirectDraw_GetAttachedSurface_t _DirectDraw_GetAttachedSurface;

	using DirectDrawSurface_Flip_t = HRESULT(__stdcall*)(LPDIRECTDRAWSURFACE, LPDIRECTDRAWSURFACE, DWORD);
	inline static DirectDrawSurface_Flip_t _DirectDrawSurface_Flip = nullptr;

	using CreateWindowExA_t = HWND(__stdcall*)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
	inline static CreateWindowExA_t _CreateWindowExA;

	inline static std::string _GameDirectory = "";

	inline static HWND _hwnd = nullptr;

	auto applyPatches(std::string hash) -> bool override {
		EnsureDwmMitigationAndRelaunch();
		_GameDirectory = GetCurrentGameInstallDirectory();
		SetupRegistryAndShortcuts();

		//Remove OS version checks
		patchEngine.PatchFunction("IsGameCompatible", 0x00423BE0, IsGameCompatible_Hook);
		
		//Force ValidateDisplayMode to return true
		patchEngine.PatchFunction("ValidateDiplayMode", 0x0040163A, ValidateDisplayMode_Hook);

		//Hook DirectDrawCreate to enable 8bpp 
		patchEngine.PatchImportedFunction("ddraw.dll", "DirectDrawCreate", DirectDrawCreate_Hook, (void**)&_DirectDrawCreate);

		//Hook CreateWindow to remove forced fullscreen
		patchEngine.PatchImportedFunction("user32.dll", "CreateWindowExA", (void*)CreateWindowExA_Hook, (void**)& _CreateWindowExA);

		//Hook ShowCursor to force cursor to be enabled
		patchEngine.PatchImportedFunction("user32.dll", "ShowCursor", (void*)ShowCursor_Hook);

		//Hook ShowMessageBox to prevent the game from changing window size when showing message
		patchEngine.PatchFunction("ShowMessageBox", 0x0041D9DB, ShowMessageBox_Hook);

		//NoCD:Hook CD-DA routines
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_open@4", (void*)AIL_redbook_open_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_close@4", (void*)AIL_redbook_close_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_play@12", (void*)AIL_redbook_play_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_stop@4", (void*)AIL_redbook_stop_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_pause@4", (void*)AIL_redbook_pause_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_resume@4", (void*)AIL_redbook_resume_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_status@4", (void*)AIL_redbook_status_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_position@4", (void*)AIL_redbook_position_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_track_info@16", (void*)AIL_redbook_track_info_Hook);
		patchEngine.PatchImportedFunction("mss32.dll", "_AIL_redbook_tracks@4", (void*)AIL_redbook_tracks_Hook);
		return true;
	}

	static auto __cdecl IsGameCompatible_Hook() -> int {
		return 1;
	}

	static auto __stdcall ValidateDisplayMode_Hook(LPDDSURFACEDESC, LPVOID)-> HRESULT {
		*(uint32_t*)0x00447068 = 1; //validDisplayModeFound
		return S_OK;
	}

	static auto __stdcall CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, 
		int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) -> HWND {

		dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE; 
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		auto x = (GetSystemMetrics(SM_CXSCREEN) - 320) / 2;
		auto y = (GetSystemMetrics(SM_CYSCREEN) - 240) / 2;

		RECT rc = { 0, 0, 320, 240 };
		AdjustWindowRectEx(&rc, dwStyle, FALSE, dwExStyle);
		nWidth = rc.right - rc.left;
		nHeight = rc.bottom - rc.top;

		_hwnd = _CreateWindowExA(
			dwExStyle, lpClassName, lpWindowName, dwStyle,
			x, y, nWidth, nHeight,
			hWndParent, hMenu, hInstance, lpParam
		);

		return _hwnd;
	}

	static auto __stdcall ShowCursor_Hook(BOOL bShow) -> int {
		if (bShow) return 1;
		return -1;
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
		_lpDD = *lplpDD;
		_lpDD->SetDisplayMode(screenWidth, screenHeight, 8);
		
		//Patch VTBL to hook other DirectDraw calls 
		auto vtbl = *(LPVOID**)*lplpDD;

		// Don't allow forced display mode switches
		if (!_DirectDraw_SetDisplayMode) _DirectDraw_SetDisplayMode = (DirectDraw_SetDisplayMode_t)vtbl[21];
		vtbl[21] = (LPVOID)DirectDraw_SetDisplayMode_Hook;

		// Don't allow Co-operative mode switches
		if (!_DirectDraw_SetCooperativeLevel) _DirectDraw_SetCooperativeLevel = (DirectDraw_SetCooperativeLevel_t)vtbl[20];
		vtbl[20] = DirectDraw_SetCooperativeLevel_Hook;

		// Force non-exclusive surfaces
		if (!_DirectDraw_CreateSurface) _DirectDraw_CreateSurface = (DirectDraw_CreateSurface_t)vtbl[6];
		vtbl[6] = DirectDraw_CreateSurface_Hook;

		return hResult;
	}

	static auto __stdcall DirectDraw_SetDisplayMode_Hook(LPDIRECTDRAW lpDD, DWORD width, DWORD height, DWORD bpp) -> HRESULT {
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		_DirectDraw_SetDisplayMode(lpDD, screenWidth, screenHeight, bpp);
		return S_OK;
	}

	static auto __stdcall DirectDraw_SetCooperativeLevel_Hook(LPDIRECTDRAW lpDD, HWND hWnd, DWORD flags) -> HRESULT {
		return _DirectDraw_SetCooperativeLevel(lpDD, hWnd, DDSCL_NORMAL);
	}

	static auto __stdcall DirectDraw_CreateSurface_Hook(LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown* pUnkOuter) -> HRESULT {
		// The game expects a backbuffer, but we want to create an off screen surface instead; so let's do that and then patch GetAttachedSurface to return another surface
		lpDDSurfaceDesc->dwSize = sizeof(DDSURFACEDESC);
		lpDDSurfaceDesc->dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		lpDDSurfaceDesc->dwWidth = 320;
		lpDDSurfaceDesc->dwHeight = 240;
		lpDDSurfaceDesc->ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

		auto result = _DirectDraw_CreateSurface(lpDD, lpDDSurfaceDesc, lplpDDSurface, pUnkOuter);
		if (result != S_OK) return result;
		auto vtbl = *(LPVOID**)*lplpDDSurface;

		if (!_DirectDrawSurface_Flip) _DirectDrawSurface_Flip = (DirectDrawSurface_Flip_t)vtbl[11];
		vtbl[11] = DirectDrawSurface_Flip_Hook;

		if (!_DirectDraw_GetAttachedSurface) _DirectDraw_GetAttachedSurface = (DirectDraw_GetAttachedSurface_t)vtbl[12];
		vtbl[12] = DirectDraw_GetAttachedSurface_Hook;

		_backBuffer = *lplpDDSurface;

		return result;
	}

	static auto __stdcall DirectDraw_GetAttachedSurface_Hook(IDirectDrawSurface* lpDDS, LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE* lplpDDAttachedSurface) -> HRESULT {
		*lplpDDAttachedSurface = _backBuffer;
		return DD_OK;
	}

	static auto __stdcall DirectDrawSurface_Flip_Hook(LPDIRECTDRAWSURFACE lpPrimary, LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride, DWORD dwFlags) -> HRESULT
	{
		RECT client;
		GetClientRect(_hwnd, &client);

		int clientWidth = client.right - client.left;
		int clientHeight = client.bottom - client.top;

		auto hDC = GetDC(_hwnd);

		HDC hdcSurface;
		_backBuffer->GetDC(&hdcSurface);
		SetStretchBltMode(hDC, COLORONCOLOR);
		StretchBlt(hDC,	0, 0, clientWidth, clientHeight, hdcSurface, 0, 0, 320, 240, SRCCOPY);
		_backBuffer->ReleaseDC(hdcSurface);

		ReleaseDC(_hwnd, hDC);

		RedrawWindow(_hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME);
		return DD_OK;
	}

	static auto __cdecl ShowMessageBox_Hook(LPCSTR lpCaption, LPCSTR lpText, int type) -> int {
		auto sub_41D9C1 = (void(__cdecl*)())0x41D9C1;

		UINT uType = (type != 0) ? 16 : 48;
		if (type != 0) sub_41D9C1();
		if (!lpCaption)	lpCaption = "Bug Too!";
		int result = MessageBoxA(_hwnd, lpText, lpCaption, uType);
		return result;
	}

 	// Audio hooks
	static auto __stdcall AIL_redbook_open_Hook() -> HANDLE {
		if (!cdPlayer.open(127'638)) return nullptr;
		return (HANDLE)1;
	}

	static auto __stdcall AIL_redbook_close_Hook(HANDLE handle) -> void {
		cdPlayer.close();
	}

	static auto __stdcall AIL_redbook_play_Hook(HANDLE handle, DWORD start, DWORD end) -> void {
		if (!handle) return;
		cdPlayer.playSectors(start, end);
	}

	static auto __stdcall AIL_redbook_stop_Hook(HANDLE handle) -> void {
		cdPlayer.stop();
	}

	static auto __stdcall AIL_redbook_pause_Hook(HANDLE handle) -> void {
		cdPlayer.pause();
	}

	static auto __stdcall AIL_redbook_resume_Hook(HANDLE handle) -> void {
		cdPlayer.resume();
	}

	static auto __stdcall AIL_redbook_status_Hook(HANDLE handle) -> int {
		return (bool)handle;
	}

	static auto __stdcall AIL_redbook_position_Hook(HANDLE handle) -> DWORD {
		return cdPlayer.position();
	}

	static auto __stdcall AIL_redbook_track_info_Hook(HANDLE handle, int trackNumber, DWORD* start, DWORD* end) -> void {
		cdPlayer.trackInfo(trackNumber, (uint32_t*)start, (uint32_t*)end);
	}

	static auto __stdcall AIL_redbook_tracks_Hook(HANDLE handle) -> int {
		return cdPlayer.trackCount();
	}

	// The game requires registry entries to work, since the installer doesn't run, we do them manually on first launch
	static auto SetupRegistryAndShortcuts() -> void {
		HKEY hKey;
		const char* regPath = "Software\\Sega\\Bug Too!";

		if (RegOpenKeyExA(HKEY_CURRENT_USER, regPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
			if (RegCreateKeyExA(HKEY_CURRENT_USER, regPath, 0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr) == ERROR_SUCCESS) {
				std::string exeDir = GetCurrentGameExecutablePath();
				size_t pos = exeDir.find_last_of("\\/");
				std::string mainDir = exeDir.substr(0, pos);
				std::string dataDir = mainDir + "\\DATA";
				std::string saveDir = mainDir + "\\SAVEGAME";

				RegSetValueExA(hKey, "Version", 0, REG_SZ, (const BYTE*)"Smells like Victory!", 20);
				RegSetValueExA(hKey, "Main Directory", 0, REG_SZ, (const BYTE*)mainDir.c_str(), (DWORD)mainDir.size() + 1);
				RegSetValueExA(hKey, "Data Directory", 0, REG_SZ, (const BYTE*)dataDir.c_str(), (DWORD)dataDir.size() + 1);
				RegSetValueExA(hKey, "Save Directory", 0, REG_SZ, (const BYTE*)saveDir.c_str(), (DWORD)saveDir.size() + 1);

				std::vector<BYTE> keyMap = { 0x00,0x00, 0x00, 0x00,0x00, 0x00, 0x0d, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
					0x00, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26, 0x00, 0x28, 0x00, 0x25, 0x00, 0x27, 0x00, 0x4d, 0x00, 0x50, 0x00 };
				RegSetValueExA(hKey, "Key Map 1", 0, REG_BINARY, keyMap.data(), (DWORD)keyMap.size());
				RegSetValueExA(hKey, "Key Map 2", 0, REG_BINARY, keyMap.data(), (DWORD)keyMap.size());
				std::vector<BYTE> joyMap{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x02, 0x00, 0x03,
										   0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
				RegSetValueExA(hKey, "Joystick Map 1", 0, REG_BINARY, joyMap.data(), (DWORD)joyMap.size());
				RegSetValueExA(hKey, "Joystick Map 2", 0, REG_BINARY, joyMap.data(), (DWORD)joyMap.size());

				DWORD dword = 1;
				RegSetValueExA(hKey, "SFX Enabled", 0, REG_DWORD, (const BYTE*)&dword, sizeof(dword));
				dword = 5;
				RegSetValueExA(hKey, "SFX Volume", 0, REG_DWORD, (const BYTE*)&dword, sizeof(dword));
				dword = 1;
				RegSetValueExA(hKey, "Music Enabled", 0, REG_DWORD, (const BYTE*)&dword, sizeof(dword));
				dword = 5;
				RegSetValueExA(hKey, "Music Volume", 0, REG_DWORD, (const BYTE*)&dword, sizeof(dword));

				RegCloseKey(hKey);

				std::cout << "Registry setup complete.\n";
			}
		} else {
			RegCloseKey(hKey);
		}
	}
};
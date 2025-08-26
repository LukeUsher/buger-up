struct BugToo : Game
{
	inline static std::string _GameDirectory = "";

	inline static HWND _hwnd = nullptr;

	auto applyPatches(std::string hash) -> bool override {
		_GameDirectory = GetCurrentGameInstallDirectory();
		SetupRegistryAndShortcuts();

		//Remove OS version checks
		patchEngine.PatchFunction("IsGameCompatible", 0x00423BE0, IsGameCompatible_Hook);

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

	static auto __stdcall ShowCursor_Hook(BOOL bShow) -> int {
		if (bShow) return 1;
		return -1;
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
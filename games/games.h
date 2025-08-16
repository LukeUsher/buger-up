#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <shobjidl.h>
#include <shlobj.h>
#include <commctrl.h>
#include <Shlwapi.h>
#include <map>

#include "patch-engine.h"

static auto GetCurrentGameInstallDirectory() -> std::string {
	char path[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);
	std::string fullPath(path);
	size_t pos = fullPath.find_last_of("\\/");
	if (pos != std::string::npos) fullPath = fullPath.substr(0, pos + 1);
	return fullPath;
}

static auto GetCurrentGameExecutablePath() -> std::string{
	char path[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);
	return path;
}

// These games require an 8bpp window which windows 8+ refuses to provide
// We update the registry to add a DWM mitigation for these games without having to switch the entire desktop to 8bpp
static auto AddDwmMitigationRegistry() -> bool {
    std::string exePath = GetCurrentGameExecutablePath();
    if (exePath.empty() || !PathFileExistsA(exePath.c_str())) return false;

    HKEY hKey;
    LONG res = RegCreateKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers",
      0, nullptr, 0, KEY_READ | KEY_WRITE, nullptr, &hKey, nullptr);

    if (res != ERROR_SUCCESS) return false;

    char existingData[256] = {};
    DWORD dataSize = sizeof(existingData);
    res = RegGetValueA(hKey, nullptr, exePath.c_str(), RRF_RT_REG_SZ, nullptr, existingData, &dataSize);

    if (res == ERROR_SUCCESS && std::string(existingData) == "$ DWM8And16BitMitigation") {
        RegCloseKey(hKey);
        return false;
    }

    // Otherwise, write the value
    const char valueData[] = "$ DWM8And16BitMitigation";
    res = RegSetValueExA(hKey, exePath.c_str(), 0, REG_SZ, (BYTE*)(valueData), sizeof(valueData));
    RegCloseKey(hKey);
    return res == ERROR_SUCCESS;
}

static auto RelaunchProcess() -> void {
    std::string exePath = GetCurrentGameExecutablePath();
    if (exePath.empty()) return;

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (CreateProcessA(exePath.c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        ExitProcess(0);
    }
}

static auto EnsureDwmMitigationAndRelaunch() -> void {
    if (AddDwmMitigationRegistry()) RelaunchProcess();
}


struct Game {
	virtual auto applyPatches(std::string hash) -> bool { return false; };
};

#include "bug.h"
#include "bug-too.h"
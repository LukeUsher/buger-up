#include <string>
#include <algorithm>
#include <cwctype>
#include <filesystem>
#include <ShlObj.h>
#include <deque>
#include <thread>
#include <iostream>
#include <fstream>

#include "game-manager.h"
#include "patch-engine.h"
#include "thirdparty/xxhash32.h"

GameManager gameManager;

auto GameManager::init() -> bool {
#if _DEBUG
	if(AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
		FILE* out_stream;
		FILE* in_stream;
		FILE* err_stream;
		freopen_s(&out_stream, "CONOUT$", "wt", stdout);
		freopen_s(&in_stream, "CONIN$", "rt", stdin);
		freopen_s(&err_stream, "CONOUT$", "wt", stderr);
		std::ios::sync_with_stdio(1);
	}

	// Give a chance to attach a debugger before proceeding
	MessageBoxA(0, "DEBUG!", "DEBUG", 0);
#endif

	printf("BuggerUp! %s %s\n", __DATE__, __TIME__);

	auto hash = getCurrentGameHash();
	
	//TODO: Add support for more games/variants
	if (hash == "1ededa0a") _game = std::make_shared<Bug>();
	if (hash == "b8446528") _game = std::make_shared<BugToo>();

	if (!_game) {
		MessageBoxA(NULL, hash.c_str(), "Unknown game hash", MB_ICONERROR);
		return false;
	}
	
	if (!_game->applyPatches(hash)) {
		MessageBoxA(NULL, hash.c_str(), "Failed to apply game patches", MB_ICONERROR);
		return false;
	}

	return true;
}

auto GameManager::getCurrentGameHash() -> std::string {
	char exePath[MAX_PATH] = { 0 };
	if (!GetModuleFileNameA(NULL, exePath, MAX_PATH)) {
		MessageBoxA(NULL, "Unable to detect game", "Error", MB_ICONERROR);
		ExitProcess(0);
	}
	
	std::ifstream file(exePath, std::ios::binary);
	if (!file) {
		MessageBoxA(NULL, "Unable to detect game", "Error", MB_ICONERROR);
		ExitProcess(0);
	}

	XXHash32 hasher(0);
	char buffer[8192];
	while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
		hasher.add(buffer, file.gcount());
	}

	uint32_t hash = hasher.hash();

	std::ostringstream oss;
	oss << std::hex;
	oss.width(8);
	oss.fill('0');
	oss << hash;

	return oss.str();
}

auto GameManager::getCurrentGameInstallDirectory() -> std::string {
	char path[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);

	std::string fullPath(path);
	size_t pos = fullPath.find_last_of("\\/");
	if (pos != std::string::npos) fullPath = fullPath.substr(0, pos + 1);

	return fullPath;
}

auto GameManager::shutdown() -> void {

}

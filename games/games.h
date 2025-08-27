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
#include <algorithm>
#include <utility>

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

struct Game {
	virtual auto applyPatches(std::string hash) -> bool { return false; };
};

#include "bug.h"
#include "bug-too.h"
#include "comix-zone.h"
#include "ecco.h"
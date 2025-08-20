#pragma once

#include <thread>
#include <string>
#include <deque>
#include "ddraw.h"
#include <mmeapi.h>
#include <mmsystem.h>
#include <joystickapi.h>
#include <Xinput.h>
#include <atomic>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

#pragma comment(lib, "xinput.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Shlwapi.lib")

#include "cd-player.hpp"
#include "games/games.h"

class GameManager {
public:
	auto init() -> bool;
	auto shutdown() -> void;
	auto getCurrentGameHash() -> std::string;
	auto getCurrentGameInstallDirectory() -> std::string;

private:
	std::shared_ptr<Game> _game;
};

extern GameManager gameManager;
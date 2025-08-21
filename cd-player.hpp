#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <vector>
#include <string>
#include <thread>
#include <fstream>
#include <algorithm>
#include <atomic>
#include <mutex>
#include <SDL3/SDL.h>

class CdPlayer {
public:
    struct CdTrack {
        std::string filename;
        uint32_t startSector = 0;
        uint32_t endSector = 0;
        bool isData = false;
        int number = 0;
    };

    auto open(uint32_t dataTrackSize) -> bool;
    auto close() -> void;

    // track-based
    auto playTrack(int track, bool loop = false) -> void;
    auto trackCount() const -> int;
    auto trackInfo(int track, uint32_t* start, uint32_t* end) -> bool;

    // sector-based
    auto playSectors(uint32_t start, uint32_t end, bool loop = false) -> void;
    auto stop() -> void;
    auto pause() -> void;
    auto resume() -> void;

    auto position() const -> uint32_t;

private:
    auto playbackThread() -> void;

    std::vector<CdTrack> _tracks;
    std::thread _thread;
    std::atomic<bool> _loop = false;
    std::atomic<uint32_t> _position = 0;
    std::atomic<bool> _terminating;

    uint64_t _currentByte = 0;
    uint64_t _endByte = 0;

    std::mutex _mutex;
    SDL_AudioStream* _stream;
};

extern CdPlayer cdPlayer;
#pragma once

#include <windows.h>
#include <mmsystem.h>
#include <xinput.h>
#include <atomic>
#include <thread>
#include <map>

struct WinmmJoy {
    enum class DpadMode {
        POV,
        Axis,
        Both
    };

    struct JoyCapture {
        HWND hwnd = nullptr;
        UINT uJoyID = 0;
        UINT uPeriod = 10;
        std::atomic<bool> active = false;
        std::thread pollThread;
        DWORD lastButtons = 0;
        int lastX = 0, lastY = 0;
        UINT threshold = 0;
        ~JoyCapture();
    };

    static auto applyPatches() -> void;

    inline static DpadMode dpadMode = DpadMode::Axis;

    // API hooks
    static auto __stdcall ThumbToJoyRange(LONG val) -> DWORD;
    static auto __stdcall MapButtons(WORD xb) -> DWORD;
    static auto  __stdcall MapFaceButtons(WORD xb) -> WORD;

    static auto __stdcall joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc) -> MMRESULT;
    static auto __stdcall joyGetPosEx(UINT uJoyID, LPJOYINFOEX pji) -> MMRESULT;
    static auto __stdcall joyGetNumDevs() -> UINT;
    static auto __stdcall joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged) -> MMRESULT;
    static auto __stdcall joySetThreshold(UINT uJoyID, UINT uThreshold) -> MMRESULT;

private:
    static auto PollJoystick(JoyCapture* capture) -> void;

    inline static std::map<UINT, JoyCapture> captures{};
};

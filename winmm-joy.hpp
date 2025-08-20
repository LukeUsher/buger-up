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

    inline static DpadMode dpadMode = DpadMode::Axis;

    // API hooks
    static DWORD __stdcall ThumbToJoyRange(LONG val);
    static DWORD __stdcall MapButtons(WORD xb);
    static WORD  __stdcall MapFaceButtons(WORD xb);

    static MMRESULT __stdcall joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc);
    static MMRESULT __stdcall joyGetPosEx(UINT uJoyID, LPJOYINFOEX pji);
    static UINT     __stdcall joyGetNumDevs();
    static MMRESULT __stdcall joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged);
    static MMRESULT __stdcall joySetThreshold(UINT uJoyID, UINT uThreshold);

private:
    inline static std::map<UINT, JoyCapture> captures{};
    static void PollJoystick(JoyCapture* capture);
};

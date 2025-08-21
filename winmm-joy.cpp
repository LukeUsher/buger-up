#include "winmm-joy.hpp"
#include <string.h>
 #include "patch-engine.h"

auto WinmmJoy::applyPatches() -> void {
    patchEngine.PatchImportedFunction("winmm.dll", "joyGetDevCapsA", WinmmJoy::joyGetDevCapsA);
    patchEngine.PatchImportedFunction("winmm.dll", "joyGetNumDevs", WinmmJoy::joyGetNumDevs);
    patchEngine.PatchImportedFunction("winmm.dll", "joyGetPosEx", WinmmJoy::joyGetPosEx);
    patchEngine.PatchImportedFunction("winmm.dll", "joySetCapture", WinmmJoy::joySetCapture);
    patchEngine.PatchImportedFunction("winmm.dll", "joySetThreshold", WinmmJoy::joySetThreshold);
}

WinmmJoy::JoyCapture::~JoyCapture() {
    active = false;
    if (pollThread.joinable()) pollThread.join();
}

auto __stdcall WinmmJoy::ThumbToJoyRange(LONG val) -> DWORD {
    return static_cast<DWORD>((int)val + 32768);
}

auto __stdcall WinmmJoy::MapButtons(WORD xb) -> DWORD {
    DWORD out = 0;
    if (xb & XINPUT_GAMEPAD_A)              out |= (1u << 0);
    if (xb & XINPUT_GAMEPAD_B)              out |= (1u << 1);
    if (xb & XINPUT_GAMEPAD_X)              out |= (1u << 2);
    if (xb & XINPUT_GAMEPAD_Y)              out |= (1u << 3);
    if (xb & XINPUT_GAMEPAD_LEFT_SHOULDER)  out |= (1u << 4);
    if (xb & XINPUT_GAMEPAD_RIGHT_SHOULDER) out |= (1u << 5);
    if (xb & XINPUT_GAMEPAD_START)          out |= (1u << 6);
    if (xb & XINPUT_GAMEPAD_BACK)           out |= (1u << 7);
    if (xb & XINPUT_GAMEPAD_DPAD_UP)        out |= (1u << 8);
    if (xb & XINPUT_GAMEPAD_DPAD_DOWN)      out |= (1u << 9);
    if (xb & XINPUT_GAMEPAD_DPAD_LEFT)      out |= (1u << 10);
    if (xb & XINPUT_GAMEPAD_DPAD_RIGHT)     out |= (1u << 11);
    if (xb & XINPUT_GAMEPAD_LEFT_THUMB)     out |= (1u << 12);
    if (xb & XINPUT_GAMEPAD_RIGHT_THUMB)    out |= (1u << 13);
    return out;
}

auto __stdcall WinmmJoy::MapFaceButtons(WORD xb) -> WORD {
    WORD out = 0;
    if (xb & XINPUT_GAMEPAD_A) out |= JOY_BUTTON1;
    if (xb & XINPUT_GAMEPAD_B) out |= JOY_BUTTON2;
    if (xb & XINPUT_GAMEPAD_X) out |= JOY_BUTTON3;
    if (xb & XINPUT_GAMEPAD_Y) out |= JOY_BUTTON4;
    return out;
}

auto __stdcall WinmmJoy::joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc) -> MMRESULT {
    if (!pjc || cbjc < sizeof(JOYCAPSA)) return MMSYSERR_INVALPARAM;
    if (uJoyID >= 4) return MMSYSERR_NODRIVER;

    ZeroMemory(pjc, cbjc);
    pjc->wMid = 0;
    pjc->wPid = 0;
    strncpy_s(pjc->szPname, "XInput Controller", MAXPNAMELEN);
    pjc->wXmin = 0;    pjc->wXmax = 0xFFFF;
    pjc->wYmin = 0;    pjc->wYmax = 0xFFFF;
    pjc->wZmin = 0;    pjc->wZmax = 0xFFFF;
    pjc->wNumButtons = 14;
    pjc->wPeriodMin = 0;
    pjc->wPeriodMax = 0;

    return JOYERR_NOERROR;
}

auto __stdcall WinmmJoy::joyGetPosEx(UINT uJoyID, LPJOYINFOEX pji) -> MMRESULT {
    if (uJoyID >= 4) return MMSYSERR_NODRIVER;

    XINPUT_STATE xi{};
    DWORD res = XInputGetState(uJoyID, &xi);

    pji->dwSize = sizeof(JOYINFOEX);

    if (res == ERROR_SUCCESS) {
        LONG lx = xi.Gamepad.sThumbLX;
        LONG ly = xi.Gamepad.sThumbLY;

        if (dpadMode == DpadMode::Axis || dpadMode == DpadMode::Both) {
            if (xi.Gamepad.wButtons & (XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT | XINPUT_GAMEPAD_DPAD_UP | XINPUT_GAMEPAD_DPAD_DOWN)) {
                lx = 0;
                ly = 0;
                if (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)  lx = -32768;
                if (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) lx = 32767;
                if (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)    ly = 32767;
                if (xi.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  ly = -32768;
            }
        }

        if (pji->dwFlags & JOY_RETURNX) pji->dwXpos = ThumbToJoyRange(lx);
        if (pji->dwFlags & JOY_RETURNY) pji->dwYpos = ThumbToJoyRange(-ly);
        if (pji->dwFlags & JOY_RETURNZ) pji->dwZpos = xi.Gamepad.bRightTrigger * 257;
        if (pji->dwFlags & JOY_RETURNR) pji->dwRpos = xi.Gamepad.bLeftTrigger * 257;

        if (pji->dwFlags & JOY_RETURNBUTTONS) {
            pji->dwButtons = MapButtons(xi.Gamepad.wButtons);
            pji->dwButtonNumber = 0;
            for (int i = 0; i < 32; ++i) {
                if (pji->dwButtons & (1u << i)) {
                    pji->dwButtonNumber = i + 1;
                    break;
                }
            }
        }

        if ((dpadMode == DpadMode::POV || dpadMode == DpadMode::Both) &&
            (pji->dwFlags & JOY_RETURNPOV)) {
            WORD b = xi.Gamepad.wButtons;
            if (b & XINPUT_GAMEPAD_DPAD_UP) {
                if (b & XINPUT_GAMEPAD_DPAD_RIGHT) pji->dwPOV = 4500;
                else if (b & XINPUT_GAMEPAD_DPAD_LEFT) pji->dwPOV = 31500;
                else pji->dwPOV = 0;
            }
            else if (b & XINPUT_GAMEPAD_DPAD_RIGHT) pji->dwPOV = 9000;
            else if (b & XINPUT_GAMEPAD_DPAD_DOWN) {
                if (b & XINPUT_GAMEPAD_DPAD_RIGHT) pji->dwPOV = 13500;
                else if (b & XINPUT_GAMEPAD_DPAD_LEFT) pji->dwPOV = 22500;
                else pji->dwPOV = 18000;
            }
            else if (b & XINPUT_GAMEPAD_DPAD_LEFT) pji->dwPOV = 27000;
            else pji->dwPOV = JOY_POVCENTERED;
        }
    } else {
        pji->dwXpos = 0x8000;
        pji->dwYpos = 0x8000;
        pji->dwZpos = 0;
        pji->dwRpos = 0;
        pji->dwButtons = 0;
        pji->dwButtonNumber = 0;
        pji->dwPOV = JOY_POVCENTERED;
    }

    return JOYERR_NOERROR;
}

auto __stdcall WinmmJoy::joyGetNumDevs() -> UINT {
    return 4;
}

auto __stdcall WinmmJoy::joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged) -> MMRESULT {
    if (uJoyID >= 4) return MMSYSERR_NODRIVER;

    if (captures.count(uJoyID)) {
        captures[uJoyID].active = false;
        if (captures[uJoyID].pollThread.joinable()) captures[uJoyID].pollThread.join();
    }

    JoyCapture& capture = captures[uJoyID];
    capture.hwnd = hwnd;
    capture.uJoyID = uJoyID;
    capture.uPeriod = uPeriod;
    capture.active = true;
    capture.pollThread = std::thread(PollJoystick, &capture);

    return JOYERR_NOERROR;
}

auto __stdcall WinmmJoy::joySetThreshold(UINT uJoyID, UINT uThreshold) -> UINT {
    if (uJoyID >= 4) return MMSYSERR_NODRIVER;
    if (captures.count(uJoyID)) captures[uJoyID].threshold = uThreshold;
    return JOYERR_NOERROR;
}

auto WinmmJoy::PollJoystick(JoyCapture* capture) -> void {
    while (capture->active.load()) {
        XINPUT_STATE state{};
        if (XInputGetState(capture->uJoyID, &state) != ERROR_SUCCESS) {
            Sleep(capture->uPeriod);
            continue;
        }

        WORD wButtons = MapFaceButtons(state.Gamepad.wButtons);
        LONG lx = state.Gamepad.sThumbLX;
        LONG ly = state.Gamepad.sThumbLY;

        int dirX = 0, dirY = 0;
        const LONG DEADZONE = 8000;

        if (state.Gamepad.sThumbLX < -DEADZONE) dirX = -1;
        else if (state.Gamepad.sThumbLX > DEADZONE) dirX = 1;

        if (state.Gamepad.sThumbLY < -DEADZONE) dirY = 1;
        else if (state.Gamepad.sThumbLY > DEADZONE) dirY = -1;

        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)  dirX = -1;
        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) dirX = 1;
        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)    dirY = -1;
        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  dirY = 1;

        DWORD dwX = (dirX + 1) * 32767;
        DWORD dwY = (dirY + 1) * 32767;

        if (dirX != capture->lastX || dirY != capture->lastY) {
            SendMessage(capture->hwnd, MM_JOY1MOVE + capture->uJoyID, 0, MAKELPARAM(dwX, dwY));
            capture->lastX = dirX;
            capture->lastY = dirY;
        }

        WORD changed = wButtons ^ (WORD)capture->lastButtons;
        if (changed) {
            if (changed & JOY_BUTTON1) {
                SendMessage(capture->hwnd,
                    (wButtons & JOY_BUTTON1) ? MM_JOY1BUTTONDOWN + capture->uJoyID : MM_JOY1BUTTONUP + capture->uJoyID,
                    wButtons | JOY_BUTTON1CHG,
                    MAKELPARAM(dwX, dwY));
            }
            if (changed & JOY_BUTTON2) {
                SendMessage(capture->hwnd,
                    (wButtons & JOY_BUTTON2) ? MM_JOY1BUTTONDOWN + capture->uJoyID : MM_JOY1BUTTONUP + capture->uJoyID,
                    wButtons | JOY_BUTTON2CHG,
                    MAKELPARAM(dwX, dwY));
            }
            if (changed & JOY_BUTTON3) {
                SendMessage(capture->hwnd,
                    (wButtons & JOY_BUTTON3) ? MM_JOY1BUTTONDOWN + capture->uJoyID : MM_JOY1BUTTONUP + capture->uJoyID,
                    wButtons | JOY_BUTTON3CHG,
                    MAKELPARAM(dwX, dwY));
            }
            if (changed & JOY_BUTTON4) {
                SendMessage(capture->hwnd,
                    (wButtons & JOY_BUTTON4) ? MM_JOY1BUTTONDOWN + capture->uJoyID : MM_JOY1BUTTONUP + capture->uJoyID,
                    wButtons | JOY_BUTTON4CHG,
                    MAKELPARAM(dwX, dwY));
            }
        }

        capture->lastButtons = wButtons;
        capture->lastX = dwX;
        capture->lastY = dwY;

        Sleep(capture->uPeriod);
    }
}

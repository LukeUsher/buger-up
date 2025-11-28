#include "winmm.hpp"
#include <string.h>
#include "patch-engine.h"
#include "logger.hpp"
#include "cd-player.hpp"

Winmm winmm;

static inline decltype(&mciGetDeviceIDA) _mciGetDeviceIDA = nullptr;
static inline decltype(&mciSendCommandA) _mciSendCommandA = nullptr;

static const MCIDEVICEID CD_DEVICE_ID = 0x1234;
static auto _cdTimeFormat = MCI_FORMAT_MSF;

auto Winmm::applyPatches() -> void {
	winmm.maxJoysticks = 4; // Default to 4 joysticks, games may override this

    patchEngine.PatchImportedFunction("winmm.dll", "joyGetDevCapsA", Winmm::joyGetDevCapsA);
    patchEngine.PatchImportedFunction("winmm.dll", "joyGetNumDevs", Winmm::joyGetNumDevs);
    patchEngine.PatchImportedFunction("winmm.dll", "joyGetPos", Winmm::joyGetPos);
    patchEngine.PatchImportedFunction("winmm.dll", "joyGetPosEx", Winmm::joyGetPosEx);
    patchEngine.PatchImportedFunction("winmm.dll", "joySetCapture", Winmm::joySetCapture);
    patchEngine.PatchImportedFunction("winmm.dll", "joySetThreshold", Winmm::joySetThreshold);

	patchEngine.PatchImportedFunction("winmm.dll", "mciGetDeviceIDA", Winmm::mciGetDeviceIDA, (void**)&_mciGetDeviceIDA);
	patchEngine.PatchImportedFunction("winmm.dll", "mciSendCommandA", Winmm::mciSendCommandA, (void**)&_mciSendCommandA);
}

Winmm::JoyCapture::~JoyCapture() {
    active = false;
    if (pollThread.joinable()) pollThread.join();
}

auto __stdcall Winmm::ThumbToJoyRange(LONG val) -> DWORD {
    return static_cast<DWORD>((int)val + 32768);
}

auto __stdcall Winmm::MapButtons(WORD xb) -> DWORD {
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

auto __stdcall Winmm::MapFaceButtons(WORD xb) -> WORD {
    WORD out = 0;
    if (xb & XINPUT_GAMEPAD_A) out |= JOY_BUTTON1;
    if (xb & XINPUT_GAMEPAD_B) out |= JOY_BUTTON2;
    if (xb & XINPUT_GAMEPAD_X) out |= JOY_BUTTON3;
    if (xb & XINPUT_GAMEPAD_Y) out |= JOY_BUTTON4;
    return out;
}

auto __stdcall Winmm::joyGetDevCapsA(UINT_PTR uJoyID, LPJOYCAPSA pjc, UINT cbjc) -> MMRESULT {
    TRACE_FUNCTION_ENTRY("winmm");
    TRACE_IN_PARAM(uJoyID);
    TRACE_IN_PARAM(pjc);
    TRACE_IN_PARAM(cbjc);

    if (!pjc || cbjc < sizeof(JOYCAPSA)) {
        TRACE_RETURN(MMSYSERR_INVALPARAM);
    }

    if (uJoyID >= winmm.maxJoysticks) {
        TRACE_RETURN(MMSYSERR_NODRIVER);
    }

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

    TRACE_OUT_PARAM(pjc);
    TRACE_RETURN(JOYERR_NOERROR);
}

auto __stdcall Winmm::joyGetNumDevs() -> UINT {
    TRACE_FUNCTION_ENTRY("winmm");
    TRACE_RETURN(winmm.maxJoysticks);
}

auto __stdcall Winmm::joyGetPos(UINT uJoyID, LPJOYINFO pji) -> MMRESULT {
    TRACE_FUNCTION_ENTRY("winmm");
    TRACE_IN_PARAM(uJoyID);
    TRACE_IN_PARAM(pji);
    if (uJoyID >= winmm.maxJoysticks) {
        TRACE_RETURN(MMSYSERR_NODRIVER);
    }
    XINPUT_STATE xi{};
    auto res = XInputGetState(uJoyID, &xi);
    if (res == ERROR_SUCCESS) {
        pji->wXpos = ThumbToJoyRange(xi.Gamepad.sThumbLX);
        pji->wYpos = ThumbToJoyRange(-xi.Gamepad.sThumbLY);
        pji->wZpos = xi.Gamepad.bRightTrigger * 257;
        pji->wButtons = MapFaceButtons(xi.Gamepad.wButtons);
    } else {
        pji->wXpos = 0x8000;
        pji->wYpos = 0x8000;
        pji->wZpos = 0;
        pji->wButtons = 0;
    }
    TRACE_OUT_PARAM(pji);
    TRACE_RETURN(JOYERR_NOERROR);
}

auto __stdcall Winmm::joyGetPosEx(UINT uJoyID, LPJOYINFOEX pji) -> MMRESULT {
    TRACE_FUNCTION_ENTRY("winmm");
    TRACE_IN_PARAM(uJoyID);
    TRACE_IN_PARAM(pji);

    if (uJoyID >= winmm.maxJoysticks) {
        TRACE_RETURN(MMSYSERR_NODRIVER);
    }

    XINPUT_STATE xi{};
    auto res = XInputGetState(uJoyID, &xi);

    pji->dwSize = sizeof(JOYINFOEX);

    if (res == ERROR_SUCCESS) {
        auto lx = xi.Gamepad.sThumbLX;
        auto ly = xi.Gamepad.sThumbLY;

        if (dpadMode == DpadMode::Axis || dpadMode == DpadMode::Both) {
            if (xi.Gamepad.wButtons & (XINPUT_GAMEPAD_DPAD_LEFT |
                XINPUT_GAMEPAD_DPAD_RIGHT |
                XINPUT_GAMEPAD_DPAD_UP |
                XINPUT_GAMEPAD_DPAD_DOWN)) {
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

            auto b = xi.Gamepad.wButtons;
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

    TRACE_OUT_PARAM(pji);

    TRACE_RETURN(JOYERR_NOERROR);
}

auto __stdcall Winmm::joySetCapture(HWND hwnd, UINT uJoyID, UINT uPeriod, BOOL fChanged) -> MMRESULT {
    TRACE_FUNCTION_ENTRY("winmm");
    TRACE_IN_PARAM(hwnd);
    TRACE_IN_PARAM(uJoyID);
    TRACE_IN_PARAM(uPeriod);
    TRACE_IN_PARAM(fChanged);

    if (uJoyID >= winmm.maxJoysticks) {
        TRACE_RETURN(MMSYSERR_NODRIVER);
    }

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

    TRACE_RETURN(JOYERR_NOERROR);
}

auto __stdcall Winmm::joySetThreshold(UINT uJoyID, UINT uThreshold) -> UINT {
    TRACE_FUNCTION_ENTRY("winmm");
    TRACE_IN_PARAM(uJoyID);
    TRACE_IN_PARAM(uThreshold);

    if (uJoyID >= winmm.maxJoysticks) {
        TRACE_RETURN(MMSYSERR_NODRIVER);
    }

    if (captures.count(uJoyID)) captures[uJoyID].threshold = uThreshold;

    TRACE_RETURN(JOYERR_NOERROR);
}

auto __stdcall Winmm::mciGetDeviceIDA(LPCSTR szDevice) -> MCIDEVICEID {
    TRACE_FUNCTION_ENTRY_STUB("winmm");
	TRACE_IN_PARAM(szDevice);
	TRACE_RETURN(_mciGetDeviceIDA(szDevice));
}

auto __stdcall Winmm::mciSendCommandA(MCIDEVICEID IDDevice, UINT uMsg, DWORD_PTR fdwCommand, DWORD_PTR dwParam) -> MCIERROR {
	TRACE_FUNCTION_ENTRY("winmm");
	TRACE_IN_PARAM(IDDevice);
	TRACE_IN_PARAM(uMsg);
	TRACE_IN_PARAM(fdwCommand);
	TRACE_IN_PARAM(dwParam);

    if (uMsg == MCI_OPEN) {
        auto* open = reinterpret_cast<MCI_OPEN_PARMSA*>(dwParam);
        TRACE_IN_PARAM(open);

        const bool isCDAudio = (fdwCommand & MCI_OPEN_TYPE) && open->lpstrDeviceType && (_stricmp(open->lpstrDeviceType, "cdaudio") == 0);
        if (isCDAudio) {
            open->wDeviceID = CD_DEVICE_ID; 
			cdPlayer.open(1000); // TODO: Read this from the game information
            TRACE_RETURN(0);
        }

        TRACE_RETURN(_mciSendCommandA(IDDevice, uMsg, fdwCommand, dwParam));
    }

    if (IDDevice != CD_DEVICE_ID) {
        TRACE_RETURN(_mciSendCommandA(IDDevice, uMsg, fdwCommand, dwParam));
    }

    switch (uMsg) {
        case MCI_CLOSE:
            cdPlayer.close();
            TRACE_RETURN(0);
        case MCI_PLAY: {
            auto* play = reinterpret_cast<MCI_PLAY_PARMS*>(dwParam);
            TRACE_IN_PARAM(play);

            uint32_t startSector = cdPlayer.position();
            uint32_t endSector = UINT32_MAX;

            if (fdwCommand & MCI_FROM) {
                DWORD from = play->dwFrom;

                if (_cdTimeFormat == MCI_FORMAT_TMSF) {
                    int track = MCI_TMSF_TRACK(from);
                    int m = MCI_TMSF_MINUTE(from);
                    int s = MCI_TMSF_SECOND(from);
                    int f = MCI_TMSF_FRAME(from);

                    uint32_t trackStart;
                    uint32_t trackEnd;

                    if (!cdPlayer.trackInfo(track, &trackStart, &trackEnd)) TRACE_RETURN(MCIERR_BAD_INTEGER);
                    startSector = trackStart + (m * 60 + s) * 75 + f;
                }
                else if (_cdTimeFormat == MCI_FORMAT_MSF) {
                    int m = MCI_MSF_MINUTE(from);
                    int s = MCI_MSF_SECOND(from);
                    int f = MCI_MSF_FRAME(from);
                    startSector = (m * 60 + s) * 75 + f;
                }  else if (fdwCommand & MCI_TRACK) {
                    int track = int(from);
                    uint32_t trackStart;

                    if (!cdPlayer.trackInfo(track, &trackStart, nullptr)) TRACE_RETURN(MCIERR_BAD_INTEGER);
                    startSector = trackStart;
                } else {
                    startSector = from;
                }
            }

            if (fdwCommand & MCI_TO) {
                DWORD to = play->dwTo;

                if (_cdTimeFormat == MCI_FORMAT_TMSF) {
                    int track = MCI_TMSF_TRACK(to);
                    int m = MCI_TMSF_MINUTE(to);
                    int s = MCI_TMSF_SECOND(to);
                    int f = MCI_TMSF_FRAME(to);

                    uint32_t trackStart;
                    uint32_t trackEnd;

                    if (!cdPlayer.trackInfo(track, &trackStart, &trackEnd)) TRACE_RETURN(MCIERR_BAD_INTEGER);

                    endSector = trackStart + (m * 60 + s) * 75 + f;
                } else if (_cdTimeFormat == MCI_FORMAT_MSF) {
                    int m = MCI_MSF_MINUTE(to);
                    int s = MCI_MSF_SECOND(to);
                    int f = MCI_MSF_FRAME(to);

                    endSector = (m * 60 + s) * 75 + f;
                } else if (fdwCommand & MCI_TRACK) {
                    int track = int(to);

                    uint32_t trackEnd;
                    if (!cdPlayer.trackInfo(track, nullptr, &trackEnd)) TRACE_RETURN(MCIERR_BAD_INTEGER);

                    endSector = trackEnd;
                } else {
                    endSector = to;
                }
            }

            cdPlayer.playSectors(startSector, endSector);

            if (fdwCommand & MCI_NOTIFY) {
                HWND cb = (HWND)play->dwCallback;
                winmm.mciNotifyRegister(cb, true);
            } else {
                winmm.mciNotifyClear();
            }

            TRACE_RETURN(0);
        }
        case MCI_STOP:
            cdPlayer.stop();
            TRACE_RETURN(0);
        case MCI_PAUSE:
            cdPlayer.pause();
            TRACE_RETURN(0);
        case MCI_RESUME:
            cdPlayer.resume();
            TRACE_RETURN(0);
        case MCI_SET: {
            auto* setp = reinterpret_cast<MCI_SET_PARMS*>(dwParam);
            TRACE_IN_PARAM(setp);

            if (fdwCommand & MCI_SET_TIME_FORMAT) {
                _cdTimeFormat = setp->dwTimeFormat;
            }

            TRACE_RETURN(0);
        }
        case MCI_STATUS: {
            auto* st = reinterpret_cast<MCI_STATUS_PARMS*>(dwParam);
            TRACE_IN_PARAM(st);

            if (fdwCommand & MCI_STATUS_ITEM) {
                TRACE_IN_PARAM(st->dwItem);

                switch (st->dwItem) {
                    case MCI_STATUS_MODE:
                        st->dwReturn = cdPlayer.playing() ? MCI_MODE_PLAY : MCI_MODE_STOP;
                        TRACE_RETURN(0);
                    case MCI_STATUS_MEDIA_PRESENT:
                        st->dwReturn = 1;
                        TRACE_RETURN(0);
                    case MCI_STATUS_NUMBER_OF_TRACKS:
                        st->dwReturn = cdPlayer.trackCount();
                        TRACE_RETURN(0);
                    case MCI_STATUS_CURRENT_TRACK:
                        st->dwReturn = cdPlayer.trackFromPosition(cdPlayer.position());
                        TRACE_RETURN(0);
                    case MCI_STATUS_TIME_FORMAT:
                        st->dwReturn = _cdTimeFormat;
                        TRACE_RETURN(0);
                    case MCI_STATUS_LENGTH: {
                        uint32_t start = 0;
                        uint32_t end = 0;

                        if ((fdwCommand & MCI_TRACK)) {
                            const int track = st->dwTrack;
                            if (!cdPlayer.trackInfo(track, &start, &end)) TRACE_RETURN(MCIERR_BAD_INTEGER);
                        }  else {
                            const int last = cdPlayer.trackCount();
                            if (last <= 0) TRACE_RETURN(MCIERR_DEVICE_NOT_READY);

                            if (!cdPlayer.trackInfo(1, &start, nullptr)) TRACE_RETURN(MCIERR_BAD_INTEGER);

                            uint32_t endDisc = 0;
                            if (!cdPlayer.trackInfo(last, nullptr, &endDisc)) TRACE_RETURN(MCIERR_BAD_INTEGER);
                            end = endDisc;
                        }

                        uint32_t length = (end > start) ? (end - start) : 0;

                        if (_cdTimeFormat == MCI_FORMAT_MSF) {
                            int m = length / (75 * 60);
                            int s = (length / 75) % 60;
                            int f = length % 75;

                            st->dwReturn = MCI_MAKE_MSF(m, s, f);
                            TRACE_RETURN(0);
                        }

                        if (_cdTimeFormat == MCI_FORMAT_TMSF) {
                            int track = (fdwCommand & MCI_TRACK) ? st->dwTrack : 0;

                            int m = length / (75 * 60);
                            int s = (length / 75) % 60;
                            int f = length % 75;

                            st->dwReturn = MCI_MAKE_TMSF(track, m, s, f);
                            TRACE_RETURN(0);
                        }

                        st->dwReturn = length;
                        TRACE_RETURN(0);
                    }
                    case MCI_STATUS_POSITION: {
                        uint32_t pos = cdPlayer.position();
                        if (pos == 0) pos = cdPlayer.trackStartSector(1);

                        if (fdwCommand & MCI_TRACK) {
                            pos = cdPlayer.trackStartSector(st->dwTrack);
                        }

                        if (_cdTimeFormat == MCI_FORMAT_MSF) {
                            int m = pos / (75 * 60);
                            int s2 = (pos / 75) % 60;
                            int f = pos % 75;

                            st->dwReturn = MCI_MAKE_MSF(m, s2, f);
                            TRACE_RETURN(0);
                        }

                        if (_cdTimeFormat == MCI_FORMAT_TMSF) {
                            int track = cdPlayer.trackFromPosition(pos);

                            uint32_t trackStart = cdPlayer.trackStartSector(track);
                            uint32_t offset = pos - trackStart;

                            int m = offset / (75 * 60);
                            int s = (offset / 75) % 60;
                            int f = offset % 75;

                            st->dwReturn = MCI_MAKE_TMSF(track, m, s, f);
                            TRACE_RETURN(0);
                        }

                        st->dwReturn = pos;
                        TRACE_RETURN(0);
                    }
                }
            }
        }
    }

	TRACE_RETURN(MCIERR_UNSUPPORTED_FUNCTION);
}

auto Winmm::mciNotifyRegister(HWND hwnd, bool enabled) -> void {
    HWND old = _mciNotifyHwnd;
    bool oldEnabled = _mciNotifyEnabled;
    if (oldEnabled && old && old != hwnd) {
        PostMessage(old, MM_MCINOTIFY, MCI_NOTIFY_SUPERSEDED, CD_DEVICE_ID);
    }
    _mciNotifyHwnd = hwnd;
    _mciNotifyEnabled = enabled;
}

auto Winmm::mciNotifyClear() -> void {
    _mciNotifyEnabled = false;
    _mciNotifyHwnd = nullptr;
}

auto Winmm::mciNotifyCd(UINT status) -> void {
    HWND hwnd = _mciNotifyHwnd;
    if (_mciNotifyEnabled && hwnd) {
        PostMessage(hwnd, MM_MCINOTIFY, status, CD_DEVICE_ID);
    }
}

auto Winmm::PollJoystick(JoyCapture* capture) -> void {
    while (capture->active) {
        XINPUT_STATE state{};
        if (XInputGetState(capture->uJoyID, &state) != ERROR_SUCCESS) {
            Sleep(capture->uPeriod);
            continue;
        }

        auto wButtons = MapFaceButtons(state.Gamepad.wButtons);
        auto lx = state.Gamepad.sThumbLX;
        auto ly = state.Gamepad.sThumbLY;

        auto dirX = 0, dirY = 0;
        const auto DEADZONE = 8000;

        if (state.Gamepad.sThumbLX < -DEADZONE) dirX = -1;
        else if (state.Gamepad.sThumbLX > DEADZONE) dirX = 1;

        if (state.Gamepad.sThumbLY < -DEADZONE) dirY = 1;
        else if (state.Gamepad.sThumbLY > DEADZONE) dirY = -1;

        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)  dirX = -1;
        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) dirX = 1;
        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)    dirY = -1;
        if (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  dirY = 1;

        auto dwX = (dirX + 1) * 32767;
        auto dwY = (dirY + 1) * 32767;

        if (dirX != capture->lastX || dirY != capture->lastY) {
            SendMessage(capture->hwnd, MM_JOY1MOVE + capture->uJoyID, 0, MAKELPARAM(dwX, dwY));
            capture->lastX = dirX;
            capture->lastY = dirY;
        }

        auto changed = wButtons ^ (WORD)capture->lastButtons;
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

struct ComixZone : Game
{
	auto applyPatches(std::string hash) -> bool override {
		// Fullscreen: always use the windowed rendering path because modern windows does not have DISPLAYDIB window support
		patchEngine.InjectJump(0x00402F7F, 0x00402FD7);

		// Fullscreen: connect fullscreen switch to resizing the main window rather than using a DISPLAYDIB
        patchEngine.InjectCall(0x004025E2, setFullscreen);
        patchEngine.InjectCall(0x00402635, setFullscreen);
        patchEngine.InjectCall(0x00402C27, setFullscreen);
        patchEngine.InjectCall(0x00402C97, setFullscreen);
        patchEngine.InjectCall(0x00402CE1, setFullscreen);

		return true;
	}

    static auto __stdcall setFullscreen(HWND hWnd, bool enable) -> void {
        //The game will pass an invalid HWND here (what would be the DISPLAYDIB window)
		//but we know the offset of the real hwnd in memory, so we can just read it directly
        hWnd = *(HWND*)0x00483B70;
        static HMENU savedMenu = NULL;
        static RECT savedRect = { 0 };

        if (enable) {
            GetWindowRect(hWnd, &savedRect);

            savedMenu = GetMenu(hWnd);
            if (savedMenu) {
                SetMenu(hWnd, NULL);
                DrawMenuBar(hWnd);
            }

            LONG style = GetWindowLongA(hWnd, GWL_STYLE);
            LONG ex = GetWindowLongA(hWnd, GWL_EXSTYLE);

            style &= ~(WS_OVERLAPPEDWINDOW);
            ex &= ~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME);

            SetWindowLongA(hWnd, GWL_STYLE, style);
            SetWindowLongA(hWnd, GWL_EXSTYLE, ex);

            MONITORINFO mi = { sizeof(mi) };
            GetMonitorInfoA(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi);

            SetWindowPos(hWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_FRAMECHANGED | SWP_NOZORDER);
            return;
        }

        LONG style = GetWindowLongA(hWnd, GWL_STYLE);
        LONG ex = GetWindowLongA(hWnd, GWL_EXSTYLE);

        style |= WS_OVERLAPPEDWINDOW;
        ex |= WS_EX_WINDOWEDGE;

        SetWindowLongA(hWnd, GWL_STYLE, style);
        SetWindowLongA(hWnd, GWL_EXSTYLE, ex);

        if (savedMenu) {
            SetMenu(hWnd, savedMenu);
            DrawMenuBar(hWnd);
        }

        SetWindowPos(hWnd, HWND_NOTOPMOST, savedRect.left, savedRect.top, savedRect.right - savedRect.left, savedRect.bottom - savedRect.top, SWP_FRAMECHANGED);
    }
};

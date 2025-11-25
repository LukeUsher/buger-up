#include "ddraw/ddraw.hpp"

struct Ecco : Game
{
    static inline decltype(&GetPrivateProfileString) _GetPrivateProfileStringA = nullptr;
    static inline decltype(&StretchDIBits) _StretchDIBits = nullptr;
    static inline SDL_Surface* _staticBitmapSurface = nullptr;

    auto applyPatches(std::string hash) -> bool override {
        //No-CD: Hook GetPrivateProfileStringA to Set the CDPath to the games directory
        patchEngine.PatchImportedFunction("kernel32.dll", "GetPrivateProfileStringA", (void*)GetPrivateProfileStringA_Hook, (void**)&_GetPrivateProfileStringA);

        //No-CD: Skip error message when data files are detected in a location that is not a CD-ROM drive
        patchEngine.InjectJump(0x004070E8, 0x00407110);

        //Prevent direct hardware IO during VBlank wait
        patchEngine.InjectJump(0x0040A205, 0x0040A242);
        patchEngine.InjectJump(0x0040A237, 0x0040A242);

		// Fullscreen fix: prevent the game from creating a new fullscreen window
        patchEngine.PatchFunction("CreateFullscreenWindow", 0x00411E00, CreateFullscreenWindow_Hook);

        // Fullscreen fix: prevent the games WndProc from racing SDL
        patchEngine.PatchFunction("WndProc", 0x00405B00, WndProc_Hook);

        //Fix the 'paused' bitmap
        patchEngine.PatchImportedFunction("gdi32.dll", "StretchDIBits", (void*)StretchDIBits_Hook, (void**)&_StretchDIBits);

        // Fix broken savedata dialogs
		patchEngine.PatchBinary(0x0040AD6B, { 0x68, 0xFF, 0xFF, 0xFF, 0xFF }); // push 0xFFFF => push 0xFFFFFFFF
        return true;
    }

    static auto WINAPI GetPrivateProfileStringA_Hook(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName) -> DWORD {
        if (lpKeyName && std::string(lpKeyName) == "path") {
            auto path = GetCurrentGameInstallDirectory();
            if (lpReturnedString && nSize > 0) {
                auto copySize = std::min(path.size(), static_cast<std::size_t>(nSize - 1));
                std::copy(path.begin(), path.begin() + copySize, lpReturnedString);
                lpReturnedString[copySize] = '\0';
            }
            return static_cast<DWORD>(path.size());
        }

        if (lpKeyName && std::string(lpKeyName) == "CDPath") {
            auto path = GetCurrentGameInstallDirectory() + std::string("\\DATA");
            if (lpReturnedString && nSize > 0) {
                auto copySize = std::min(path.size(), static_cast<std::size_t>(nSize - 1));
                std::copy(path.begin(), path.begin() + copySize, lpReturnedString);
                lpReturnedString[copySize] = '\0';
            }
            return static_cast<DWORD>(path.size());
        }

        return _GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
    }

    static auto __cdecl CreateFullscreenWindow_Hook() -> void {
		auto g_FullscreenWindow = reinterpret_cast<HWND*>(0x00488B40);
        auto g_MainWindow = *reinterpret_cast<HWND*>(0x00488970);
        *g_FullscreenWindow = nullptr;
	}

    static auto __stdcall WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT {
        auto _wndProc = reinterpret_cast<decltype(&WndProc_Hook)>(patchEngine.GetOriginalFunctionPointer("WndProc"));

        if(directDraw._cooperativeLevel & DDSCL_FULLSCREEN) {
            switch (uMsg) {
                case WM_WINDOWPOSCHANGING:
                case WM_WINDOWPOSCHANGED:
                case WM_SIZE:
                case WM_MOVE:
                case WM_STYLECHANGING:
                case WM_STYLECHANGED:
                case WM_DISPLAYCHANGE:
                case WM_NCACTIVATE:
                case WM_ACTIVATE:
                case WM_ACTIVATEAPP:
                    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
            }
		}

		return _wndProc(hWnd, uMsg, wParam, lParam);
	}

    //Ecco uses this function to render a single fullscreen bitmap so we don't need to be too smart here.
    static int WINAPI StretchDIBits_Hook(HDC hdc, int xDest, int yDest, int DestWidth, int DestHeight,
        int XSrc, int YSrc, int SrcWidth, int SrcHeight, const void* lpBits, const BITMAPINFO* bmi, UINT iUsage, DWORD dwRop) {

        if (!_staticBitmapSurface) {
            HMODULE exe = GetModuleHandleA(nullptr);

            HRSRC res = FindResourceA(exe, MAKEINTRESOURCEA(107), RT_BITMAP);
            if (!res) return SrcHeight;

            DWORD size = SizeofResource(exe, res);
            HGLOBAL handle = LoadResource(exe, res);
            uint8_t* data = (uint8_t*)LockResource(handle);
            if (!data) return SrcHeight;

            BITMAPINFOHEADER* bih = (BITMAPINFOHEADER*)data;
            int w = bih->biWidth;
            int h = std::abs(bih->biHeight);
            bool topDown = bih->biHeight < 0;

            RGBQUAD* palette = (RGBQUAD*)(data + bih->biSize);
            uint8_t* pixels = (uint8_t*)palette + (bih->biClrUsed ? bih->biClrUsed : 256) * sizeof(RGBQUAD);
            int srcStride = ((w + 1) & ~1);

            _staticBitmapSurface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_ARGB8888);
            uint32_t* dst = (uint32_t*)_staticBitmapSurface->pixels;
            int dstPitch32 = _staticBitmapSurface->pitch / 4;

            for (int y = 0; y < h; ++y) {
                int srcY = topDown ? y : h - 1 - y;
                uint8_t* srcRow = pixels + srcY * srcStride;
                uint32_t* dstRow = dst + y * dstPitch32;

                for (int x = 0; x < w; ++x) {
                    RGBQUAD c = palette[srcRow[x]];
                    dstRow[x] = 0xFF000000 | (c.rgbRed << 16) | (c.rgbGreen << 8) | (c.rgbBlue);
                }
            }
        }

        SDL_Surface* windowSurface = SDL_GetWindowSurface(directDraw._window);
        if (!windowSurface) return SrcHeight;

        SDL_BlitSurfaceScaled(_staticBitmapSurface, nullptr, windowSurface, nullptr, SDL_SCALEMODE_NEAREST);
        SDL_UpdateWindowSurface(directDraw._window);

        return SrcHeight;
    }

};

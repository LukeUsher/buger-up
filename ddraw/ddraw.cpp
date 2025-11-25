#include "ddraw.hpp"
#include "surface.hpp"
#include "clipper.hpp"
#include "palette.hpp"

#include "../gdi32/gdi32.hpp"
#include "../logger.hpp"

#include "../patch-engine.h"

#include <initguid.h>
DEFINE_GUID(IID_IDirectDraw,        0x6C14DB80, 0xA733, 0x11CE, 0xA5, 0xDC, 0x00, 0xAA, 0x00, 0xB9, 0x33, 0x56);
DEFINE_GUID(IID_IDirectDrawSurface, 0x6C14DB81, 0xA733, 0x11CE, 0xA5, 0xDC, 0x00, 0xAA, 0x00, 0xB9, 0x33, 0x56);
DEFINE_GUID(IID_IDirectDrawPalette, 0x6C14DB82, 0xA733, 0x11CE, 0xA5, 0xDC, 0x00, 0xAA, 0x00, 0xB9, 0x33, 0x56);
DEFINE_GUID(IID_IDirectDrawClipper, 0x6C14DB83, 0xA733, 0x11CE, 0xA5, 0xDC, 0x00, 0xAA, 0x00, 0xB9, 0x33, 0x56);

DirectDraw directDraw;

auto DirectDraw::applyPatches() -> void {
    patchEngine.PatchImportedFunction("ddraw.dll", "DirectDrawCreate", DirectDrawCreate);
}

auto DirectDraw::flipPrimary() -> bool {
    if (!_primarySurface) return false;
    if (!_primarySurface->_surface) return false;

    // In non-exclusive mode, we have to do the final blit using the GDI palette, not the DDD3D palette
    if ((_cooperativeLevel & DDSCL_EXCLUSIVE) == 0 || !_primarySurface->_palette) {
        SDL_SetSurfacePalette(_primarySurface->_surface, gdi32._systemPalette);
    } else {
		SDL_SetSurfacePalette(_primarySurface->_surface, _primarySurface->_palette->_palette);
    }

	auto _windowSurface = SDL_GetWindowSurface(_window);
	if (!_windowSurface) return false;

    SDL_Rect windowRect;
	SDL_GetWindowSize(_window, &windowRect.w, &windowRect.h);
    SDL_GetWindowPosition(_window, &windowRect.x, &windowRect.y);

    if (_cooperativeLevel & DDSCL_FULLSCREEN) {
        int srcW = _primarySurface->_surface->w;
        int srcH = _primarySurface->_surface->h;

        int dstW, dstH;
        SDL_GetWindowSize(_window, &dstW, &dstH);

        int scaleX = dstW / srcW;
        int scaleY = dstH / srcH;
        int scale = (scaleX < scaleY) ? scaleX : scaleY;

        if (scale < 1) scale = 1;

        int scaledW = srcW * scale;
        int scaledH = srcH * scale;

        SDL_Rect dstRect;
        dstRect.w = scaledW;
        dstRect.h = scaledH;
        dstRect.x = (dstW - scaledW) / 2;
        dstRect.y = (dstH - scaledH) / 2;

        SDL_BlitSurfaceScaled(_primarySurface->_surface, nullptr, _windowSurface, &dstRect, SDL_SCALEMODE_NEAREST);
    } else {
        SDL_BlitSurface(_primarySurface->_surface, &windowRect, _windowSurface, nullptr);
    }
	
    SDL_UpdateWindowSurface(_window);
    return true;
}

auto DirectDraw::DirectDrawCreate(GUID* lpGUID, IDirectDraw** lplpDD, IUnknown* pUnkOuter) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(lpGUID);
    TRACE_IN_PARAM(lplpDD);
    TRACE_IN_PARAM(pUnkOuter);

    if (!lplpDD) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    directDraw.Initialize(lpGUID);

    *lplpDD = &directDraw;

    TRACE_OUT_PARAM(lplpDD);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(riid);
    TRACE_IN_PARAM(ppvObject);

    if (!ppvObject) {
        TRACE_RETURN(E_POINTER);
    }

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IDirectDraw) {
        *ppvObject = this;
        AddRef();
    } else {
        TRACE_RETURN(E_NOINTERFACE);
    }

    TRACE_OUT_PARAM(ppvObject);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::AddRef() -> ULONG {
    TRACE_FUNCTION_ENTRY("IDirectDraw");
    TRACE_RETURN(++refCount);
}

auto DirectDraw::Release() -> ULONG {
    TRACE_FUNCTION_ENTRY("IDirectDraw");


    if (--refCount == 0) {
        //TODO: Destory all directDraw objects
    }

    TRACE_RETURN(refCount);
}

auto DirectDraw::Compact() -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::CreateClipper(DWORD flags, LPDIRECTDRAWCLIPPER* outClipper, IUnknown*) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(flags);
    TRACE_IN_PARAM(outClipper);

    if (!outClipper) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto hr = DirectDrawClipperImpl::Create(flags, outClipper);

    TRACE_OUT_PARAM(outClipper);
    TRACE_RETURN(hr);
}

auto DirectDraw::CreatePalette(DWORD flags, LPPALETTEENTRY entries, LPDIRECTDRAWPALETTE* outPalette, IUnknown*) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(flags);
    TRACE_IN_PARAM(entries);
    TRACE_IN_PARAM(outPalette);

    if (!outPalette) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto hr = DirectDrawPaletteImpl::Create(flags, entries, outPalette);

    TRACE_OUT_PARAM(outPalette);
    TRACE_RETURN(hr);
}

auto DirectDraw::CreateSurface(LPDDSURFACEDESC lpDDSD, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown*) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(lpDDSD);
    TRACE_IN_PARAM(lplpDDSurface);

    if (!lpDDSD || !lplpDDSurface) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto hr = DirectDrawSurfaceImpl::Create(lpDDSD, lplpDDSurface);

    TRACE_OUT_PARAM(lplpDDSurface);
    TRACE_RETURN(hr);
}

auto DirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSrc, LPDIRECTDRAWSURFACE* lplpDDDest) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(lpDDSrc);
    TRACE_IN_PARAM(lplpDDDest);
    TRACE_OUT_PARAM(lplpDDDest);

    TRACE_RETURN(DDERR_UNSUPPORTED);
}


auto DirectDraw::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(dwFlags);
    TRACE_IN_PARAM(lpDDSurfaceDesc);
    TRACE_IN_PARAM(lpContext);
    TRACE_IN_PARAM(lpEnumModesCallback);

    if (!lpEnumModesCallback) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    struct Mode { DWORD w, h, bpp; };
    static const Mode modes[] = {
        {320, 240, 8},
        {640, 480, 8 },
        {640, 480, 16},
        {800, 600, 8},
        {800, 600, 16},
        {1024, 768, 16},
        {1024, 768, 32},
    };

    for (auto& m : modes) {
        DDSURFACEDESC desc{};
        desc.dwSize = sizeof(DDSURFACEDESC);
        desc.dwWidth = m.w;
        desc.dwHeight = m.h;
        desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
        desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        desc.ddpfPixelFormat.dwFlags = DDPF_RGB;
        desc.ddpfPixelFormat.dwRGBBitCount = m.bpp;
        auto hr = lpEnumModesCallback(&desc, lpContext);
        if (hr != DDENUMRET_OK) break;
    }

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::EnumSurfaces(DWORD flags, LPDDSURFACEDESC lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK cb) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(flags);
    TRACE_IN_PARAM(lpDDSD);
    TRACE_IN_PARAM(lpContext);
    TRACE_IN_PARAM(cb);

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::FlipToGDISurface() -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(lpDDDriverCaps);
    TRACE_IN_PARAM(lpDDHELCaps);

    TRACE_OUT_PARAM(lpDDDriverCaps);
    TRACE_OUT_PARAM(lpDDHELCaps);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetDisplayMode(LPDDSURFACEDESC lpDDSD) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(lpDDSD);

    lpDDSD->dwWidth = _displayWidth;
    lpDDSD->dwHeight = _displayHeight;

    TRACE_OUT_PARAM(lpDDSD);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(lpNumCodes);
    TRACE_IN_PARAM(lpCodes);

    TRACE_OUT_PARAM(lpNumCodes);
    TRACE_OUT_PARAM(lpCodes);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE* surf) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(surf);
    TRACE_OUT_PARAM(surf);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetMonitorFrequency(LPDWORD pFreq) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(pFreq);
    TRACE_OUT_PARAM(pFreq);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetScanLine(LPDWORD pLine) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(pLine);
    TRACE_OUT_PARAM(pLine);
    // Intentionally unsupported; games accept that device has no scanline counter
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDraw::GetVerticalBlankStatus(LPBOOL pbIsInVBlank) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(pbIsInVBlank);

    if (!pbIsInVBlank) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    *pbIsInVBlank = FALSE;

    TRACE_OUT_PARAM(pbIsInVBlank);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::Initialize(GUID* lpGUID) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(lpGUID);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        TRACE_RETURN(DDERR_GENERIC);
    }

    SDL_Rect display;
	SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &display);
	_displayWidth = display.w;
	_displayHeight = display.h;

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::RestoreDisplayMode() -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::SetCooperativeLevel(HWND hwnd, DWORD flags) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(hwnd);
    TRACE_IN_PARAM(flags);

    if (!hwnd && !(flags & DDSCL_NORMAL)) TRACE_RETURN(DDERR_INVALIDPARAMS);
    if (!hwnd && (flags & DDSCL_CREATEDEVICEWINDOW)) TRACE_RETURN(DDERR_INVALIDPARAMS);

    if (_hwnd != hwnd) {
        if (_window) {
            SDL_DestroyWindow(_window);
        }

        _hwnd = hwnd;

        auto props = SDL_CreateProperties();
        SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, _hwnd);

        _window = SDL_CreateWindowWithProperties(props);
        SDL_DestroyProperties(props);

        if (!_window) {
            TRACE_RETURN(DDERR_GENERIC);
        }
    }

    _cooperativeLevel = flags;
    Sleep(100);

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::SetDisplayMode(DWORD w, DWORD h, DWORD bpp) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM(w);
    TRACE_IN_PARAM(h);
    TRACE_IN_PARAM(bpp);

    _displayWidth = w;
    _displayHeight = h;
    _displayDepth = bpp;

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::WaitForVerticalBlank(DWORD flags, HANDLE h) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM(flags);
    TRACE_IN_PARAM(h);

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::_findMainWindow() -> HWND {
    struct EnumData {
        DWORD pid = GetCurrentProcessId();
        HWND hwnd = nullptr;
    } data;

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        auto* data = reinterpret_cast<EnumData*>(lParam);

        DWORD windowPID = 0;
        GetWindowThreadProcessId(hwnd, &windowPID);
        
        if (windowPID != data->pid) return TRUE;

        //if (!IsWindowVisible(hwnd)) return TRUE;

        char className[256]{};
        GetClassNameA(hwnd, className, sizeof(className));

        std::string_view cls{ className };
        if (cls == "ConsoleWindowClass") return TRUE; 

        data->hwnd = hwnd;
        return FALSE;
    }, reinterpret_cast<LPARAM>(&data));

    return data.hwnd;
}

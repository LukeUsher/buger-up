#include "ddraw.hpp"
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
    if ((_cooperativeLevel & DDSCL_EXCLUSIVE) == 0) {
        SDL_SetSurfacePalette(_primarySurface->_surface, gdi32._systemPalette);
    }

	// If no palette was set at all, fall back to the GDI palette
    if (!_primarySurface->_palette) {
        SDL_SetSurfacePalette(_primarySurface->_surface, gdi32._systemPalette);
	}

    _primarySurface->_texture = SDL_CreateTextureFromSurface(_renderer, _primarySurface->_surface);
    if (!_primarySurface->_texture) {
        printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return false;
    }

    if ((_cooperativeLevel & DDSCL_EXCLUSIVE) == 0 && _primarySurface->_palette) {
        SDL_SetSurfacePalette(_primarySurface->_surface, _primarySurface->_palette->_palette);
    }

    int rw, rh;
    SDL_GetCurrentRenderOutputSize(_renderer, &rw, &rh);

    auto sw = _primarySurface->_surface->w;
    auto sh = _primarySurface->_surface->h;

    auto srcAspect = static_cast<float>(sw) / static_cast<float>(sh);
    auto dstAspect = static_cast<float>(rw) / static_cast<float>(rh);

    SDL_FRect dstRect;
    if (srcAspect > dstAspect) {
        dstRect.w = static_cast<float>(rw);
        dstRect.h = static_cast<float>(rw) / srcAspect;
        dstRect.x = 0.0f;
        dstRect.y = (static_cast<float>(rh) - dstRect.h) / 2.0f;
    } else {
        dstRect.h = static_cast<float>(rh);
        dstRect.w = static_cast<float>(rh) * srcAspect;
        dstRect.y = 0.0f;
        dstRect.x = (static_cast<float>(rw) - dstRect.w) / 2.0f;
    }

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    SDL_RenderTexture(_renderer, _primarySurface->_texture, nullptr, &dstRect);
    SDL_RenderPresent(_renderer);
    SDL_DestroyTexture(_primarySurface->_texture);
    return true;
}

auto DirectDraw::DirectDrawCreate(GUID* lpGUID, IDirectDraw** lplpDD, IUnknown* pUnkOuter) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("lpGUID", lpGUID);
    TRACE_IN_PARAM("lplpDD", lplpDD);
    TRACE_IN_PARAM("pUnkOuter", pUnkOuter);

    if (!lplpDD) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    if (!directDraw._window) {
        directDraw.Initialize(lpGUID);
    }

    *lplpDD = &directDraw;

    TRACE_OUT_PARAM("lplpDD", lplpDD);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("riid", &riid);
    TRACE_IN_PARAM("ppvObject", ppvObject);

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

    TRACE_OUT_PARAM("ppvObject", ppvObject);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::AddRef() -> ULONG {
    TRACE_FUNCTION_ENTRY("IDirectDraw");
    TRACE_RETURN(++refCount);
}

auto DirectDraw::Release() -> ULONG {
    TRACE_FUNCTION_ENTRY("IDirectDraw");
    TRACE_RETURN(--refCount);
}

auto DirectDraw::Compact() -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::CreateClipper(DWORD flags, LPDIRECTDRAWCLIPPER* outClipper, IUnknown*) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("outClipper", outClipper);

    if (!outClipper) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto hr = DirectDrawClipperImpl::Create(flags, outClipper);

    TRACE_OUT_PARAM("outClipper", outClipper);
    TRACE_RETURN(hr);
}

auto DirectDraw::CreatePalette(DWORD flags, LPPALETTEENTRY entries, LPDIRECTDRAWPALETTE* outPalette, IUnknown*) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("entries", entries);
    TRACE_IN_PARAM("outPalette", outPalette);

    if (!outPalette) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto hr = DirectDrawPaletteImpl::Create(flags, entries, outPalette);

    TRACE_OUT_PARAM("outPalette", outPalette);
    TRACE_RETURN(hr);
}

auto DirectDraw::CreateSurface(LPDDSURFACEDESC lpDDSD, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown*) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("lpDDSD", lpDDSD);
    TRACE_IN_PARAM("lplpDDSurface", lplpDDSurface);

    if (!lpDDSD || !lplpDDSurface) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto hr = DirectDrawSurfaceImpl::Create(lpDDSD, lplpDDSurface);

    TRACE_OUT_PARAM("lplpDDSurface", lplpDDSurface);
    TRACE_RETURN(hr);
}

auto DirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSrc, LPDIRECTDRAWSURFACE* lplpDDDest) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM("lpDDSrc", lpDDSrc);
    TRACE_IN_PARAM("lplpDDDest", lplpDDDest);
    TRACE_OUT_PARAM("lplpDDDest", lplpDDDest);

    TRACE_RETURN(DDERR_UNSUPPORTED);
}


auto DirectDraw::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("dwFlags", dwFlags);
    TRACE_IN_PARAM("lpDDSurfaceDesc", lpDDSurfaceDesc);
    TRACE_IN_PARAM("lpContext", lpContext);
    TRACE_IN_PARAM("lpEnumModesCallback", lpEnumModesCallback);

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

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("lpDDSD", lpDDSD);
    TRACE_IN_PARAM("lpContext", lpContext);
    TRACE_IN_PARAM("cb", cb);

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::FlipToGDISurface() -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM("lpDDDriverCaps", lpDDDriverCaps);
    TRACE_IN_PARAM("lpDDHELCaps", lpDDHELCaps);

    TRACE_OUT_PARAM("lpDDDriverCaps", lpDDDriverCaps);
    TRACE_OUT_PARAM("lpDDHELCaps", lpDDHELCaps);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetDisplayMode(LPDDSURFACEDESC lpDDSD) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("lpDDSD", lpDDSD);

    lpDDSD->dwWidth = _displayWidth;
    lpDDSD->dwHeight = _displayHeight;

    TRACE_OUT_PARAM("lpDDSD", lpDDSD);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM("lpNumCodes", lpNumCodes);
    TRACE_IN_PARAM("lpCodes", lpCodes);

    TRACE_OUT_PARAM("lpNumCodes", lpNumCodes);
    TRACE_OUT_PARAM("lpCodes", lpCodes);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE* surf) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM("surf", surf);

    TRACE_OUT_PARAM("surf", surf);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetMonitorFrequency(LPDWORD pFreq) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM("pFreq", pFreq);
    TRACE_OUT_PARAM("pFreq", pFreq);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::GetScanLine(LPDWORD pLine) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM("pLine", pLine);
    TRACE_OUT_PARAM("pLine", pLine);
    // Intentionally unsupported; games accept that device has no scanline counter
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDraw::GetVerticalBlankStatus(LPBOOL pbIsInVBlank) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM("pbIsInVBlank", pbIsInVBlank);

    if (!pbIsInVBlank) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    *pbIsInVBlank = FALSE;

    TRACE_OUT_PARAM("pbIsInVBlank", pbIsInVBlank);
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::Initialize(GUID* lpGUID) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("lpGUID", lpGUID);

    if (_window) {
        TRACE_RETURN(DD_OK);
    }

    _hwnd = _findMainWindow();
    if (!_hwnd) {
        TRACE_RETURN(DDERR_GENERIC);
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        TRACE_RETURN(DDERR_GENERIC);
    }

    auto props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, _hwnd);

    _window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    if (!_window) {
        TRACE_RETURN(DDERR_GENERIC);
    }

    _renderer = SDL_CreateRenderer(_window, nullptr);
    if (!_renderer) {
        TRACE_RETURN(DDERR_GENERIC);
    }

    int w, h;
    SDL_GetWindowSize(_window, &w, &h);
    _displayWidth = _windowDisplayWidth = w;
    _displayHeight = _windowDisplayHeight = h;
    _displayDepth = 8;

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::RestoreDisplayMode() -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDraw::SetCooperativeLevel(HWND hwnd, DWORD flags) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("hwnd", hwnd);
    TRACE_IN_PARAM("flags", flags);

    _cooperativeLevel = flags;

    if ((flags & DDSCL_FULLSCREEN) == 0) {
        _displayWidth = _windowDisplayWidth;
        _displayHeight = _windowDisplayHeight;
    }

    Sleep(100);

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::SetDisplayMode(DWORD w, DWORD h, DWORD bpp) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDraw");

    TRACE_IN_PARAM("width", w);
    TRACE_IN_PARAM("height", h);
    TRACE_IN_PARAM("bpp", bpp);

    _displayWidth = w;
    _displayHeight = h;
    _displayDepth = bpp;

    TRACE_RETURN(DD_OK);
}

auto DirectDraw::WaitForVerticalBlank(DWORD flags, HANDLE h) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDraw");

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("handle", h);

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

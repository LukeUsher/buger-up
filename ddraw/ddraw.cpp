#include "ddraw.hpp"
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

    _primarySurface->_texture = SDL_CreateTextureFromSurface(_renderer, _primarySurface->_surface);
    if (!_primarySurface->_texture) return false;

    int rw, rh;
    SDL_GetCurrentRenderOutputSize(_renderer, &rw, &rh);

    int sw = _primarySurface->_surface->w;
    int sh = _primarySurface->_surface->h;

    float srcAspect = static_cast<float>(sw) / static_cast<float>(sh);
    float dstAspect = static_cast<float>(rw) / static_cast<float>(rh);

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
    TRACE_FUNC("ddraw");

    TRACE_IN("lpGUID", lpGUID);
    TRACE_IN("lplpDD", lplpDD);
    TRACE_IN("pUnkOuter", pUnkOuter);

    HRESULT hr = DD_OK;

    if (!lplpDD) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (!directDraw._window) {
        directDraw.Initialize(lpGUID);
    }

    *lplpDD = &directDraw;

    TRACE_OUT("lplpDD", lplpDD);
    TRACE_RET("ddraw", hr);

    return hr;
}

auto DirectDraw::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("riid", &riid);
    TRACE_IN("ppvObject", ppvObject);

    HRESULT hr = S_OK;

    if (!ppvObject) {
        hr = E_POINTER;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IDirectDraw) {
        *ppvObject = this;
        AddRef();
    } else {
        hr = E_NOINTERFACE;
    }

    TRACE_OUT("ppvObject", ppvObject);
    TRACE_RET("ddraw", hr);
    return hr;
}


auto DirectDraw::AddRef() -> ULONG {
    TRACE_FUNC("ddraw");
    TRACE_RET("ddraw", refCount + 1);
    return ++refCount;
}

auto DirectDraw::Release() -> ULONG {
    TRACE_FUNC("ddraw");
    TRACE_RET("ddraw", refCount - 1);
    return --refCount;
}

auto DirectDraw::Compact() -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::CreateClipper(DWORD flags, LPDIRECTDRAWCLIPPER* outClipper, IUnknown*) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("flags", flags);
    TRACE_IN("outClipper", outClipper);

    HRESULT hr = DD_OK;

    if (!outClipper) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    hr = DirectDrawClipperImpl::Create(flags, outClipper);

    TRACE_OUT("outClipper", outClipper);
    TRACE_RET("ddraw", hr);
    return hr;
}


auto DirectDraw::CreatePalette(DWORD flags, LPPALETTEENTRY entries, LPDIRECTDRAWPALETTE* outPalette, IUnknown*) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("flags", flags);
    TRACE_IN("entries", entries);
    TRACE_IN("outPalette", outPalette);

    HRESULT hr = DD_OK;

    if (!outPalette) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    hr = DirectDrawPaletteImpl::Create(flags, entries, outPalette);

    TRACE_OUT("outPalette", outPalette);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::CreateSurface(LPDDSURFACEDESC lpDDSD, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown*) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpDDSD", lpDDSD);
    TRACE_IN("lplpDDSurface", lplpDDSurface);

    HRESULT hr = DD_OK;

    if (!lpDDSD || !lplpDDSurface) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    hr = DirectDrawSurfaceImpl::Create(lpDDSD, lplpDDSurface);

    TRACE_OUT("lplpDDSurface", lplpDDSurface);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSrc, LPDIRECTDRAWSURFACE* lplpDDDest) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpDDSrc", lpDDSrc);
    TRACE_IN("lplpDDDest", lplpDDDest);

    HRESULT hr = DDERR_UNSUPPORTED;

    TRACE_RET("ddraw", hr);
    return hr;
}


auto DirectDraw::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("dwFlags", dwFlags);
    TRACE_IN("lpDDSurfaceDesc", lpDDSurfaceDesc);
    TRACE_IN("lpContext", lpContext);
    TRACE_IN("lpEnumModesCallback", lpEnumModesCallback);

    HRESULT hr = DD_OK;

    if (!lpEnumModesCallback) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
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
        hr = lpEnumModesCallback(&desc, lpContext);
        if (hr != DDENUMRET_OK) break;
    }

    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::EnumSurfaces(DWORD flags, LPDDSURFACEDESC lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK cb) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("flags", flags);
    TRACE_IN("lpDDSD", lpDDSD);
    TRACE_IN("lpContext", lpContext);
    TRACE_IN("cb", cb);

    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::FlipToGDISurface() -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}


auto DirectDraw::GetCaps(LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpDDDriverCaps", lpDDDriverCaps);
    TRACE_IN("lpDDHELCaps", lpDDHELCaps);

    HRESULT hr = DD_OK;

    TRACE_OUT("lpDDDriverCaps", lpDDDriverCaps);
    TRACE_OUT("lpDDHELCaps", lpDDHELCaps);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::GetDisplayMode(LPDDSURFACEDESC lpDDSD) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpDDSD", lpDDSD);

    HRESULT hr = DD_OK;

    TRACE_OUT("lpDDSD", lpDDSD);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::GetFourCCCodes(LPDWORD lpNumCodes, LPDWORD lpCodes) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpNumCodes", lpNumCodes);
    TRACE_IN("lpCodes", lpCodes);

    HRESULT hr = DD_OK;

    TRACE_OUT("lpNumCodes", lpNumCodes);
    TRACE_OUT("lpCodes", lpCodes);
    TRACE_RET("ddraw", hr);
    return hr;
}


auto DirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE* surf) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("surf", surf);

    HRESULT hr = DD_OK;

    TRACE_OUT("surf", surf);
    TRACE_RET("ddraw", hr);
    return hr;
}


auto DirectDraw::GetMonitorFrequency(LPDWORD pFreq) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("pFreq", pFreq);

    HRESULT hr = DD_OK;

    TRACE_OUT("pFreq", pFreq);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::GetScanLine(LPDWORD pLine) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("pLine", pLine);

    HRESULT hr = DD_OK;

    TRACE_OUT("pLine", pLine);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::GetVerticalBlankStatus(LPBOOL pbIsInVBlank) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("pbIsInVBlank", pbIsInVBlank);

    HRESULT hr = DD_OK;

    if (!pbIsInVBlank) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    *pbIsInVBlank = FALSE;

    TRACE_OUT("pbIsInVBlank", pbIsInVBlank);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::Initialize(GUID* lpGUID) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpGUID", lpGUID);

    HRESULT hr = DD_OK;

    if (_window) {
        TRACE_RET("ddraw", hr);
        return hr;
    }

    _hwnd = _findMainWindow();
    if (!_hwnd) {
        hr = -1;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        hr = -1;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, _hwnd);

    _window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    if (!_window) {
        hr = -1;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    _renderer = SDL_CreateRenderer(_window, nullptr);
    if (!_renderer) {
        hr = -1;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    int w, h;
    SDL_GetWindowSize(_window, &w, &h);
    _displayWidth = _windowDisplayWidth = w;
    _displayHeight = _windowDisplayHeight = h;
    _displayDepth = 8;

    TRACE_RET("ddraw", hr);
    return hr;
}


auto DirectDraw::RestoreDisplayMode() -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::SetCooperativeLevel(HWND hwnd, DWORD flags) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("hwnd", hwnd);
    TRACE_IN("flags", flags);

    _cooperativeLevel = flags;

    if ((flags & DDSCL_FULLSCREEN) == 0) {
        _displayWidth = _windowDisplayWidth;
        _displayHeight = _windowDisplayHeight;
    }

    Sleep(100);

    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}


auto DirectDraw::SetDisplayMode(DWORD w, DWORD h, DWORD bpp) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("width", w);
    TRACE_IN("height", h);
    TRACE_IN("bpp", bpp);

    _displayWidth = w;
    _displayHeight = h;
    _displayDepth = bpp;

    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDraw::WaitForVerticalBlank(DWORD flags, HANDLE h) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("flags", flags);
    TRACE_IN("handle", h);

    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
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

        if (!IsWindowVisible(hwnd)) return TRUE; 

        char className[256]{};
        GetClassNameA(hwnd, className, sizeof(className));

        std::string_view cls{ className };
        if (cls == "ConsoleWindowClass") return TRUE; 

        data->hwnd = hwnd;
        return FALSE;
    }, reinterpret_cast<LPARAM>(&data));

    return data.hwnd;
}

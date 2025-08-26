#include "ddraw.hpp"


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
    if (!lplpDD) return DDERR_INVALIDPARAMS;
    if (!directDraw._window) directDraw.Initialize(lpGUID);
    *lplpDD = &directDraw;
    return DD_OK;    
}

auto DirectDraw::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    if (!ppvObject) return E_POINTER;
    *ppvObject = nullptr;
    if (riid == IID_IUnknown || riid == IID_IDirectDraw) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

auto DirectDraw::AddRef() -> ULONG {
    return ++refCount;
}

auto DirectDraw::Release() -> ULONG {
    ULONG count = --refCount;
    //if (count == 0) delete this; We are a global object/non-deletable.
    return count;
}

auto DirectDraw::Compact() -> HRESULT {
    std::cout << "[DirectDraw1Impl] Compact ignored\n";
    return DD_OK;
}

auto DirectDraw::CreateClipper(DWORD flags, LPDIRECTDRAWCLIPPER* outClipper, IUnknown*) -> HRESULT {
    std::cout << "[DirectDraw1Impl] CreateClipper called\n";
    return DirectDrawClipperImpl::Create(flags, outClipper);
}

auto DirectDraw::CreatePalette(DWORD flags, LPPALETTEENTRY entries, LPDIRECTDRAWPALETTE* outPalette, IUnknown*) -> HRESULT {
    if (!outPalette) return DDERR_INVALIDPARAMS;
    return DirectDrawPaletteImpl::Create(flags, entries, outPalette);
}

auto DirectDraw::CreateSurface(LPDDSURFACEDESC lpDDSD, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown*) -> HRESULT {
    if (!lpDDSD || !lplpDDSurface) return DDERR_INVALIDPARAMS;
    return DirectDrawSurfaceImpl::Create(lpDDSD, lplpDDSurface);
}

auto DirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSrc, LPDIRECTDRAWSURFACE* lplpDDDest) -> HRESULT {
    std::cout << "[DirectDraw1Impl] DuplicateSurface ignored\n";
    return DDERR_UNSUPPORTED;
}

auto DirectDraw::EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback) -> HRESULT {
    if (!lpEnumModesCallback) return DDERR_INVALIDPARAMS;

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
        HRESULT hr = lpEnumModesCallback(&desc, lpContext);
        if (hr != DDENUMRET_OK) break;
    }

    return DD_OK;
}

auto DirectDraw::EnumSurfaces(DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMSURFACESCALLBACK) -> HRESULT {
    std::cout << "[DirectDraw1Impl] EnumSurfaces ignored\n";
    return DD_OK;
}

auto DirectDraw::FlipToGDISurface() -> HRESULT {
    std::cout << "[DirectDraw1Impl] FlipToGDISurface ignored\n";
    return DD_OK;
}

auto DirectDraw::GetCaps(LPDDCAPS, LPDDCAPS) -> HRESULT {
    std::cout << "[DirectDraw1Impl] GetCaps ignored\n";
    return DD_OK;
}

auto DirectDraw::GetDisplayMode(LPDDSURFACEDESC) -> HRESULT {
    std::cout << "[DirectDraw1Impl] GetDisplayMode ignored\n";
    return DD_OK;
}

auto DirectDraw::GetFourCCCodes(LPDWORD, LPDWORD) -> HRESULT {
    std::cout << "[DirectDraw1Impl] GetFourCCCodes ignored\n";
    return DD_OK;
}

auto DirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE*) -> HRESULT {
    std::cout << "[DirectDraw1Impl] GetGDISurface ignored\n";
    return DD_OK;
}

auto DirectDraw::GetMonitorFrequency(LPDWORD) -> HRESULT {
    std::cout << "[DirectDraw1Impl] GetMonitorFrequency ignored\n";
    return DD_OK;
}

auto DirectDraw::GetScanLine(LPDWORD) -> HRESULT {
    std::cout << "[DirectDraw1Impl] GetScanLine ignored\n";
    return DD_OK;
}

auto DirectDraw::GetVerticalBlankStatus(LPBOOL pbIsInVBlank) -> HRESULT {
    if (!pbIsInVBlank) return DDERR_INVALIDPARAMS;

    *pbIsInVBlank = FALSE;
    std::cout << "[DirectDraw1Impl] GetVerticalBlankStatus ignored\n";
    return DD_OK;
}

auto DirectDraw::Initialize(GUID*) -> HRESULT {
    if (_window) return DD_OK;

    _hwnd = _findMainWindow();
    if (!_hwnd)  return -1;
    if (!SDL_Init(SDL_INIT_VIDEO)) return -1;

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetPointerProperty(props, SDL_PROP_WINDOW_CREATE_WIN32_HWND_POINTER, _hwnd);
    _window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    if (!_window) return -1;
    _renderer = SDL_CreateRenderer(_window, nullptr);
    if (!_renderer) return -1;

    int w, h;
    SDL_GetWindowSize(_window, &w, &h);
    _displayWidth = _windowDisplayWidth = w;
    _displayHeight = _windowDisplayHeight = h;
    _displayDepth = 8; // default to 8bpp unless otherwise specified

    return DD_OK;
}

auto DirectDraw::RestoreDisplayMode() -> HRESULT {
    std::cout << "[DirectDraw1Impl] RestoreDisplayMode ignored\n";
    return DD_OK;
}

auto DirectDraw::SetCooperativeLevel(HWND hwnd, DWORD dwFlags) -> HRESULT {
    _cooperativeLevel = dwFlags;
    // If we switched to windowed mode, undo SetDisplayMode
    if ((directDraw._cooperativeLevel & DDSCL_FULLSCREEN) == 0) {
        directDraw._displayWidth = directDraw._windowDisplayWidth;
        directDraw._displayHeight = directDraw._windowDisplayHeight;
    }
    //NOTE: We defer actual mode switching until the primary surface is created; Bug! will crash within WM_SIZE otherwise
    Sleep(100); // Required because games break if this returns too quickly
    return DD_OK;
}

auto DirectDraw::SetDisplayMode(DWORD dwWidth, DWORD dwHeight, DWORD dwBpp) -> HRESULT {
    _displayWidth = dwWidth;
    _displayHeight = dwHeight;
    _displayDepth = dwBpp;
    return DD_OK;
}

auto DirectDraw::WaitForVerticalBlank(DWORD, HANDLE) -> HRESULT {
    std::cout << "[DirectDraw1Impl] WaitForVerticalBlank ignored\n";
    return DD_OK;
}

auto DirectDraw::_findMainWindow() -> HWND
{
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

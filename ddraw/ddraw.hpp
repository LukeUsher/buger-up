#pragma once
#include <windows.h>
#include <ddraw.h>
#include "surface.hpp"
#include "palette.hpp"
#include "clipper.hpp"
#include <iostream>
#include <mutex>
#include <SDL3/SDL.h>

struct DirectDraw : public IDirectDraw {
    static auto applyPatches() -> void;
    auto flipPrimary() -> bool;
    static auto __stdcall DirectDrawCreate(GUID* lpGUID, IDirectDraw** lplpDD, IUnknown* pUnkOuter) -> HRESULT;

    std::atomic<LONG> refCount = 1;

    auto __stdcall QueryInterface(REFIID riid, void** ppvObject)->HRESULT override;
    auto __stdcall AddRef()->ULONG override;
    auto __stdcall Release()->ULONG override;
    auto __stdcall Compact()->HRESULT override;
    auto __stdcall CreateClipper(DWORD flags, LPDIRECTDRAWCLIPPER* outClipper, IUnknown*)->HRESULT;
    auto __stdcall CreatePalette(DWORD flags, LPPALETTEENTRY entries, LPDIRECTDRAWPALETTE* outPalette, IUnknown*)->HRESULT override;
    auto __stdcall CreateSurface(LPDDSURFACEDESC lpDDSD, LPDIRECTDRAWSURFACE* lplpDDSurface, IUnknown*)->HRESULT;
    auto __stdcall DuplicateSurface(LPDIRECTDRAWSURFACE lpDDSrc, LPDIRECTDRAWSURFACE* lplpDDDest)->HRESULT override;
    auto __stdcall EnumDisplayModes(DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback)->HRESULT override;
    auto __stdcall EnumSurfaces(DWORD, LPDDSURFACEDESC, LPVOID, LPDDENUMSURFACESCALLBACK)->HRESULT;
    auto __stdcall FlipToGDISurface()->HRESULT;
    auto __stdcall GetCaps(LPDDCAPS, LPDDCAPS)->HRESULT;
    auto __stdcall GetDisplayMode(LPDDSURFACEDESC)->HRESULT;
    auto __stdcall GetFourCCCodes(LPDWORD, LPDWORD)->HRESULT;
    auto __stdcall GetGDISurface(LPDIRECTDRAWSURFACE*)->HRESULT;
    auto __stdcall GetMonitorFrequency(LPDWORD)->HRESULT override;
    auto __stdcall GetScanLine(LPDWORD)->HRESULT override;
    auto __stdcall GetVerticalBlankStatus(LPBOOL pbIsInVBlank)->HRESULT override;
    auto __stdcall Initialize(GUID*)->HRESULT override;
    auto __stdcall RestoreDisplayMode()->HRESULT override;
    auto __stdcall SetCooperativeLevel(HWND hwnd, DWORD)->HRESULT override;
    auto __stdcall SetDisplayMode(DWORD, DWORD, DWORD)->HRESULT;
    auto __stdcall WaitForVerticalBlank(DWORD, HANDLE)->HRESULT override;

    HWND _hwnd;
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    DirectDrawSurfaceImpl* _primarySurface = nullptr;

    uint32_t _displayWidth;
    uint32_t _displayHeight;
    uint32_t _displayDepth;

    uint32_t _windowDisplayWidth;
    uint32_t _windowDisplayHeight;
    uint32_t _cooperativeLevel;

    auto _findMainWindow() -> HWND;
};

extern DirectDraw directDraw;


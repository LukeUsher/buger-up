#pragma once
#include <Windows.h>
#include <ddraw.h>
#include "palette.hpp"
#include <iostream>
#include <atomic>
#include <vector>
#include <SDL3/SDL.h>

struct DirectDrawSurfaceImpl : public IDirectDrawSurface {
    std::atomic<LONG> refCount{ 1 };

    DDSURFACEDESC desc{};
    bool isPrimary = false;
    SDL_Surface* _surface = nullptr;
    SDL_Texture* _texture = nullptr;
    DirectDrawPaletteImpl* _palette = nullptr;
    DirectDrawSurfaceImpl* _backbuffer = nullptr;


    ~DirectDrawSurfaceImpl();

    static auto Create(LPDDSURFACEDESC desc, IDirectDrawSurface** outSurface) -> HRESULT;

    auto __stdcall QueryInterface(REFIID riid, void** ppvObject)->HRESULT override;
    auto __stdcall AddRef()->ULONG override;
    auto __stdcall Release()->ULONG override;

    auto __stdcall AddAttachedSurface(LPDIRECTDRAWSURFACE)->HRESULT override;
    auto __stdcall AddOverlayDirtyRect(LPRECT)->HRESULT override;
    auto __stdcall Blt(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDBLTFX)->HRESULT override;
    auto __stdcall BltBatch(LPDDBLTBATCH, DWORD, DWORD)->HRESULT override;
    auto __stdcall BltFast(DWORD, DWORD, LPDIRECTDRAWSURFACE, LPRECT, DWORD)->HRESULT override;
    auto __stdcall DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE)->HRESULT override;
    auto __stdcall EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK)->HRESULT override;
    auto __stdcall EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK)->HRESULT override;
    auto __stdcall Flip(LPDIRECTDRAWSURFACE, DWORD)->HRESULT override;
    auto __stdcall GetAttachedSurface(LPDDSCAPS, LPDIRECTDRAWSURFACE*)->HRESULT override;
    auto __stdcall GetBltStatus(DWORD)->HRESULT override;
    auto __stdcall GetCaps(LPDDSCAPS)->HRESULT override;
    auto __stdcall GetClipper(LPDIRECTDRAWCLIPPER*)->HRESULT override;
    auto __stdcall GetColorKey(DWORD, LPDDCOLORKEY)->HRESULT override;
    auto __stdcall GetDC(HDC*)->HRESULT override;
    auto __stdcall GetFlipStatus(DWORD)->HRESULT override;
    auto __stdcall GetOverlayPosition(LPLONG, LPLONG)->HRESULT override;
    auto __stdcall GetPalette(LPDIRECTDRAWPALETTE*)->HRESULT override;
    auto __stdcall GetPixelFormat(LPDDPIXELFORMAT)->HRESULT override;
    auto __stdcall GetSurfaceDesc(LPDDSURFACEDESC)->HRESULT override;
    auto __stdcall Initialize(LPDIRECTDRAW, LPDDSURFACEDESC)->HRESULT override;
    auto __stdcall IsLost()->HRESULT override;
    auto __stdcall Lock(LPRECT, LPDDSURFACEDESC, DWORD, HANDLE)->HRESULT override;
    auto __stdcall ReleaseDC(HDC)->HRESULT override;
    auto __stdcall Restore()->HRESULT override;
    auto __stdcall SetClipper(LPDIRECTDRAWCLIPPER)->HRESULT override;
    auto __stdcall SetColorKey(DWORD, LPDDCOLORKEY)->HRESULT override;
    auto __stdcall SetOverlayPosition(LONG, LONG)->HRESULT override;
    auto __stdcall SetPalette(LPDIRECTDRAWPALETTE)->HRESULT override;
    auto __stdcall Unlock(LPVOID)->HRESULT override;
    auto __stdcall UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX)->HRESULT override;
    auto __stdcall UpdateOverlayDisplay(DWORD)->HRESULT override;
    auto __stdcall UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE)->HRESULT override;
};

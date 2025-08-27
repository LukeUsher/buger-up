#pragma once

#include <Windows.h>
#include <ddraw.h>
#include <iostream>
#include <atomic>
#include <SDL3/SDL.h>

struct DirectDrawPaletteImpl : public IDirectDrawPalette {
    std::atomic<LONG> refCount{ 1 };
    DWORD caps;
    SDL_Palette* _palette;

    static auto Create(DWORD flags, LPPALETTEENTRY entries, IDirectDrawPalette** outPalette) -> HRESULT;

    auto __stdcall QueryInterface(REFIID riid, void** ppvObject)->HRESULT override;
    auto __stdcall AddRef()->ULONG override;
    auto __stdcall Release()->ULONG override;

    auto __stdcall GetCaps(LPDWORD)->HRESULT override;
    auto __stdcall GetEntries(DWORD, DWORD, DWORD, LPPALETTEENTRY)->HRESULT override;
    auto __stdcall Initialize(LPDIRECTDRAW, DWORD, LPPALETTEENTRY)->HRESULT override;
    auto __stdcall SetEntries(DWORD, DWORD, DWORD, LPPALETTEENTRY)->HRESULT override;
};
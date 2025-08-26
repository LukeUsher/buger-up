#pragma once
#include <Windows.h>
#include <ddraw.h>
#include <iostream>
#include <atomic>

struct DirectDrawClipperImpl : public IDirectDrawClipper {
    std::atomic<LONG> refCount{ 1 };
    DWORD caps;

    static auto Create(DWORD flags, IDirectDrawClipper** outClipper) -> HRESULT;

    auto __stdcall QueryInterface(REFIID riid, void** ppvObject)->HRESULT override;
    auto __stdcall AddRef()->ULONG override;
    auto __stdcall Release()->ULONG override;

    auto __stdcall GetClipList(LPRECT, LPRGNDATA, LPDWORD)->HRESULT override;
    auto __stdcall GetHWnd(HWND*)->HRESULT override;
    auto __stdcall Initialize(LPDIRECTDRAW, DWORD)->HRESULT override;
    auto __stdcall IsClipListChanged(BOOL*)->HRESULT override;
    auto __stdcall SetClipList(LPRGNDATA, DWORD)->HRESULT override;
    auto __stdcall SetHWnd(DWORD, HWND)->HRESULT override;
};

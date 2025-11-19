#pragma once

#include <Windows.h>
#include <SDL3/SDL.h>
#include <unordered_map>

struct GDI32 {
    GDI32();
    ~GDI32();

    static auto applyPatches() -> void;

    static auto __stdcall CreatePalette(const LOGPALETTE* lpLogPal) -> HPALETTE;
    static auto __stdcall DeleteObject(HGDIOBJ hobj) -> BOOL;
    static auto __stdcall SelectPalette(HDC hdc, HPALETTE hpal, BOOL bForceBackground) -> HPALETTE;
    static auto __stdcall RealizePalette(HDC hdc) -> UINT;
    static auto __stdcall AnimatePalette(HPALETTE hpal, UINT start, UINT count, const PALETTEENTRY* entries) -> BOOL;

    static auto mergePaletteIntoSystem(HPALETTE hpal) -> void;
    static auto isStaticSystemColor(int index) -> bool;

    SDL_Palette* _systemPalette;

    struct LogicalPalette {
        PALETTEENTRY entries[256];
        UINT count;
    };

    struct PaletteMapping {
        uint8_t logicalToSystem[256];
    };

    static inline std::unordered_map<HPALETTE, LogicalPalette>     _logicalPalettes;
    static inline std::unordered_map<HPALETTE, PaletteMapping>     _paletteTranslation;
    static inline std::unordered_map<HDC, HPALETTE>                _selectedPaletteForDC;

    static inline HPALETTE _currentlyRealizedPalette = nullptr;

};

extern GDI32 gdi32;

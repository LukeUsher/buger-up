#include "gdi32.hpp"
#include "../ddraw/ddraw.hpp"
#include "../logger.hpp"
#include "../patch-engine.h"

GDI32 gdi32;

using CreatePalette_t = HPALETTE(WINAPI*)(const LOGPALETTE*);
using DeleteObject_t = BOOL(WINAPI*)(HGDIOBJ);
using AnimatePalette_t = BOOL(WINAPI*)(HPALETTE, UINT, UINT, const PALETTEENTRY*);
using SelectPalette_t = HPALETTE(WINAPI*)(HDC, HPALETTE, BOOL);
using RealizePalette_t = UINT(WINAPI*)(HDC);

static inline CreatePalette_t   _CreatePalette = nullptr;
static inline DeleteObject_t    _DeleteObject = nullptr;
static inline AnimatePalette_t  _AnimatePalette = nullptr;
static inline SelectPalette_t   _SelectPalette = nullptr;
static inline RealizePalette_t  _RealizePalette = nullptr;

static const int StaticSystemColors[20] = {
    0,1,2,3,4,5,6,7,8,9,
    246,247,248,249,250,251,252,253,254,255
};

auto GDI32::isStaticSystemColor(int index) -> bool {
    for (auto i = 0; i < 20; i++) {
        if (StaticSystemColors[i] == index) return true;
    }

    return false;
}

GDI32::GDI32() {
    _systemPalette = SDL_CreatePalette(256);
    for (auto i = 0; i < 256; i++) {
        _systemPalette->colors[i].r = 0;
        _systemPalette->colors[i].g = 0;
        _systemPalette->colors[i].b = 0;
        _systemPalette->colors[i].a = 255;
    }
}

GDI32::~GDI32() {
    if (_systemPalette) {
        SDL_DestroyPalette(_systemPalette);
        gdi32._systemPalette = nullptr;
    }
}

auto GDI32::applyPatches() -> void {
    patchEngine.PatchImportedFunction("gdi32", "CreatePalette", CreatePalette, (void**)&_CreatePalette);
    patchEngine.PatchImportedFunction("gdi32", "AnimatePalette", AnimatePalette, (void**)&_AnimatePalette);
    patchEngine.PatchImportedFunction("gdi32", "SelectPalette", SelectPalette, (void**)&_SelectPalette);
    patchEngine.PatchImportedFunction("gdi32", "RealizePalette", RealizePalette, (void**)&_RealizePalette);
    patchEngine.PatchImportedFunction("gdi32", "DeleteObject", DeleteObject, (void**)&_DeleteObject);
}

auto __stdcall GDI32::CreatePalette(const LOGPALETTE* lpLogPal) -> HPALETTE {
    TRACE_FUNCTION_ENTRY("gdi32");
    TRACE_IN_PARAM("lpLogPal", lpLogPal);

    if (!lpLogPal) TRACE_RETURN(nullptr);

    auto hpal = _CreatePalette(lpLogPal);

    LogicalPalette pal{};
    pal.count = lpLogPal->palNumEntries;

    for (auto i = 0u; i < pal.count; i++) {
        pal.entries[i] = lpLogPal->palPalEntry[i];
    }

    _logicalPalettes[hpal] = pal;
    TRACE_RETURN(hpal);
}

auto __stdcall GDI32::DeleteObject(HGDIOBJ hobj) -> BOOL {
    TRACE_FUNCTION_ENTRY("gdi32");
    TRACE_IN_PARAM("hobj", hobj);
    auto hpal = (HPALETTE)hobj;
    _logicalPalettes.erase(hpal);
    _paletteTranslation.erase(hpal);
    TRACE_RETURN(_DeleteObject(hobj));
}

auto __stdcall GDI32::SelectPalette(HDC hdc, HPALETTE hpal, BOOL forceBackground) -> HPALETTE {
    TRACE_FUNCTION_ENTRY("gdi32");
    TRACE_IN_PARAM("hdc", hdc);
    TRACE_IN_PARAM("hpal", hpal);
    TRACE_IN_PARAM("forceBackground", forceBackground);
    _selectedPaletteForDC[hdc] = hpal;
    TRACE_RETURN(_SelectPalette(hdc, hpal, forceBackground));
}

auto GDI32::mergePaletteIntoSystem(HPALETTE hpal) -> void {
    if (!_logicalPalettes.contains(hpal)) return;

    auto& src = _logicalPalettes[hpal];

    if (directDraw._primarySurface && directDraw._primarySurface->_palette) {
        for (auto i = 0u; i < src.count; i++) {
            auto& pe = src.entries[i];
            gdi32._systemPalette->colors[i].r = pe.peRed;
            gdi32._systemPalette->colors[i].g = pe.peGreen;
            gdi32._systemPalette->colors[i].b = pe.peBlue;
            gdi32._systemPalette->colors[i].a = 255;
        }

        SDL_SetPaletteColors(gdi32._systemPalette, gdi32._systemPalette->colors, 0, 256);
        return;
    }

    auto& map = _paletteTranslation[hpal];

    for (int i = 0; i < 256; i++) map.logicalToSystem[i] = 0;

    int sysIndex = 0;
    for (auto i = 0u; i < src.count; i++) {
        auto& pe = src.entries[i];
        auto flags = pe.peFlags;

        if (flags & PC_EXPLICIT) {
            auto explicitIndex = pe.peRed;
            map.logicalToSystem[i] = explicitIndex;
            continue;
        }

        while (sysIndex < 256 && isStaticSystemColor(sysIndex)) sysIndex++;
        if (sysIndex >= 256) break;

        if (flags & PC_RESERVED) {
            map.logicalToSystem[i] = sysIndex;
            gdi32._systemPalette->colors[sysIndex].r = pe.peRed;
            gdi32._systemPalette->colors[sysIndex].g = pe.peGreen;
            gdi32._systemPalette->colors[sysIndex].b = pe.peBlue;
            gdi32._systemPalette->colors[sysIndex].a = 255;
            sysIndex++;
            continue;
        }

        if (flags & PC_NOCOLLAPSE) {
            map.logicalToSystem[i] = sysIndex;
            gdi32._systemPalette->colors[sysIndex].r = pe.peRed;
            gdi32._systemPalette->colors[sysIndex].g = pe.peGreen;
            gdi32._systemPalette->colors[sysIndex].b = pe.peBlue;
            gdi32._systemPalette->colors[sysIndex].a = 255;
            sysIndex++;
            continue;
        }

        int match = -1;

        for (auto si = 0; si < sysIndex; si++) {
            if (isStaticSystemColor(si)) continue;

            auto& c = gdi32._systemPalette->colors[si];
            if (c.r == pe.peRed && c.g == pe.peGreen && c.b == pe.peBlue) {
                match = si;
                break;
            }
        }

        if (match >= 0) {
            map.logicalToSystem[i] = match;
            continue;
        }

        map.logicalToSystem[i] = sysIndex;

        gdi32._systemPalette->colors[sysIndex].r = pe.peRed;
        gdi32._systemPalette->colors[sysIndex].g = pe.peGreen;
        gdi32._systemPalette->colors[sysIndex].b = pe.peBlue;
        gdi32._systemPalette->colors[sysIndex].a = 255;

        sysIndex++;
    }

    SDL_SetPaletteColors(gdi32._systemPalette, gdi32._systemPalette->colors, 0, 256);
}

auto __stdcall GDI32::RealizePalette(HDC hdc) -> UINT {
    TRACE_FUNCTION_ENTRY("gdi32");
    TRACE_IN_PARAM("hdc", hdc);

    auto changed = _RealizePalette(hdc);

    HPALETTE hpal = nullptr;
    if (_selectedPaletteForDC.contains(hdc)) hpal = _selectedPaletteForDC[hdc];
    if (!hpal) return 0;

    mergePaletteIntoSystem(hpal);

    TRACE_RETURN(changed);
}

auto __stdcall GDI32::AnimatePalette(HPALETTE hpal, UINT start, UINT count, const PALETTEENTRY* ppe) -> BOOL {
    auto res = _AnimatePalette(hpal, start, count, ppe);

    if (_logicalPalettes.contains(hpal)) {
        auto& pal = _logicalPalettes[hpal];
        for (auto i = 0u; i < count; i++) pal.entries[start + i] = ppe[i];
        if (hpal == _currentlyRealizedPalette) mergePaletteIntoSystem(hpal);
    }

    TRACE_RETURN(res);
}

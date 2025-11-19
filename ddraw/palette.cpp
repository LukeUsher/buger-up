#include "palette.hpp"
#include "ddraw.hpp"
#include "../logger.hpp"

auto DirectDrawPaletteImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM("riid", &riid);
    TRACE_IN_PARAM("ppvObject", ppvObject);

    if (!ppvObject) {
        TRACE_RETURN(E_POINTER);
    }

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IDirectDrawPalette) {
        *ppvObject = this;
        AddRef();
    } else {
        TRACE_RETURN(E_NOINTERFACE);
    }

    TRACE_OUT_PARAM("ppvObject", ppvObject);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawPaletteImpl::AddRef() -> ULONG {
    TRACE_FUNCTION_ENTRY("ddraw");
    TRACE_RETURN(++refCount);
}

auto DirectDrawPaletteImpl::Release() -> ULONG {
    TRACE_FUNCTION_ENTRY("ddraw");

    refCount--;

    if (refCount == 0) {
        if (_palette) SDL_DestroyPalette(_palette);
        delete this;
    }

    TRACE_RETURN(refCount);
}

auto DirectDrawPaletteImpl::Create(DWORD flags, LPPALETTEENTRY entries, IDirectDrawPalette** outPalette) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("entries", entries);
    TRACE_IN_PARAM("outPalette", outPalette);

    if (!outPalette) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto p = new (std::nothrow) DirectDrawPaletteImpl();
    if (!p) {
        TRACE_RETURN(DDERR_OUTOFMEMORY);
    }

    if ((flags & DDPCAPS_8BIT) != DDPCAPS_8BIT) {
        delete p;
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    p->caps = flags;

    p->_palette = SDL_CreatePalette(256);
    if (!p->_palette) {
        delete p;
        TRACE_RETURN(DDERR_OUTOFMEMORY);
    }

    if (entries) p->SetEntries(0, 0, 256, entries);

    *outPalette = p;

    TRACE_OUT_PARAM("outPalette", outPalette);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawPaletteImpl::GetCaps(LPDWORD outFlags) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM("outFlags", outFlags);

    if (!outFlags) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    *outFlags = caps;

    TRACE_OUT_PARAM("outFlags", outFlags);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawPaletteImpl::GetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY outEntries) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("start", start);
    TRACE_IN_PARAM("count", count);
    TRACE_IN_PARAM("outEntries", outEntries);

    if (!outEntries || !_palette) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    if (start >= static_cast<DWORD>(_palette->ncolors)) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    count = std::min<DWORD>(count, _palette->ncolors - start);

    for (auto i = 0u; i < count; i++) {
        auto& c = _palette->colors[start + i];
        outEntries[i].peRed = c.r;
        outEntries[i].peGreen = c.g;
        outEntries[i].peBlue = c.b;
        outEntries[i].peFlags = 0;
    }

    TRACE_OUT_PARAM("outEntries", outEntries);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawPaletteImpl::Initialize(LPDIRECTDRAW lpDD, DWORD flags, LPPALETTEENTRY inEntries) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM("lpDD", lpDD);
    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("inEntries", inEntries);

    caps = flags;

    TRACE_RETURN(DD_OK);
}

auto DirectDrawPaletteImpl::SetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY inEntries) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("start", start);
    TRACE_IN_PARAM("count", count);
    TRACE_IN_PARAM("inEntries", inEntries);

    if (!inEntries || !_palette) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    if (start >= static_cast<DWORD>(_palette->ncolors)) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    count = std::min<DWORD>(count, _palette->ncolors - start);

    for (DWORD i = 0; i < count; i++) {
        SDL_Color c;
        c.r = inEntries[i].peRed;
        c.g = inEntries[i].peGreen;
        c.b = inEntries[i].peBlue;
        c.a = 255;
        _palette->colors[start + i] = c;
    }

    if (!SDL_SetPaletteColors(_palette, _palette->colors + start, start, count)) {
        std::cerr << "SDL_SetPaletteColors failed: " << SDL_GetError() << "\n";
        TRACE_RETURN(DDERR_GENERIC);
    }

    TRACE_RETURN(DD_OK);
}

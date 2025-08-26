#include "palette.hpp"

auto DirectDrawPaletteImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    if (!ppvObject) return E_POINTER;
    *ppvObject = nullptr;
    if (riid == IID_IUnknown || riid == IID_IDirectDrawPalette) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

auto DirectDrawPaletteImpl::AddRef() -> ULONG {
    return ++refCount;
}

auto DirectDrawPaletteImpl::Release() -> ULONG {
    ULONG count = --refCount;
    if (count == 0) {
        if (_palette) SDL_DestroyPalette(_palette);
        delete this;
    }
    return count;
}

auto DirectDrawPaletteImpl::Create(DWORD flags, LPPALETTEENTRY entries, IDirectDrawPalette** outPalette) -> HRESULT {
    if (!outPalette) return DDERR_INVALIDPARAMS;

    DirectDrawPaletteImpl* p = new (std::nothrow) DirectDrawPaletteImpl();
    if (!p) return DDERR_OUTOFMEMORY;

    if ((flags & DDPCAPS_8BIT) != DDPCAPS_8BIT) return DDERR_INVALIDPARAMS;

    p->caps = flags;

    p->_palette = SDL_CreatePalette(256);
    if (!p->_palette) {
        delete p;
        return DDERR_OUTOFMEMORY;
    }
    if (entries) p->SetEntries(0, 0, 256, entries);

    *outPalette = p;
    return DD_OK;
}

auto DirectDrawPaletteImpl::GetCaps(LPDWORD outFlags) -> HRESULT {
    if (!outFlags) return DDERR_INVALIDPARAMS;
    *outFlags = caps;
    return DD_OK;
}

auto DirectDrawPaletteImpl::GetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY outEntries) -> HRESULT {
    if (!outEntries || !_palette) return DDERR_INVALIDPARAMS;
    if (start >= _palette->ncolors) return DDERR_INVALIDPARAMS;

    count = std::min<DWORD>(count, _palette->ncolors - start);

    for (DWORD i = 0; i < count; i++) {
        const SDL_Color& c = _palette->colors[start + i];
        outEntries[i].peRed = c.r;
        outEntries[i].peGreen = c.g;
        outEntries[i].peBlue = c.b;
        outEntries[i].peFlags = 0;
    }

    return DD_OK;
}

auto DirectDrawPaletteImpl::Initialize(LPDIRECTDRAW, DWORD flags, LPPALETTEENTRY inEntries) -> HRESULT {
    caps = flags;
    std::cout << "[DirectDrawPaletteImpl] Initialize stub\n";
    return DD_OK;
}

auto DirectDrawPaletteImpl::SetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY inEntries) -> HRESULT {
    if (!inEntries || !_palette) return DDERR_INVALIDPARAMS;
    if (start >= _palette->ncolors) return DDERR_INVALIDPARAMS;

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
        return DDERR_GENERIC;
    }

    return DD_OK;
}
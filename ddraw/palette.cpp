#include "palette.hpp"
#include "../logger.hpp"

auto DirectDrawPaletteImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
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

    if (riid == IID_IUnknown || riid == IID_IDirectDrawPalette) {
        *ppvObject = this;
        AddRef();
    } else {
        hr = E_NOINTERFACE;
    }

    TRACE_OUT("ppvObject", ppvObject);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawPaletteImpl::AddRef() -> ULONG {
    TRACE_FUNC("ddraw");

    refCount++;
    TRACE_RET("ddraw", refCount);
    return refCount;
}

auto DirectDrawPaletteImpl::Release() -> ULONG {
    TRACE_FUNC("ddraw");

    refCount--;
    ULONG count = refCount;

    TRACE_RET("ddraw", count);

    if (count == 0) {
        if (_palette) SDL_DestroyPalette(_palette);
        delete this;
    }

    return count;
}

auto DirectDrawPaletteImpl::Create(DWORD flags, LPPALETTEENTRY entries, IDirectDrawPalette** outPalette) -> HRESULT {
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

    DirectDrawPaletteImpl* p = new (std::nothrow) DirectDrawPaletteImpl();
    if (!p) {
        hr = DDERR_OUTOFMEMORY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if ((flags & DDPCAPS_8BIT) != DDPCAPS_8BIT) {
        delete p;
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    p->caps = flags;

    p->_palette = SDL_CreatePalette(256);
    if (!p->_palette) {
        delete p;
        hr = DDERR_OUTOFMEMORY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (entries)
        p->SetEntries(0, 0, 256, entries);

    *outPalette = p;

    TRACE_OUT("outPalette", outPalette);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawPaletteImpl::GetCaps(LPDWORD outFlags) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("outFlags", outFlags);

    HRESULT hr = DD_OK;

    if (!outFlags) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    *outFlags = caps;

    TRACE_OUT("outFlags", outFlags);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawPaletteImpl::GetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY outEntries) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("flags", flags);
    TRACE_IN("start", start);
    TRACE_IN("count", count);
    TRACE_IN("outEntries", outEntries);

    HRESULT hr = DD_OK;

    if (!outEntries || !_palette) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (start >= _palette->ncolors) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    count = std::min<DWORD>(count, _palette->ncolors - start);

    for (DWORD i = 0; i < count; i++) {
        const SDL_Color& c = _palette->colors[start + i];
        outEntries[i].peRed = c.r;
        outEntries[i].peGreen = c.g;
        outEntries[i].peBlue = c.b;
        outEntries[i].peFlags = 0;
    }

    TRACE_OUT("outEntries", outEntries);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawPaletteImpl::Initialize(LPDIRECTDRAW lpDD, DWORD flags, LPPALETTEENTRY inEntries) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpDD", lpDD);
    TRACE_IN("flags", flags);
    TRACE_IN("inEntries", inEntries);

    caps = flags;

    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawPaletteImpl::SetEntries(DWORD flags, DWORD start, DWORD count, LPPALETTEENTRY inEntries) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("flags", flags);
    TRACE_IN("start", start);
    TRACE_IN("count", count);
    TRACE_IN("inEntries", inEntries);

    HRESULT hr = DD_OK;

    if (!inEntries || !_palette) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (start >= _palette->ncolors) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
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
        hr = DDERR_GENERIC;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    TRACE_RET("ddraw", hr);
    return hr;
}

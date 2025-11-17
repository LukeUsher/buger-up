#include "surface.hpp"
#include "ddraw.hpp"
#include "../logger.hpp"

DirectDrawSurfaceImpl::~DirectDrawSurfaceImpl() {
    if (_surface) SDL_DestroySurface(_surface);
    if (_texture) SDL_DestroyTexture(_texture);
    if (isPrimary) directDraw._primarySurface = nullptr;
}

auto DirectDrawSurfaceImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
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
    if (riid == IID_IUnknown || riid == IID_IDirectDrawSurface) {
        *ppvObject = this;
        AddRef();
    } else {
        hr = E_NOINTERFACE;
    }

    TRACE_OUT("ppvObject", ppvObject);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::Create(LPDDSURFACEDESC desc, IDirectDrawSurface** outSurface) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("desc", desc);
    TRACE_IN("outSurface", outSurface);

    HRESULT hr = DD_OK;

    if (!desc || !outSurface) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    // Bug! tries to create a surface in the zero page; this seems to be a mistake because it never checks for success or uses it
    if ((uintptr_t)outSurface < 0x1000) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (desc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) {
        // TODO: move to directDraw.createPrimarySurface
        if (!directDraw._primarySurface) {
            DirectDrawSurfaceImpl* primary = new DirectDrawSurfaceImpl();
            if (!primary) {
                hr = DDERR_OUTOFMEMORY;
                TRACE_RET("ddraw", hr);
                return hr;
            }

            primary->isPrimary = true;

            primary->desc.dwSize = sizeof(DDSURFACEDESC);
            primary->desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS;
            primary->desc.dwWidth = directDraw._displayWidth;
            primary->desc.dwHeight = directDraw._displayHeight;
            primary->desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
            primary->desc.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8; // TODO: Don't hardcode, use SetDisplayMode if it was called
            primary->desc.ddpfPixelFormat.dwRGBBitCount = 8;
            primary->desc.ddpfPixelFormat.dwRBitMask = 0;
            primary->desc.ddpfPixelFormat.dwGBitMask = 0;
            primary->desc.ddpfPixelFormat.dwBBitMask = 0;
            primary->desc.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
            primary->desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

            primary->_surface = SDL_CreateSurface(primary->desc.dwWidth, primary->desc.dwHeight, SDL_PIXELFORMAT_INDEX8);
            if (!primary->_surface) {
                delete primary;
                hr = DDERR_INVALIDPIXELFORMAT;
                TRACE_RET("ddraw", hr);
                return hr;
            }

            if (desc->ddsCaps.dwCaps & DDSCAPS_FLIP) {
                DirectDrawSurfaceImpl* bb = new DirectDrawSurfaceImpl();
                if (!bb) {
                    delete primary;
                    hr = DDERR_OUTOFMEMORY;
                    TRACE_RET("ddraw", hr);
                    return hr;
                }

                bb->desc = primary->desc;
                bb->_surface = SDL_CreateSurface(primary->desc.dwWidth, primary->desc.dwHeight, SDL_PIXELFORMAT_INDEX8);
                if (!bb->_surface) {
                    delete bb;
                    delete primary;
                    hr = DDERR_OUTOFMEMORY;
                    TRACE_RET("ddraw", hr);
                    return hr;
                }
                primary->_backbuffer = bb;
            }

            directDraw._primarySurface = primary;
        }

        *outSurface = directDraw._primarySurface;

        SDL_SetWindowFullscreen(directDraw._window, (directDraw._cooperativeLevel & DDSCL_FULLSCREEN) != 0);

        TRACE_OUT("outSurface", outSurface);
        TRACE_RET("ddraw", hr);
        return hr;
    }

    DirectDrawSurfaceImpl* s = new DirectDrawSurfaceImpl();
    if (!s) {
        hr = DDERR_OUTOFMEMORY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    s->desc = *desc;
    s->desc.dwSize = sizeof(DDSURFACEDESC);

    auto sdlFormat = SDL_PIXELFORMAT_UNKNOWN;
    if (s->desc.dwFlags & DDSD_PIXELFORMAT) {
        switch (s->desc.ddpfPixelFormat.dwRGBBitCount) {
        case 8:  sdlFormat = SDL_PIXELFORMAT_INDEX8;   break;
        case 16: sdlFormat = SDL_PIXELFORMAT_RGB565;   break;
        case 24: sdlFormat = SDL_PIXELFORMAT_RGB24;    break;
        case 32: sdlFormat = SDL_PIXELFORMAT_ABGR8888; break;
        default: sdlFormat = SDL_PIXELFORMAT_INDEX8;   break;
        }
    } else {
        sdlFormat = SDL_PIXELFORMAT_INDEX8;
    }

    s->_surface = SDL_CreateSurface(s->desc.dwWidth, s->desc.dwHeight, sdlFormat);
    if (!s->_surface) {
        delete s;
        hr = DDERR_OUTOFMEMORY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    s->desc.ddpfPixelFormat.dwRGBBitCount = SDL_BITSPERPIXEL(sdlFormat);

    *outSurface = s;

    TRACE_OUT("outSurface", outSurface);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::AddRef() -> ULONG {
    TRACE_FUNC("ddraw");
    refCount++;
    TRACE_RET("ddraw", refCount);
    return refCount;
}

auto DirectDrawSurfaceImpl::Release() -> ULONG {
    TRACE_FUNC("ddraw");
    refCount--;
    ULONG count = refCount;
    TRACE_RET("ddraw", count);
    if (count == 0) delete this;
    return count;
}

auto DirectDrawSurfaceImpl::AddAttachedSurface(LPDIRECTDRAWSURFACE) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DDERR_UNSUPPORTED;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::AddOverlayDirtyRect(LPRECT) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DDERR_UNSUPPORTED;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::Blt(LPRECT dstRect, LPDIRECTDRAWSURFACE lpDDSrc, LPRECT srcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFX) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("dstRect", dstRect);
    TRACE_IN("lpDDSrc", lpDDSrc);
    TRACE_IN("srcRect", srcRect);
    TRACE_IN("dwFlags", dwFlags);
    TRACE_IN("lpDDBltFX", lpDDBltFX);

    HRESULT hr = DD_OK;

    if (!_surface) {
        hr = DD_OK;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if ((dwFlags & DDBLT_COLORFILL) && lpDDBltFX) {
        SDL_LockSurface(_surface);
        uint8_t* pixels = (Uint8*)_surface->pixels;
        int pitch = _surface->pitch;
        SDL_Rect dst;

        if (dstRect) {
            dst.x = dstRect->left;
            dst.y = dstRect->top;
            dst.w = dstRect->right - dstRect->left;
            dst.h = dstRect->bottom - dstRect->top;
        } else {
            dst.x = 0;
            dst.y = 0;
            dst.w = _surface->w;
            dst.h = _surface->h;
        }

        int fillWidth = std::min(dst.w, _surface->w - dst.x);
        int fillHeight = std::min(dst.h, _surface->h - dst.y);

        for (int y = dst.y; y < dst.y + dst.h; ++y) {
            uint8_t* row = pixels + y * pitch;
            switch (SDL_BYTESPERPIXEL(_surface->format)) {
                case 1: {
                    uint8_t* row8 = (uint8_t*)row;
                    for (int x = 0; x < fillWidth; ++x) row8[dst.x + x] = (uint8_t)lpDDBltFX->dwFillColor;
                    break;
                }
                case 2: {
                    uint16_t* row16 = (uint16_t*)row;
                    for (int x = 0; x < fillWidth; ++x) row16[dst.x + x] = (uint16_t)lpDDBltFX->dwFillColor;
                    break;
                }
                case 4: {
                    uint32_t* row32 = (uint32_t*)row;
                    for (int x = 0; x < fillWidth; ++x) row32[dst.x + x] = (uint32_t)lpDDBltFX->dwFillColor;
                    break;
                }
            }
        }

        SDL_UnlockSurface(_surface);
    }

    if (lpDDSrc) {
        DirectDrawSurfaceImpl* src = static_cast<DirectDrawSurfaceImpl*>(lpDDSrc);

        SDL_Rect srcR = srcRect ? SDL_Rect{ srcRect->left, srcRect->top, srcRect->right - srcRect->left, srcRect->bottom - srcRect->top }
                                : SDL_Rect{ 0, 0, src->_surface->w, src->_surface->h };

        SDL_Rect dstR = dstRect ? SDL_Rect{ dstRect->left, dstRect->top, dstRect->right - dstRect->left, dstRect->bottom - dstRect->top }
                                : SDL_Rect{ 0, 0, _surface->w, _surface->h };

        if (isPrimary) {
            int winX, winY;
            SDL_GetWindowPosition(directDraw._window, &winX, &winY);
            dstR.x -= winX;
            dstR.y -= winY;
        }

        srcR.w = std::min(srcR.w, src->_surface->w - srcR.x);
        srcR.h = std::min(srcR.h, src->_surface->h - srcR.y);
        dstR.w = std::min(dstR.w, _surface->w - dstR.x);
        dstR.h = std::min(dstR.h, _surface->h - dstR.y);

        //HACK: Assume all surfaces will eventually be blitted to the primary; so make sure all our palettes match
        if (SDL_BYTESPERPIXEL(_surface->format) == 1 && SDL_BYTESPERPIXEL(src->_surface->format) == 1 && directDraw._primarySurface && directDraw._primarySurface->_palette) {
            if (src->_palette == nullptr)  SDL_SetSurfacePalette(src->_surface, directDraw._primarySurface->_palette->_palette);
            if (_palette == nullptr)  SDL_SetSurfacePalette(_surface, directDraw._primarySurface->_palette->_palette);
        }

        if (!SDL_BlitSurfaceScaled(src->_surface, &srcR, _surface, &dstR, SDL_SCALEMODE_LINEAR)) {
            hr = DDERR_INVALIDPIXELFORMAT;
            TRACE_RET("ddraw", hr);
            return hr;
        }
    }

    if (isPrimary && !directDraw.flipPrimary()) {
        hr = DDERR_INCOMPATIBLEPRIMARY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::BltBatch(LPDDBLTBATCH, DWORD, DWORD) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DDERR_UNSUPPORTED;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrc, LPRECT srcRect, DWORD dwFlags) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("dwX", dwX);
    TRACE_IN("dwY", dwY);
    TRACE_IN("lpDDSrc", lpDDSrc);
    TRACE_IN("srcRect", srcRect);
    TRACE_IN("dwFlags", dwFlags);

    HRESULT hr = DD_OK;

    if (!lpDDSrc) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    RECT dstRect{};
    if (srcRect) {
        dstRect.left = static_cast<LONG>(dwX);
        dstRect.top = static_cast<LONG>(dwY);
        dstRect.right = dwX + (srcRect->right - srcRect->left);
        dstRect.bottom = dwY + (srcRect->bottom - srcRect->top);
    } else {
        DDSURFACEDESC ddsd{};
        ddsd.dwSize = sizeof(ddsd);
        lpDDSrc->GetSurfaceDesc(&ddsd);
        dstRect.left = dwX;
        dstRect.top = dwY;
        dstRect.right = dwX + ddsd.dwWidth;
        dstRect.bottom = dwY + ddsd.dwHeight;
    }

    hr = this->Blt(&dstRect, lpDDSrc, srcRect, dwFlags, nullptr);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DDERR_UNSUPPORTED;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::Flip(LPDIRECTDRAWSURFACE, DWORD) -> HRESULT {
    TRACE_FUNC("ddraw");

    HRESULT hr = DD_OK;

    if (!isPrimary || !_backbuffer || !_surface) {
        hr = DDERR_NOTFLIPPABLE;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (!SDL_BlitSurface(_backbuffer->_surface, nullptr, _surface, nullptr)) {
        hr = DDERR_INCOMPATIBLEPRIMARY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (!directDraw.flipPrimary()) {
        hr = DDERR_INCOMPATIBLEPRIMARY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetAttachedSurface(LPDDSCAPS /*caps*/, LPDIRECTDRAWSURFACE* ppSurface) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("ppSurface", ppSurface);

    HRESULT hr = DD_OK;

    if (!ppSurface) {
        hr = E_POINTER;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    if (!isPrimary || !_backbuffer) {
        hr = DDERR_NOTFOUND;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    *ppSurface = _backbuffer;
    _backbuffer->AddRef();

    TRACE_OUT("ppSurface", ppSurface);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetBltStatus(DWORD) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetCaps(LPDDSCAPS) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetClipper(LPDIRECTDRAWCLIPPER*) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetColorKey(DWORD, LPDDCOLORKEY) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetDC(HDC*) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DDERR_UNSUPPORTED;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetFlipStatus(DWORD) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetOverlayPosition(LPLONG, LPLONG) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DDERR_UNSUPPORTED;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetPalette(LPDIRECTDRAWPALETTE*) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetPixelFormat(LPDDPIXELFORMAT) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::GetSurfaceDesc(LPDDSURFACEDESC) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::Initialize(LPDIRECTDRAW, LPDDSURFACEDESC) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::IsLost() -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::Lock(LPRECT pRect, LPDDSURFACEDESC pSurfaceDesc, DWORD dwFlags, HANDLE) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("pRect", pRect);
    TRACE_IN("pSurfaceDesc", pSurfaceDesc);
    TRACE_IN("dwFlags", dwFlags);

    HRESULT hr = DD_OK;

    if (!pSurfaceDesc || !_surface) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    *pSurfaceDesc = desc;

    SDL_Rect rect;
    if (pRect) {
        rect.x = pRect->left;
        rect.y = pRect->top;
        rect.w = pRect->right - pRect->left;
        rect.h = pRect->bottom - pRect->top;
    } else {
        rect = { 0, 0, (int)desc.dwWidth, (int)desc.dwHeight };
    }

    if (!SDL_LockSurface(_surface)) {
        hr = DDERR_GENERIC;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    uint8_t* pixels = static_cast<uint8_t*>(_surface->pixels);
    if (pRect)
        pixels += rect.y * _surface->pitch + rect.x * SDL_BYTESPERPIXEL(_surface->format);

    pSurfaceDesc->lpSurface = pixels;
    pSurfaceDesc->lPitch = _surface->pitch;

    TRACE_OUT("pSurfaceDesc", pSurfaceDesc);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::ReleaseDC(HDC) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::Restore() -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::SetClipper(LPDIRECTDRAWCLIPPER) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::SetColorKey(DWORD, LPDDCOLORKEY) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::SetOverlayPosition(LONG, LONG) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpDDPalette", lpDDPalette);

    HRESULT hr = DD_OK;

    if (!lpDDPalette) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    _palette = static_cast<DirectDrawPaletteImpl*>(lpDDPalette);
    SDL_SetSurfacePalette(_surface, _palette->_palette);

    if (isPrimary && _backbuffer && _backbuffer->_surface)
        SDL_SetSurfacePalette(_backbuffer->_surface, _palette->_palette);

    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::Unlock(LPVOID) -> HRESULT {
    TRACE_FUNC("ddraw");

    HRESULT hr = DD_OK;

    if (!_surface) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    SDL_UnlockSurface(_surface);

    if (isPrimary && !directDraw.flipPrimary()) {
        hr = DDERR_INCOMPATIBLEPRIMARY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DDERR_UNSUPPORTED;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::UpdateOverlayDisplay(DWORD) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawSurfaceImpl::UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE) -> HRESULT {
    TRACE_FUNC("ddraw");
    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

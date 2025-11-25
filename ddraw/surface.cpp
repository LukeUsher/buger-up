#include "ddraw.hpp"
#include "surface.hpp"
#include "clipper.hpp"
#include "palette.hpp"
#include "../gdi32/gdi32.hpp"
#include "../logger.hpp"

DirectDrawSurfaceImpl::~DirectDrawSurfaceImpl() {
    if (_surface) SDL_DestroySurface(_surface);
    if (isPrimary) directDraw._primarySurface = nullptr;
}

auto DirectDrawSurfaceImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM(&riid);
    TRACE_IN_PARAM(ppvObject);

    if (!ppvObject) {
        TRACE_RETURN(E_POINTER);
    }

    *ppvObject = nullptr;
    if (riid == IID_IUnknown || riid == IID_IDirectDrawSurface) {
        *ppvObject = this;
        AddRef();
    } else {
        TRACE_RETURN(E_NOINTERFACE);
    }

    TRACE_OUT_PARAM(ppvObject);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::Create(LPDDSURFACEDESC desc, IDirectDrawSurface** outSurface) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM(desc);
    TRACE_IN_PARAM(outSurface);

    if (!desc || !outSurface) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    // Bug! tries to create a surface in the zero page; this seems to be a mistake because it never checks for success or uses it
    if ((uintptr_t)outSurface < 0x1000) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    if (desc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) {
        if (directDraw._primarySurface) {
			SDL_DestroySurface(directDraw._primarySurface->_surface);
			delete directDraw._primarySurface;
			directDraw._primarySurface = nullptr;
        }

        directDraw._primarySurface = new DirectDrawSurfaceImpl();
        if (!directDraw._primarySurface) {
            TRACE_RETURN(DDERR_OUTOFMEMORY);
        }

        directDraw._primarySurface->isPrimary = true;

        directDraw._primarySurface->desc.dwSize = sizeof(DDSURFACEDESC);
        directDraw._primarySurface->desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS;
        directDraw._primarySurface->desc.dwWidth = directDraw._displayWidth;
        directDraw._primarySurface->desc.dwHeight = directDraw._displayHeight;
        directDraw._primarySurface->desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
        directDraw._primarySurface->desc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8; // TODO: Don't hardcode, use SetDisplayMode if it was called
        directDraw._primarySurface->desc.ddpfPixelFormat.dwRGBBitCount = 8;
        directDraw._primarySurface->desc.ddpfPixelFormat.dwRBitMask = 0;
        directDraw._primarySurface->desc.ddpfPixelFormat.dwGBitMask = 0;
        directDraw._primarySurface->desc.ddpfPixelFormat.dwBBitMask = 0;
        directDraw._primarySurface->desc.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
        directDraw._primarySurface->desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY | DDSCAPS_FRONTBUFFER;

        directDraw._primarySurface->_surface = SDL_CreateSurface(directDraw._primarySurface->desc.dwWidth, directDraw._primarySurface->desc.dwHeight, SDL_PIXELFORMAT_INDEX8);
        if (!directDraw._primarySurface->_surface) {
            delete directDraw._primarySurface;
            TRACE_RETURN(DDERR_INVALIDPIXELFORMAT);
        }

        auto bb = new DirectDrawSurfaceImpl();
        if (!bb) {
            delete directDraw._primarySurface;
            TRACE_RETURN(DDERR_OUTOFMEMORY);
        }

        bb->desc = directDraw._primarySurface->desc;
        bb->_surface = SDL_CreateSurface(directDraw._primarySurface->desc.dwWidth, directDraw._primarySurface->desc.dwHeight, SDL_PIXELFORMAT_INDEX8);
        if (!bb->_surface) {
            delete bb;
            delete directDraw._primarySurface;
            TRACE_RETURN(DDERR_OUTOFMEMORY);
        }
        directDraw._primarySurface->_backbuffer = bb;
   
        *outSurface = directDraw._primarySurface;

        SDL_SetWindowFullscreen(directDraw._window, (directDraw._cooperativeLevel & DDSCL_FULLSCREEN) != 0);

        TRACE_OUT_PARAM(outSurface);
        TRACE_RETURN(DD_OK);
    }

    auto s = new DirectDrawSurfaceImpl();
    if (!s) {
        TRACE_RETURN(DDERR_OUTOFMEMORY);
    }

    s->desc = {};
    s->desc.dwSize = sizeof(DDSURFACEDESC);

    s->desc.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
    s->desc.dwWidth = 1;
    s->desc.dwHeight = 1;
    if (desc->dwFlags & DDSD_WIDTH) s->desc.dwWidth = desc->dwWidth;
    if (desc->dwFlags & DDSD_HEIGHT) s->desc.dwHeight = desc->dwHeight;
    s->desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    s->desc.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
    s->desc.ddpfPixelFormat.dwRGBBitCount = 8;
    if (desc->dwFlags & DDSD_PIXELFORMAT) s->desc.ddpfPixelFormat = desc->ddpfPixelFormat;

    auto sdlFormat = SDL_PIXELFORMAT_INDEX8;
    switch (s->desc.ddpfPixelFormat.dwRGBBitCount) {
    case 8:  sdlFormat = SDL_PIXELFORMAT_INDEX8; break;
    case 16: sdlFormat = SDL_PIXELFORMAT_RGB565; break;
    case 24: sdlFormat = SDL_PIXELFORMAT_RGB24;  break;
    case 32: sdlFormat = SDL_PIXELFORMAT_ABGR8888; break;
    }

    s->desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
    if (desc->dwFlags & DDSD_CAPS) {
        if (desc->ddsCaps.dwCaps & DDSCAPS_SYSTEMMEMORY) s->desc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
        else if (desc->ddsCaps.dwCaps & (DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM | DDSCAPS_NONLOCALVIDMEM)) s->desc.ddsCaps.dwCaps = desc->ddsCaps.dwCaps | DDSCAPS_VIDEOMEMORY | DDSCAPS_OFFSCREENPLAIN;
        else if (desc->ddsCaps.dwCaps & DDSCAPS_BACKBUFFER) s->desc.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER | DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
        else if (desc->ddsCaps.dwCaps & DDSCAPS_OFFSCREENPLAIN) s->desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
        else s->desc.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
    }

    s->_surface = SDL_CreateSurface(s->desc.dwWidth, s->desc.dwHeight, sdlFormat);
    if (!s->_surface) {
        TRACE_RETURN(DDERR_OUTOFMEMORY);
    }

    s->desc.lPitch = s->_surface->pitch;
    s->desc.dwFlags |= DDSD_PITCH;
    s->desc.ddpfPixelFormat.dwRGBBitCount = SDL_BITSPERPIXEL(sdlFormat);

    *outSurface = s;

    TRACE_OUT_PARAM(outSurface);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::AddRef() -> ULONG {
    TRACE_FUNCTION_ENTRY("ddraw");
    TRACE_RETURN(++refCount);
}

auto DirectDrawSurfaceImpl::Release() -> ULONG {
    TRACE_FUNCTION_ENTRY("ddraw");
    refCount--;
    if (refCount == 0) delete this;
    TRACE_RETURN(refCount);
}

auto DirectDrawSurfaceImpl::AddAttachedSurface(LPDIRECTDRAWSURFACE lpDDSAttachedSurface) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_IN_PARAM(lpDDSAttachedSurface);
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDrawSurfaceImpl::AddOverlayDirtyRect(LPRECT) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDrawSurfaceImpl::Blt(LPRECT dstRect, LPDIRECTDRAWSURFACE lpDDSrc, LPRECT srcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFX) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM(dstRect);
    TRACE_IN_PARAM(lpDDSrc);
    TRACE_IN_PARAM(srcRect);
    TRACE_IN_PARAM(dwFlags);
    TRACE_IN_PARAM(lpDDBltFX);

    if (!_surface) {
        TRACE_RETURN(DD_OK);
    }

    if ((dwFlags & DDBLT_COLORFILL) && lpDDBltFX) {
        SDL_LockSurface(_surface);
        auto pixels = static_cast<uint8_t*>(_surface->pixels);
        auto pitch = _surface->pitch;
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

        auto fillWidth = std::min(dst.w, _surface->w - dst.x);
        auto fillHeight = std::min(dst.h, _surface->h - dst.y);

        for (auto y = dst.y; y < dst.y + dst.h; ++y) {
            auto row = pixels + y * pitch;
            switch (SDL_BYTESPERPIXEL(_surface->format)) {
                case 1: {
                    for (auto x = 0; x < fillWidth; ++x) row[dst.x + x] = static_cast<uint8_t>(lpDDBltFX->dwFillColor);
                    break;
                }
                case 2: {
                    auto row16 = reinterpret_cast<uint16_t*>(row);
                    for (auto x = 0; x < fillWidth; ++x) row16[dst.x + x] = static_cast<uint16_t>(lpDDBltFX->dwFillColor);
                    break;
                }
                case 4: {
                    auto row32 = reinterpret_cast<uint32_t*>(row);
                    for (auto x = 0; x < fillWidth; ++x) row32[dst.x + x] = static_cast<uint32_t>(lpDDBltFX->dwFillColor);
                    break;
                }
            }
        }

        SDL_UnlockSurface(_surface);
    }

    _lastBlit = SDL_GetTicks() + 2;

    if (lpDDSrc) {
        auto src = static_cast<DirectDrawSurfaceImpl*>(lpDDSrc);

        auto srcR = srcRect ? SDL_Rect{ srcRect->left, srcRect->top, srcRect->right - srcRect->left, srcRect->bottom - srcRect->top }
                                : SDL_Rect{ 0, 0, src->_surface->w, src->_surface->h };

        auto dstR = dstRect ? SDL_Rect{ dstRect->left, dstRect->top, dstRect->right - dstRect->left, dstRect->bottom - dstRect->top }
                                : SDL_Rect{ 0, 0, _surface->w, _surface->h };

        srcR.w = std::min(srcR.w, src->_surface->w - srcR.x);
        srcR.h = std::min(srcR.h, src->_surface->h - srcR.y);
        dstR.w = std::min(dstR.w, _surface->w - dstR.x);
        dstR.h = std::min(dstR.h, _surface->h - dstR.y);

		//If both services are 8bpp paletted use StretchSurface instead of BlitSurfaceScaled to avoid expensive per-pixel colour lookups
        if (SDL_BYTESPERPIXEL(_surface->format) == 1 && SDL_BYTESPERPIXEL(src->_surface->format) == 1) {
            if (!SDL_StretchSurface(src->_surface, &srcR, _surface, &dstR, SDL_SCALEMODE_NEAREST)) {
				printf("SDL_StretchSurface failed: %s\n", SDL_GetError());
                TRACE_RETURN(DDERR_INVALIDPIXELFORMAT);
            }
        } else if (!SDL_BlitSurfaceScaled(src->_surface, &srcR, _surface, &dstR, SDL_SCALEMODE_LINEAR)) {
			printf("SDL_BlitSurfaceScaled failed: %s\n", SDL_GetError());
            TRACE_RETURN(DDERR_INVALIDPIXELFORMAT);
        }
    }

    if (isPrimary && !directDraw.flipPrimary()) {
        TRACE_RETURN(DDERR_INCOMPATIBLEPRIMARY);
    }

    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::BltBatch(LPDDBLTBATCH, DWORD, DWORD) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDrawSurfaceImpl::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrc, LPRECT srcRect, DWORD dwFlags) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM(dwX);
    TRACE_IN_PARAM(dwY);
    TRACE_IN_PARAM(lpDDSrc);
    TRACE_IN_PARAM(srcRect);
    TRACE_IN_PARAM(dwFlags);

    if (!lpDDSrc) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    if (_clipper) {
        TRACE_RETURN(DDERR_BLTFASTCANTCLIP);
    }

    auto src = static_cast<DirectDrawSurfaceImpl*>(lpDDSrc);

    SDL_SetSurfacePalette(src->_surface, gdi32._systemPalette);
    SDL_SetSurfacePalette(_surface, gdi32._systemPalette);

    RECT srect{};
    if (srcRect) {
        srect = *srcRect;
    } else {
        srect.left = 0;
        srect.top = 0;
        srect.right = src->_surface->w;
        srect.bottom = src->_surface->h;
    }

    if (srect.left < 0 || srect.top < 0 || srect.right > src->_surface->w || srect.bottom > src->_surface->h)  {
        TRACE_RETURN(DDERR_INVALIDRECT);
    }

    auto width = srect.right - srect.left;
    auto height = srect.bottom - srect.top;

    RECT drect{};
    drect.left = dwX;
    drect.top = dwY;
    drect.right = dwX + width;
    drect.bottom = dwY + height;

    if (drect.left < 0 || drect.top < 0 || drect.right > _surface->w || drect.bottom > _surface->h) {
        TRACE_RETURN(DDERR_INVALIDRECT);
    }

    _lastBlit = SDL_GetTicks() + 2;
    SDL_Rect sdlSrc = { srect.left, srect.top, width, height };
    SDL_Rect sdlDst = { drect.left, drect.top, width, height };

    if (!SDL_BlitSurface(src->_surface, &sdlSrc, _surface, &sdlDst)) {
        printf("SDL_BlitSurface failed: %s\n", SDL_GetError());
        TRACE_RETURN(DDERR_INVALIDPIXELFORMAT);
    }

    if (isPrimary && !directDraw.flipPrimary()) {
       TRACE_RETURN(DDERR_INCOMPATIBLEPRIMARY);
    }

    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDrawSurfaceImpl::EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::Flip(LPDIRECTDRAWSURFACE, DWORD) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    if (!isPrimary || !_backbuffer || !_surface) {
        TRACE_RETURN(DDERR_NOTFLIPPABLE);
    }

    if (!SDL_BlitSurface(_backbuffer->_surface, nullptr, _surface, nullptr)) {
        TRACE_RETURN(DDERR_INCOMPATIBLEPRIMARY);
    }

    if (!directDraw.flipPrimary()) {
        TRACE_RETURN(DDERR_INCOMPATIBLEPRIMARY);
    }

    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::GetAttachedSurface(LPDDSCAPS /*caps*/, LPDIRECTDRAWSURFACE* ppSurface) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM(ppSurface);

    if (!ppSurface) {
        TRACE_RETURN(E_POINTER);
    }

    if (!isPrimary || !_backbuffer) {
        TRACE_RETURN(DDERR_NOTFOUND);
    }

    *ppSurface = _backbuffer;
    _backbuffer->AddRef();

    TRACE_OUT_PARAM(ppSurface);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::GetBltStatus(DWORD flags) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");
    TRACE_IN_PARAM(flags);

    auto now = SDL_GetTicks();

    if (now < _lastBlit) {
        TRACE_RETURN(DDERR_WASSTILLDRAWING);
    }

    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::GetCaps(LPDDSCAPS) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::GetClipper(LPDIRECTDRAWCLIPPER*) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::GetColorKey(DWORD, LPDDCOLORKEY) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::GetDC(HDC*) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDrawSurfaceImpl::GetFlipStatus(DWORD) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::GetOverlayPosition(LPLONG, LPLONG) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDrawSurfaceImpl::GetPalette(LPDIRECTDRAWPALETTE*) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDrawSurfaceImpl::GetPixelFormat(LPDDPIXELFORMAT) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DDERR_UNSUPPORTED);
}

auto DirectDrawSurfaceImpl::GetSurfaceDesc(LPDDSURFACEDESC lpDesc) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");
	TRACE_IN_PARAM(lpDesc);
    *lpDesc = desc;
    TRACE_OUT_PARAM(lpDesc);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::Initialize(LPDIRECTDRAW, LPDDSURFACEDESC) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::IsLost() -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::Lock(LPRECT pRect, LPDDSURFACEDESC pSurfaceDesc, DWORD dwFlags, HANDLE) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM(pRect);
    TRACE_IN_PARAM(pSurfaceDesc);
    TRACE_IN_PARAM(dwFlags);

    if (!pSurfaceDesc || !_surface) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

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
        TRACE_RETURN(DDERR_GENERIC);
    }

    auto pixels = static_cast<uint8_t*>(_surface->pixels);
    if (pRect) pixels += rect.y * _surface->pitch + rect.x * SDL_BYTESPERPIXEL(_surface->format);

    *pSurfaceDesc = {};
    pSurfaceDesc->dwSize = sizeof(DDSURFACEDESC);
    pSurfaceDesc->dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_LPSURFACE | DDSD_PITCH;
    pSurfaceDesc->lpSurface = pixels;
    pSurfaceDesc->lPitch = _surface->pitch;
    pSurfaceDesc->dwWidth = desc.dwWidth;
    pSurfaceDesc->dwHeight = desc.dwHeight;
    pSurfaceDesc->ddpfPixelFormat = desc.ddpfPixelFormat;

    TRACE_OUT_PARAM(pSurfaceDesc);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::ReleaseDC(HDC) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::Restore() -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::SetClipper(LPDIRECTDRAWCLIPPER lpDirectDrawClipper) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");
    TRACE_IN_PARAM(lpDirectDrawClipper);

    DirectDrawClipperImpl* newClip = reinterpret_cast<DirectDrawClipperImpl*>(lpDirectDrawClipper);

    if (!newClip) {
        if (_clipper) {
            _clipper->_attachedSurface = nullptr;
            _clipper->Release();
            _clipper = nullptr;
        }
        TRACE_RETURN(DD_OK);
    }

    if (_clipper == newClip) {
        TRACE_RETURN(DD_OK);
    }

    if (_clipper) {
        _clipper->_attachedSurface = nullptr;
        _clipper->Release();
        _clipper = nullptr;
    }

    newClip->AddRef();
    newClip->_attachedSurface = this;
    _clipper = newClip;

    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::SetColorKey(DWORD, LPDDCOLORKEY) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::SetOverlayPosition(LONG, LONG) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    TRACE_IN_PARAM(lpDDPalette);

    if (!lpDDPalette) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    _palette = reinterpret_cast<DirectDrawPaletteImpl*>(lpDDPalette);
	SDL_SetSurfacePalette(_surface, _palette->_palette);
    if(isPrimary && _backbuffer) {
		SDL_SetSurfacePalette(_backbuffer->_surface, _palette->_palette);
	}

    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::Unlock(LPVOID) -> HRESULT {
    TRACE_FUNCTION_ENTRY("ddraw");

    if (!_surface) {
          TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    SDL_UnlockSurface(_surface);

    if (isPrimary && !directDraw.flipPrimary()) {
        TRACE_RETURN(DDERR_INCOMPATIBLEPRIMARY);
    }

    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::UpdateOverlayDisplay(DWORD) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

auto DirectDrawSurfaceImpl::UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("ddraw");
    TRACE_RETURN(DD_OK);
}

#include "surface.hpp"
#include "ddraw.hpp"

DirectDrawSurfaceImpl::~DirectDrawSurfaceImpl() {
    if (_surface) SDL_DestroySurface(_surface);
    if (_texture) SDL_DestroyTexture(_texture);
    if (isPrimary) directDraw._primarySurface = nullptr;
}

auto DirectDrawSurfaceImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    if (!ppvObject) return E_POINTER;
    *ppvObject = nullptr;
    if (riid == IID_IUnknown || riid == IID_IDirectDrawSurface) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

auto DirectDrawSurfaceImpl::Create(LPDDSURFACEDESC desc, IDirectDrawSurface** outSurface) -> HRESULT {
    if (!desc || !outSurface) return DDERR_INVALIDPARAMS;

    // Bug! tries to create a surface in the zero page; this seems to be a mistake because it never checks for success or uses it
    if ((uintptr_t)outSurface < 0x1000) return DDERR_INVALIDPARAMS;

    if (desc->ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) {
        //TODO: move to directDraw.createPrimarySurface
        if (!directDraw._primarySurface) {
            DirectDrawSurfaceImpl* primary = new DirectDrawSurfaceImpl();
            if (!primary) return DDERR_OUTOFMEMORY;

            primary->isPrimary = true;

            primary->desc.dwSize = sizeof(DDSURFACEDESC);
            primary->desc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS;
            primary->desc.dwWidth = directDraw._displayWidth;
            primary->desc.dwHeight = directDraw._displayHeight;
            primary->desc.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
            primary->desc.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8; //TODO: Don't hardcode, use SetDisplayMode if it was called
            primary->desc.ddpfPixelFormat.dwRGBBitCount = 8;
            primary->desc.ddpfPixelFormat.dwRBitMask = 0;
            primary->desc.ddpfPixelFormat.dwGBitMask = 0;
            primary->desc.ddpfPixelFormat.dwBBitMask = 0;
            primary->desc.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
            primary->desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
            primary->_surface = SDL_CreateSurface(primary->desc.dwWidth, primary->desc.dwHeight, SDL_PIXELFORMAT_INDEX8);
            if (!primary->_surface) {
                delete primary;
                return DDERR_INVALIDPIXELFORMAT;
            }

            if (desc->ddsCaps.dwCaps & DDSCAPS_FLIP) {
                DirectDrawSurfaceImpl* bb = new DirectDrawSurfaceImpl();
                bb->desc = primary->desc;
                bb->_surface = SDL_CreateSurface(primary->desc.dwWidth, primary->desc.dwHeight, SDL_PIXELFORMAT_INDEX8);
                if (!bb->_surface) {
                    delete bb;
                    delete primary;
                    return DDERR_OUTOFMEMORY;
                }
                primary->_backbuffer = bb;
            }

            directDraw._primarySurface = primary;
 
        }

        *outSurface = directDraw._primarySurface;

        SDL_SetWindowFullscreen(directDraw._window, (directDraw._cooperativeLevel & DDSCL_FULLSCREEN) != 0);


        return DD_OK;
    }

    DirectDrawSurfaceImpl* s = new DirectDrawSurfaceImpl();
    if (!s) return DDERR_OUTOFMEMORY;

    s->desc = *desc;
    s->desc.dwSize = sizeof(DDSURFACEDESC);

    auto sdlFormat = SDL_PIXELFORMAT_UNKNOWN;
    if (s->desc.dwFlags & DDSD_PIXELFORMAT) {
        switch (s->desc.ddpfPixelFormat.dwRGBBitCount) {
            case 8:  sdlFormat = SDL_PIXELFORMAT_INDEX8; break;
            case 16: sdlFormat = SDL_PIXELFORMAT_RGB565; break;
            case 24: sdlFormat = SDL_PIXELFORMAT_RGB24; break;
            case 32: sdlFormat = SDL_PIXELFORMAT_ABGR8888; break;
            default: sdlFormat = SDL_PIXELFORMAT_INDEX8;
        }
    } else {
        sdlFormat = SDL_PIXELFORMAT_INDEX8;
    }

    s->_surface = SDL_CreateSurface(s->desc.dwWidth, s->desc.dwHeight, sdlFormat);

    if (!s->_surface) {
        delete s;
        return DDERR_OUTOFMEMORY;
    }

    s->desc.ddpfPixelFormat.dwRGBBitCount = SDL_BITSPERPIXEL(sdlFormat);

    if (!s->_surface) {
        delete s;
        return DDERR_OUTOFMEMORY;
    }

    *outSurface = s;
    return DD_OK;
}

auto DirectDrawSurfaceImpl::AddRef() -> ULONG {
    return ++refCount;
}

auto DirectDrawSurfaceImpl::Release() -> ULONG {
    ULONG count = --refCount;
    if (count == 0) delete this;
    return count;
}

auto DirectDrawSurfaceImpl::AddAttachedSurface(LPDIRECTDRAWSURFACE) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] AddAttachedSurface ignored\n";
    return DDERR_UNSUPPORTED;
}

auto DirectDrawSurfaceImpl::AddOverlayDirtyRect(LPRECT) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] AddOverlayDirtyRect ignored\n";
    return DDERR_UNSUPPORTED;
}

auto DirectDrawSurfaceImpl::Blt(LPRECT dstRect, LPDIRECTDRAWSURFACE lpDDSrc, LPRECT srcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFX) -> HRESULT {
    if (!_surface) return DD_OK;
 
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

        if (!SDL_BlitSurfaceScaled(src->_surface, &srcR, _surface, &dstR, SDL_SCALEMODE_LINEAR)) return DDERR_INVALIDPIXELFORMAT;
    }

    if (isPrimary && !directDraw.flipPrimary()) return DDERR_INCOMPATIBLEPRIMARY;
    return DD_OK;
}


auto DirectDrawSurfaceImpl::BltBatch(LPDDBLTBATCH, DWORD, DWORD) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] BltBatch ignored\n";
    return DDERR_UNSUPPORTED;
}

auto DirectDrawSurfaceImpl::BltFast(DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE lpDDSrc, LPRECT srcRect, DWORD dwFlags) -> HRESULT {
    if (!lpDDSrc) return DDERR_INVALIDPARAMS;

    RECT dstRect{};
    if (srcRect) {
        dstRect.left = static_cast<LONG>(dwX);
        dstRect.top = static_cast<LONG>(dwY);
        dstRect.right = dwX + (srcRect->right - srcRect->left);
        dstRect.bottom = dwY + (srcRect->bottom - srcRect->top);
    }  else {
        DDSURFACEDESC ddsd{};
        ddsd.dwSize = sizeof(ddsd);
        lpDDSrc->GetSurfaceDesc(&ddsd);
        dstRect.left = dwX;
        dstRect.top = dwY;
        dstRect.right = dwX + ddsd.dwWidth;
        dstRect.bottom = dwY + ddsd.dwHeight;
    }

    return this->Blt(&dstRect, lpDDSrc, srcRect, dwFlags, nullptr);
}

auto DirectDrawSurfaceImpl::DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] DeleteAttachedSurface ignored\n";
    return DDERR_UNSUPPORTED;
}

auto DirectDrawSurfaceImpl::EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] EnumAttachedSurfaces ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::EnumOverlayZOrders(DWORD, LPVOID, LPDDENUMSURFACESCALLBACK) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] EnumOverlayZOrders ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::Flip(LPDIRECTDRAWSURFACE, DWORD) -> HRESULT {
    if (!isPrimary) return DDERR_NOTFLIPPABLE;
    if (!_backbuffer || !_surface) return DDERR_NOTFLIPPABLE;

    if (!SDL_BlitSurface(_backbuffer->_surface, nullptr, _surface, nullptr)) return DDERR_INCOMPATIBLEPRIMARY;

    if (!directDraw.flipPrimary()) return DDERR_INCOMPATIBLEPRIMARY;
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetAttachedSurface(LPDDSCAPS /*caps*/, LPDIRECTDRAWSURFACE* ppSurface) -> HRESULT {
    if (!ppSurface) return E_POINTER;
    if (!isPrimary || !_backbuffer) return DDERR_NOTFOUND;
    *ppSurface = _backbuffer;
    _backbuffer->AddRef();
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetBltStatus(DWORD) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetBltStatus ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetCaps(LPDDSCAPS) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetCaps ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetClipper(LPDIRECTDRAWCLIPPER*) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetClipper ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetColorKey(DWORD, LPDDCOLORKEY) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetColorKey ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetDC(HDC*) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetDC ignored\n";
    return DDERR_UNSUPPORTED;
}

auto DirectDrawSurfaceImpl::GetFlipStatus(DWORD) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetFlipStatus ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetOverlayPosition(LPLONG, LPLONG) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetOverlayPosition ignored\n";
    return DDERR_UNSUPPORTED;
}

auto DirectDrawSurfaceImpl::GetPalette(LPDIRECTDRAWPALETTE*) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetPalette ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetPixelFormat(LPDDPIXELFORMAT) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] GetPixelFormat ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::GetSurfaceDesc(LPDDSURFACEDESC lpDesc) -> HRESULT {
    if (!lpDesc) return DDERR_INVALIDPARAMS;
    *lpDesc = desc;
    return DD_OK;
}

auto DirectDrawSurfaceImpl::Initialize(LPDIRECTDRAW, LPDDSURFACEDESC) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] Initialize ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::IsLost() -> HRESULT {
    //std::cout << "[DirectDrawSurfaceImpl] IsLost ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::Lock(LPRECT pRect, LPDDSURFACEDESC pSurfaceDesc, DWORD dwFlags, HANDLE) -> HRESULT {
    if (!pSurfaceDesc) return DDERR_INVALIDPARAMS;
    if (!_surface) return DDERR_INVALIDPARAMS;

    *pSurfaceDesc = desc;

    void* pixelsPtr = nullptr;
    int pitch = 0;

    SDL_Rect rect;
    if (pRect) {
        rect.x = pRect->left;
        rect.y = pRect->top;
        rect.w = pRect->right - pRect->left;
        rect.h = pRect->bottom - pRect->top;
    } else {
        rect = { 0, 0, (int)desc.dwWidth, (int)desc.dwHeight };
    }

    if(!SDL_LockSurface(_surface)) return DDERR_GENERIC;

    uint8_t* pixels = static_cast<uint8_t*>(_surface->pixels);
    if (pRect) pixels += rect.y * _surface->pitch + rect.x * SDL_BYTESPERPIXEL(_surface->format);

    pSurfaceDesc->lpSurface = pixels;
    pSurfaceDesc->lPitch = _surface->pitch;

    return DD_OK;
}


auto DirectDrawSurfaceImpl::ReleaseDC(HDC) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] ReleaseDC ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::Restore() -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] Restore ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::SetClipper(LPDIRECTDRAWCLIPPER) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] SetClipper ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::SetColorKey(DWORD, LPDDCOLORKEY) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] SetColorKey ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::SetOverlayPosition(LONG, LONG) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] SetOverlayPosition ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::SetPalette(LPDIRECTDRAWPALETTE lpDDPalette) -> HRESULT {
    if (!lpDDPalette) return DDERR_INVALIDPARAMS;
    _palette = static_cast<DirectDrawPaletteImpl*>(lpDDPalette);
    SDL_SetSurfacePalette(_surface, _palette->_palette);

    if (isPrimary && _backbuffer && _backbuffer->_surface) SDL_SetSurfacePalette(_backbuffer->_surface, _palette->_palette);
    return DD_OK;
}

auto DirectDrawSurfaceImpl::Unlock(LPVOID) -> HRESULT {
    if (!_surface) return DDERR_INVALIDPARAMS;
    SDL_UnlockSurface(_surface);
    if (isPrimary && !directDraw.flipPrimary()) return DDERR_INCOMPATIBLEPRIMARY;
    return DD_OK;
}

auto DirectDrawSurfaceImpl::UpdateOverlay(LPRECT, LPDIRECTDRAWSURFACE, LPRECT, DWORD, LPDDOVERLAYFX) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] UpdateOverlay ignored\n";
    return DDERR_UNSUPPORTED;
}

auto DirectDrawSurfaceImpl::UpdateOverlayDisplay(DWORD) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] UpdateOverlayDisplay ignored\n";
    return DD_OK;
}

auto DirectDrawSurfaceImpl::UpdateOverlayZOrder(DWORD, LPDIRECTDRAWSURFACE) -> HRESULT {
    std::cout << "[DirectDrawSurfaceImpl] UpdateOverlayZOrder ignored\n";
    return DD_OK;
}

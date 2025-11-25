#include "clipper.hpp"
#include "../logger.hpp"

auto DirectDrawClipperImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM(riid);
    TRACE_IN_PARAM(ppvObject);

    if (!ppvObject) {
        TRACE_RETURN(E_POINTER);
    }

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IDirectDrawClipper) {
        *ppvObject = this;
        AddRef();
    } else {
        TRACE_RETURN(E_NOINTERFACE);
    }

    TRACE_OUT_PARAM(ppvObject);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::AddRef() -> ULONG {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");
    TRACE_RETURN(++refCount);
}

auto DirectDrawClipperImpl::Release() -> ULONG {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    refCount--;
    if (refCount == 0) delete this;

    TRACE_RETURN(--refCount);
}

auto DirectDrawClipperImpl::Create(DWORD flags, IDirectDrawClipper** outClipper) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM(flags);
    TRACE_IN_PARAM(outClipper);

    if (!outClipper) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto p = new (std::nothrow) DirectDrawClipperImpl();
    if (!p) {
        TRACE_RETURN(DDERR_OUTOFMEMORY);
    }

    p->caps = flags;
    *outClipper = p;

    TRACE_OUT_PARAM(outClipper);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::GetClipList(LPRECT lpRect, LPRGNDATA lpRgnData, LPDWORD lpSize) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM(lpRect);
    TRACE_IN_PARAM(lpRgnData);
    TRACE_IN_PARAM(lpSize);

    if (!lpSize) TRACE_RETURN(DDERR_INVALIDPARAMS);
    if (!_hwnd && !_staticClipList) return DDERR_NOCLIPLIST;

    RGNDATA* srcRegion = nullptr;
	std::vector<uint8_t> temp; // to hold dynamic region data

    if (_hwnd) {
        RECT client;
        if (!GetClientRect(_hwnd, &client)) TRACE_RETURN(DDERR_INVALIDPARAMS);
        MapWindowPoints(_hwnd, NULL, reinterpret_cast<POINT*>(&client), 2);

        DWORD required = sizeof(RGNDATAHEADER) + sizeof(RECT);
        temp.resize(required);

        LPRGNDATA rgn = reinterpret_cast<LPRGNDATA>(temp.data());
        rgn->rdh.dwSize = sizeof(RGNDATAHEADER);
        rgn->rdh.iType = RDH_RECTANGLES;
        rgn->rdh.nCount = 1;
        rgn->rdh.nRgnSize = sizeof(RECT);
        rgn->rdh.rcBound = client;

        memcpy(rgn->Buffer, &client, sizeof(RECT));

        srcRegion = rgn;
    } else if (_staticClipList) {
        srcRegion = reinterpret_cast<RGNDATA*>(_staticClipList->data());
	}

     DWORD required = srcRegion->rdh.dwSize + srcRegion->rdh.nRgnSize;
    if (!lpRgnData) {
        *lpSize = required;
        TRACE_RETURN(DD_OK);
    }

    if (*lpSize < required) TRACE_RETURN(DDERR_REGIONTOOSMALL);

    memcpy(lpRgnData, srcRegion, required);
    *lpSize = required;

    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::GetHWnd(HWND* outHwnd) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM(outHwnd);

    if (!outHwnd) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    *outHwnd = _hwnd;

    TRACE_OUT_PARAM(outHwnd);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::Initialize(LPDIRECTDRAW lpDD, DWORD flags) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM(lpDD);
    TRACE_IN_PARAM(flags);

    caps = flags;

    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::IsClipListChanged(BOOL* outChanged) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDrawClipper");

    TRACE_IN_PARAM(outChanged);

    if (!outChanged) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    *outChanged = FALSE;

    TRACE_OUT_PARAM(outChanged);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::SetClipList(LPRGNDATA lpRgnData, DWORD flags) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM(lpRgnData);
    TRACE_IN_PARAM(flags);

    if (_hwnd) return DDERR_CLIPPERISUSINGHWND;
    if (!lpRgnData) return DDERR_INVALIDPARAMS;

    DWORD size = lpRgnData->rdh.dwSize + lpRgnData->rdh.nRgnSize;
    _staticClipList.emplace();
    _staticClipList->resize(size);
    memcpy(_staticClipList->data(), lpRgnData, size);

    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::SetHWnd(DWORD flags, HWND hwnd) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM(flags);
    TRACE_IN_PARAM(hwnd);

	_hwnd = hwnd;

    TRACE_RETURN(DD_OK);
}

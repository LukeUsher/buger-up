#include "clipper.hpp"
#include "../logger.hpp"

auto DirectDrawClipperImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM("riid", &riid);
    TRACE_IN_PARAM("ppvObject", ppvObject);

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

    TRACE_OUT_PARAM("ppvObject", ppvObject);
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

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("outClipper", outClipper);

    if (!outClipper) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    auto p = new (std::nothrow) DirectDrawClipperImpl();
    if (!p) {
        TRACE_RETURN(DDERR_OUTOFMEMORY);
    }

    p->caps = flags;
    *outClipper = p;

    TRACE_OUT_PARAM("outClipper", outClipper);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::GetClipList(LPRECT lpRect, LPRGNDATA lpRgnData, LPDWORD lpSize) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDrawClipper");

    TRACE_IN_PARAM("lpRect", lpRect);
    TRACE_IN_PARAM("lpRgnData", lpRgnData);
    TRACE_IN_PARAM("lpSize", lpSize);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::GetHWnd(HWND* outHwnd) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM("outHwnd", outHwnd);

    if (!outHwnd) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    *outHwnd = nullptr;

    TRACE_OUT_PARAM("outHwnd", outHwnd);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::Initialize(LPDIRECTDRAW lpDD, DWORD flags) -> HRESULT {
    TRACE_FUNCTION_ENTRY("IDirectDrawClipper");

    TRACE_IN_PARAM("lpDD", lpDD);
    TRACE_IN_PARAM("flags", flags);

    caps = flags;

    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::IsClipListChanged(BOOL* outChanged) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDrawClipper");

    TRACE_IN_PARAM("outChanged", outChanged);

    if (!outChanged) {
        TRACE_RETURN(DDERR_INVALIDPARAMS);
    }

    *outChanged = FALSE;

    TRACE_OUT_PARAM("outChanged", outChanged);
    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::SetClipList(LPRGNDATA lpRgnData, DWORD flags) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDrawClipper");

    TRACE_IN_PARAM("lpRgnData", lpRgnData);
    TRACE_IN_PARAM("flags", flags);

    TRACE_RETURN(DD_OK);
}

auto DirectDrawClipperImpl::SetHWnd(DWORD flags, HWND hwnd) -> HRESULT {
    TRACE_FUNCTION_ENTRY_STUB("IDirectDrawClipper");

    TRACE_IN_PARAM("flags", flags);
    TRACE_IN_PARAM("hwnd", hwnd);

    TRACE_RETURN(DD_OK);
}

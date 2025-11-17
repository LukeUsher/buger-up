#include "clipper.hpp"
#include "../logger.hpp"

auto DirectDrawClipperImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
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

    if (riid == IID_IUnknown || riid == IID_IDirectDrawClipper) {
        *ppvObject = this;
        AddRef();
    }
    else {
        hr = E_NOINTERFACE;
    }

    TRACE_OUT("ppvObject", ppvObject);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawClipperImpl::AddRef() -> ULONG {
    TRACE_FUNC("ddraw");

    refCount++;
    TRACE_RET("ddraw", refCount);
    return refCount;
}

auto DirectDrawClipperImpl::Release() -> ULONG {
    TRACE_FUNC("ddraw");

    refCount--;
    ULONG rc = refCount;

    TRACE_RET("ddraw", rc);

    if (rc == 0)
        delete this;

    return rc;
}

auto DirectDrawClipperImpl::Create(DWORD flags, IDirectDrawClipper** outClipper) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("flags", flags);
    TRACE_IN("outClipper", outClipper);

    HRESULT hr = DD_OK;

    if (!outClipper) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    DirectDrawClipperImpl* p = new (std::nothrow) DirectDrawClipperImpl();
    if (!p) {
        hr = DDERR_OUTOFMEMORY;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    p->caps = flags;
    *outClipper = p;

    TRACE_OUT("outClipper", outClipper);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawClipperImpl::GetClipList(LPRECT lpRect, LPRGNDATA lpRgnData, LPDWORD lpSize) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpRect", lpRect);
    TRACE_IN("lpRgnData", lpRgnData);
    TRACE_IN("lpSize", lpSize);

    HRESULT hr = DD_OK;

    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawClipperImpl::GetHWnd(HWND* outHwnd) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("outHwnd", outHwnd);

    HRESULT hr = DD_OK;

    if (!outHwnd) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    *outHwnd = nullptr;

    TRACE_OUT("outHwnd", outHwnd);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawClipperImpl::Initialize(LPDIRECTDRAW lpDD, DWORD flags) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpDD", lpDD);
    TRACE_IN("flags", flags);

    caps = flags;

    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawClipperImpl::IsClipListChanged(BOOL* outChanged) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("outChanged", outChanged);

    HRESULT hr = DD_OK;

    if (!outChanged) {
        hr = DDERR_INVALIDPARAMS;
        TRACE_RET("ddraw", hr);
        return hr;
    }

    *outChanged = FALSE;

    TRACE_OUT("outChanged", outChanged);
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawClipperImpl::SetClipList(LPRGNDATA lpRgnData, DWORD flags) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("lpRgnData", lpRgnData);
    TRACE_IN("flags", flags);

    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

auto DirectDrawClipperImpl::SetHWnd(DWORD flags, HWND hwnd) -> HRESULT {
    TRACE_FUNC("ddraw");

    TRACE_IN("flags", flags);
    TRACE_IN("hwnd", hwnd);

    HRESULT hr = DD_OK;
    TRACE_RET("ddraw", hr);
    return hr;
}

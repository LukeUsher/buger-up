#include "clipper.hpp"

auto DirectDrawClipperImpl::QueryInterface(REFIID riid, void** ppvObject) -> HRESULT {
    if (!ppvObject) return E_POINTER;
    *ppvObject = nullptr;
    if (riid == IID_IUnknown || riid == IID_IDirectDrawClipper) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

auto DirectDrawClipperImpl::AddRef() -> ULONG {
    return ++refCount;
}

auto DirectDrawClipperImpl::Release() -> ULONG {
    ULONG count = --refCount;
    if (count == 0) delete this;
    return count;
}

auto DirectDrawClipperImpl::Create(DWORD flags, IDirectDrawClipper** outClipper) -> HRESULT {
    if (!outClipper) return DDERR_INVALIDPARAMS;

    DirectDrawClipperImpl* p = new (std::nothrow) DirectDrawClipperImpl();
    if (!p) return DDERR_OUTOFMEMORY;

    p->caps = flags;
    *outClipper = p;
    return DD_OK;
}

auto DirectDrawClipperImpl::GetClipList(LPRECT, LPRGNDATA, LPDWORD) -> HRESULT {
    std::cout << "[DirectDrawClipperImpl] GetClipList stub\n";
    return DD_OK;
}

auto DirectDrawClipperImpl::GetHWnd(HWND* outHwnd) -> HRESULT {
    if (!outHwnd) return DDERR_INVALIDPARAMS;
    *outHwnd = nullptr;
    std::cout << "[DirectDrawClipperImpl] GetHWnd stub\n";
    return DD_OK;
}

auto DirectDrawClipperImpl::Initialize(LPDIRECTDRAW, DWORD flags) -> HRESULT {
    caps = flags;
    std::cout << "[DirectDrawClipperImpl] Initialize stub\n";
    return DD_OK;
}

auto DirectDrawClipperImpl::IsClipListChanged(BOOL* outChanged) -> HRESULT {
    if (!outChanged) return DDERR_INVALIDPARAMS;
    *outChanged = FALSE;
    std::cout << "[DirectDrawClipperImpl] IsClipListChanged stub\n";
    return DD_OK;
}

auto DirectDrawClipperImpl::SetClipList(LPRGNDATA, DWORD) -> HRESULT {
    std::cout << "[DirectDrawClipperImpl] SetClipList stub\n";
    return DD_OK;
}

auto DirectDrawClipperImpl::SetHWnd(DWORD, HWND) -> HRESULT {
    std::cout << "[DirectDrawClipperImpl] SetHWnd stub\n";
    return DD_OK;
}

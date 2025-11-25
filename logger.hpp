#pragma once
#include <cstdio>
#include <cstdint>
#include <windows.h>
#include <type_traits>
#include <ddraw.h>

#if _DEBUG

inline void _log_timestamp() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    printf("[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}

inline void log_value(const void* p) { printf("%p", p); }
inline void log_value(void* p) { printf("%p", p); }

template<typename T>
inline std::enable_if_t<std::is_integral_v<T>&& std::is_signed_v<T>>
log_value(T v) {
    printf("%lld", (long long)v);
}

template<typename T>
inline std::enable_if_t<std::is_integral_v<T>&& std::is_unsigned_v<T>>
log_value(T v) {
    printf("%llu", (unsigned long long)v);
}

template<typename T>
inline std::enable_if_t<std::is_function_v<std::remove_pointer_t<T>>>
log_value(T p) {
    printf("funcptr=%p", (void*)p);
}

inline void log_value(HRESULT hr) {
    printf("HRESULT=0x%08X", (unsigned)hr);
}

inline void log_value(DWORD v) {
    printf("0x%08X", (unsigned)v);
}

inline void log_value(const char* s) {
    printf("\"%s\"", s ? s : "(null)");
}

inline void log_value(const DDSURFACEDESC* ddsd)
{
    if (!ddsd) {
        printf("DDSURFACEDESC=null");
        return;
    }

    printf("DDSURFACEDESC{ ");
    printf("w=%lu h=%lu ", ddsd->dwWidth, ddsd->dwHeight);

    printf("flags=0x%08lX ", ddsd->dwFlags);
    printf("caps=0x%08lX ", ddsd->ddsCaps.dwCaps);

    printf("bpp=%lu ", ddsd->ddpfPixelFormat.dwRGBBitCount);

    printf("}");
}

template<typename T>
inline void log_param(const char* name, const T& value) {
    printf("        %-12s = ", name);
    log_value(value);
    printf("\n");
}

template<typename T>
inline std::enable_if_t<!std::is_pointer_v<T> &&
    !std::is_integral_v<T> &&
    !std::is_enum_v<T> &&
    !std::is_function_v<T>>
    log_value(const T& v) {
    printf("{object %s size=%zu}", typeid(T).name(), sizeof(T));
}

inline bool is_pointer_readable(const void* p)
{
    MEMORY_BASIC_INFORMATION mbi{};
    if (VirtualQuery(p, &mbi, sizeof(mbi)) != sizeof(mbi))
        return false;

    DWORD protect = mbi.Protect & 0xFF; // mask off guard flags

    if (protect == PAGE_NOACCESS || protect == PAGE_GUARD)
        return false;

    return true;
}


struct LogScope {
    const char* func;
    const char* module;
    bool printedOut = false;

    LogScope(const char* f, const char* m)
        : func(f), module(m)
    {
        _log_timestamp();
        printf("[%s] %s(\n", module, func);
        printf("    IN:\n");
    }

    void ensure_out_header() {
        if (!printedOut) {
            printf("    OUT:\n");
            printedOut = true;
        }
    }
};

#define TRACE_FUNCTION_ENTRY(mod)   LogScope _logscope(__FUNCTION__, mod)
#define TRACE_FUNCTION_ENTRY_STUB(mod)   LogScope _logscope((std::string(__FUNCTION__) + " [UNIMPLEMENTED]").c_str(), mod)
#define TRACE_IN_PARAM(val)  log_param(#val, val)

#define TRACE_OUT_PARAM(val)                                                 \
    do {                                                                     \
        _logscope.ensure_out_header();                                       \
        log_param(#val, val);                                                \
    } while(0)

#define TRACE_RETURN(val) \
    do { \
        printf(") = "); \
        log_value(val); \
        printf("\n\n"); \
        return val; \
    } while (0)


#else

#define TRACE_FUNCTION_ENTRY(mod)
#define TRACE_FUNCTION_ENTRY_STUB(mod)
#define TRACE_IN_PARAM(val)
#define TRACE_OUT_PARAM(val)
#define TRACE_RETURN(val) return val

#endif
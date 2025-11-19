#include <windows.h>
#include "logger.hpp"

extern "C" {
    #pragma comment(linker,"/export:WinGBitBlt=_WinGBitBlt@32,@1")
    #pragma comment(linker,"/export:WinGCreateBitmap=_WinGCreateBitmap@12,@2")
    #pragma comment(linker,"/export:WinGCreateDC=_WinGCreateDC@0,@3")
    #pragma comment(linker,"/export:WinGCreateHalftoneBrush=_WinGCreateHalftoneBrush@12,@4")
    #pragma comment(linker,"/export:WinGCreateHalftonePalette=_WinGCreateHalftonePalette@0,@5")
    #pragma comment(linker,"/export:WinGGetDIBColorTable=_WinGGetDIBColorTable@16,@6")
    #pragma comment(linker,"/export:WinGGetDIBPointer=_WinGGetDIBPointer@8,@7")
    #pragma comment(linker,"/export:WinGRecommendDIBFormat=_WinGRecommendDIBFormat@4,@8")
    #pragma comment(linker,"/export:WinGSetDIBColorTable=_WinGSetDIBColorTable@16,@9")
    #pragma comment(linker,"/export:WinGStretchBlt=_WinGStretchBlt@40,@10")

    __declspec(dllexport) auto __stdcall WinGBitBlt(HDC hdcDst, INT xDst, INT yDst, INT width, INT height, HDC hdcSrc, INT xSrc, INT ySrc) -> BOOL {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_IN_PARAM("hdcDst", hdcDst);
        TRACE_IN_PARAM("xDst", xDst);
        TRACE_IN_PARAM("yDst", yDst);
        TRACE_IN_PARAM("width", width);
        TRACE_IN_PARAM("height", height);
        TRACE_IN_PARAM("hdcSrc", hdcSrc);
        TRACE_IN_PARAM("xSrc", xSrc);
        TRACE_IN_PARAM("ySrc", ySrc);

        TRACE_RETURN(BitBlt(hdcDst, xDst, yDst, width, height, hdcSrc, xSrc, ySrc, SRCCOPY));
    }

    __declspec(dllexport) auto __stdcall WinGCreateBitmap(HDC hdc, BITMAPINFO* bmi, void** bits) -> HBITMAP {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_IN_PARAM("hdc", hdc);
        TRACE_IN_PARAM("bmi", bmi);
        TRACE_IN_PARAM("bits", bits);

        auto r = CreateDIBSection(hdc, bmi, DIB_RGB_COLORS, bits, 0, 0);

        TRACE_OUT_PARAM("bits", bits);
        TRACE_RETURN(r);
    }

    __declspec(dllexport) HDC WINAPI WinGCreateDC() {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_RETURN(CreateCompatibleDC(NULL));
    }

    __declspec(dllexport) HBRUSH WINAPI WinGCreateHalftoneBrush(HDC hdc, COLORREF color, INT type) {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_IN_PARAM("hdc", hdc);
        TRACE_IN_PARAM("color", color);
        TRACE_IN_PARAM("type", type);
        TRACE_RETURN(CreateSolidBrush(color));
    }
    
    __declspec(dllexport) HPALETTE WINAPI WinGCreateHalftonePalette() {
        TRACE_FUNCTION_ENTRY("wing32");

        auto hdc = GetDC(0);
        auto hPal = CreateHalftonePalette(hdc);
        ReleaseDC(0, hdc);

        TRACE_RETURN(hPal);
    }

    __declspec(dllexport) UINT WINAPI WinGGetDIBColorTable(HDC hdc, UINT start, UINT end, RGBQUAD* colors) {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_IN_PARAM("hdc", hdc);
        TRACE_IN_PARAM("start", start);
        TRACE_IN_PARAM("end", end);
        TRACE_IN_PARAM("colors", colors);

        auto r = GetDIBColorTable(hdc, start, end, colors);

        TRACE_OUT_PARAM("colors", colors);
        TRACE_RETURN(r);
    }

    __declspec(dllexport) LPVOID WINAPI WinGGetDIBPointer(HBITMAP hbmp, BITMAPINFO* bmi) {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_IN_PARAM("hbmp", hbmp);
        TRACE_IN_PARAM("bmi", bmi);

        DIBSECTION ds;
        LPVOID r = nullptr;

        if (GetObjectW(hbmp, sizeof(ds), &ds) == sizeof(ds)) {
            if (bmi) bmi->bmiHeader = ds.dsBmih;
            r = ds.dsBm.bmBits;
        }

        TRACE_OUT_PARAM("bmi", bmi);
        TRACE_RETURN(r);
    }

    __declspec(dllexport) BOOL WINAPI WinGRecommendDIBFormat(BITMAPINFO* bmi) {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_IN_PARAM("bmi", bmi);

        BOOL r = FALSE;

        if (bmi) {
            bmi->bmiHeader = {};
            bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi->bmiHeader.biWidth = 320;
            bmi->bmiHeader.biHeight = 1;
            bmi->bmiHeader.biPlanes = 1;
            bmi->bmiHeader.biBitCount = 8;
            bmi->bmiHeader.biCompression = BI_RGB;
            bmi->bmiHeader.biClrImportant = 0;
            r = TRUE;
        }

        TRACE_OUT_PARAM("bmi", bmi);
        TRACE_RETURN(r);
    }
    
    __declspec(dllexport) UINT WINAPI WinGSetDIBColorTable(HDC hdc, UINT start, UINT end, RGBQUAD* colors) {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_IN_PARAM("hdc", hdc);
        TRACE_IN_PARAM("start", start);
        TRACE_IN_PARAM("end", end);
        TRACE_IN_PARAM("colors", colors);

        auto r = SetDIBColorTable(hdc, start, end, colors);

        TRACE_RETURN(r);
    }

    __declspec(dllexport) BOOL WINAPI WinGStretchBlt(HDC hdcDst, INT xDst, INT yDst, INT widthDst, INT heightDst, HDC hdcSrc, INT xSrc, INT ySrc, INT widthSrc, INT heightSrc) {
        TRACE_FUNCTION_ENTRY("wing32");
        TRACE_IN_PARAM("hdcDst", hdcDst);
        TRACE_IN_PARAM("xDst", xDst);
        TRACE_IN_PARAM("yDst", yDst);
        TRACE_IN_PARAM("widthDst", widthDst);
        TRACE_IN_PARAM("heightDst", heightDst);
        TRACE_IN_PARAM("hdcSrc", hdcSrc);
        TRACE_IN_PARAM("xSrc", xSrc);
        TRACE_IN_PARAM("ySrc", ySrc);
        TRACE_IN_PARAM("widthSrc", widthSrc);
        TRACE_IN_PARAM("heightSrc", heightSrc);

        int prev = SetStretchBltMode(hdcDst, COLORONCOLOR);
        auto r = StretchBlt(hdcDst, xDst, yDst, widthDst, heightDst, hdcSrc, xSrc, ySrc, widthSrc, heightSrc, SRCCOPY);
        SetStretchBltMode(hdcDst, prev);
        TRACE_RETURN(r);
    }
}

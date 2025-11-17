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

    __declspec(dllexport) BOOL WINAPI WinGBitBlt(HDC hdcDst, INT xDst, INT yDst, INT width, INT height, HDC hdcSrc, INT xSrc, INT ySrc) {
        TRACE_FUNC("wing");
        TRACE_IN("hdcDst", hdcDst);
        TRACE_IN("xDst", xDst);
        TRACE_IN("yDst", yDst);
        TRACE_IN("width", width);
        TRACE_IN("height", height);
        TRACE_IN("hdcSrc", hdcSrc);
        TRACE_IN("xSrc", xSrc);
        TRACE_IN("ySrc", ySrc);

        BOOL r = BitBlt(hdcDst, xDst, yDst, width, height, hdcSrc, xSrc, ySrc, SRCCOPY);
        TRACE_RET("wing", r);
        return r;
    }

    __declspec(dllexport) HBITMAP WINAPI WinGCreateBitmap(HDC hdc, BITMAPINFO* bmi, void** bits) {
        TRACE_FUNC("wing");
        TRACE_IN("hdc", hdc);
        TRACE_IN("bmi", bmi);
        TRACE_IN("bits", bits);

        HBITMAP r = CreateDIBSection(hdc, bmi, DIB_RGB_COLORS, bits, 0, 0);

        TRACE_OUT("bits", bits);
        TRACE_RET("wing", r);
        return r;
    }

    __declspec(dllexport) HDC WINAPI WinGCreateDC() {
        TRACE_FUNC("wing");

        HDC r = CreateCompatibleDC(NULL);

        TRACE_RET("wing", r);
        return r;
    }

    __declspec(dllexport) HBRUSH WINAPI WinGCreateHalftoneBrush(HDC hdc, COLORREF color, INT type) {
        TRACE_FUNC("wing");
        TRACE_IN("hdc", hdc);
        TRACE_IN("color", color);
        TRACE_IN("type", type);

        HBRUSH r = CreateSolidBrush(color);

        TRACE_RET("wing", r);
        return r;
    }
    
    __declspec(dllexport) HPALETTE WINAPI WinGCreateHalftonePalette() {
        TRACE_FUNC("wing");

        HDC hdc = GetDC(0);
        HPALETTE hPal = CreateHalftonePalette(hdc);
        ReleaseDC(0, hdc);

        TRACE_RET("wing", hPal);
        return hPal;
    }

    __declspec(dllexport) UINT WINAPI WinGGetDIBColorTable(HDC hdc, UINT start, UINT end, RGBQUAD* colors) {
        TRACE_FUNC("wing");
        TRACE_IN("hdc", hdc);
        TRACE_IN("start", start);
        TRACE_IN("end", end);
        TRACE_IN("colors", colors);

        UINT r = GetDIBColorTable(hdc, start, end, colors);

        TRACE_OUT("colors", colors);
        TRACE_RET("wing", r);
        return r;
    }

    __declspec(dllexport) LPVOID WINAPI WinGGetDIBPointer(HBITMAP hbmp, BITMAPINFO* bmi) {
        TRACE_FUNC("wing");
        TRACE_IN("hbmp", hbmp);
        TRACE_IN("bmi", bmi);

        DIBSECTION ds;
        LPVOID r = nullptr;

        if (GetObjectW(hbmp, sizeof(ds), &ds) == sizeof(ds)) {
            if (bmi)
                bmi->bmiHeader = ds.dsBmih;

            r = ds.dsBm.bmBits;
        }

        TRACE_OUT("bmi", bmi);
        TRACE_RET("wing", r);
        return r;
    }

    __declspec(dllexport) BOOL WINAPI WinGRecommendDIBFormat(BITMAPINFO* bmi) {
        TRACE_FUNC("wing");
        TRACE_IN("bmi", bmi);

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

        TRACE_OUT("bmi", bmi);
        TRACE_RET("wing", r);
        return r;
    }
    
    __declspec(dllexport) UINT WINAPI WinGSetDIBColorTable(HDC hdc, UINT start, UINT end, RGBQUAD* colors) {
        TRACE_FUNC("wing");
        TRACE_IN("hdc", hdc);
        TRACE_IN("start", start);
        TRACE_IN("end", end);
        TRACE_IN("colors", colors);

        UINT r = SetDIBColorTable(hdc, start, end, colors);

        TRACE_RET("wing", r);
        return r;
    }

    __declspec(dllexport) BOOL WINAPI WinGStretchBlt(HDC hdcDst, INT xDst, INT yDst, INT widthDst, INT heightDst, HDC hdcSrc, INT xSrc, INT ySrc, INT widthSrc, INT heightSrc) {
        TRACE_FUNC("wing");
        TRACE_IN("hdcDst", hdcDst);
        TRACE_IN("xDst", xDst);
        TRACE_IN("yDst", yDst);
        TRACE_IN("widthDst", widthDst);
        TRACE_IN("heightDst", heightDst);
        TRACE_IN("hdcSrc", hdcSrc);
        TRACE_IN("xSrc", xSrc);
        TRACE_IN("ySrc", ySrc);
        TRACE_IN("widthSrc", widthSrc);
        TRACE_IN("heightSrc", heightSrc);

        int prev = SetStretchBltMode(hdcDst, COLORONCOLOR);
        BOOL r = StretchBlt(hdcDst, xDst, yDst,
            widthDst, heightDst,
            hdcSrc, xSrc, ySrc,
            widthSrc, heightSrc,
            SRCCOPY);
        SetStretchBltMode(hdcDst, prev);

        TRACE_RET("wing", r);
        return r;
    }
}

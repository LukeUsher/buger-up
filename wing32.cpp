#include <windows.h>


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

    __declspec(dllexport) auto WINAPI WinGBitBlt(HDC hdcDst, INT xDst, INT yDst, INT width, INT height, HDC hdcSrc, INT xSrc, INT ySrc) -> BOOL {
        return BitBlt(hdcDst, xDst, yDst, width, height, hdcSrc, xSrc, ySrc, SRCCOPY);
    }

    __declspec(dllexport) auto WINAPI WinGCreateBitmap(HDC hdc, BITMAPINFO* bmi, void** bits) -> HBITMAP {
        return CreateDIBSection(hdc, bmi, DIB_RGB_COLORS, bits, 0, 0);
    }

    __declspec(dllexport) auto WINAPI WinGCreateDC() -> HDC {       
        return CreateCompatibleDC(NULL);   
    }

    __declspec(dllexport) auto WINAPI WinGCreateHalftoneBrush(HDC hdc, COLORREF color, INT type) -> HBRUSH {
        return CreateSolidBrush(color);
    }

    __declspec(dllexport) auto WINAPI WinGCreateHalftonePalette() -> HPALETTE {
        auto hDC = GetDC(0);
        auto hPal = CreateHalftonePalette(hDC);
        ReleaseDC(0, hDC);
        return hPal;
    }

    __declspec(dllexport) auto WINAPI WinGGetDIBColorTable(HDC hdc, UINT start, UINT end, RGBQUAD* colors) -> UINT {
        return GetDIBColorTable(hdc, start, end, colors);
    }

    __declspec(dllexport) auto WINAPI WinGGetDIBPointer(HBITMAP hbmp, BITMAPINFO* bmi) -> LPVOID {
        DIBSECTION ds;

        if (GetObjectW(hbmp, sizeof(ds), &ds) == sizeof(ds)) {
            if (bmi) bmi->bmiHeader = ds.dsBmih;
            return ds.dsBm.bmBits;
        }

        return NULL;
    }

    __declspec(dllexport) auto WINAPI WinGRecommendDIBFormat(BITMAPINFO* bmi) -> BOOL {
        if (!bmi) return FALSE;

        bmi->bmiHeader = {};
        bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi->bmiHeader.biWidth = 320;
        bmi->bmiHeader.biHeight = 1;
        bmi->bmiHeader.biPlanes = 1;
        bmi->bmiHeader.biBitCount = 8;
        bmi->bmiHeader.biCompression = BI_RGB;
        bmi->bmiHeader.biClrImportant = 0;
        return TRUE;
    }

    __declspec(dllexport) auto WINAPI WinGSetDIBColorTable(HDC hdc, UINT start, UINT end, RGBQUAD* colors) -> UINT {
        return SetDIBColorTable(hdc, start, end, colors);
    }

    __declspec(dllexport) auto WINAPI WinGStretchBlt(HDC hdcDst, INT xDst, INT yDst, INT widthDst, INT heightDst, HDC hdcSrc, INT xSrc, INT ySrc, INT widthSrc, INT heightSrc) -> BOOL {
        auto previousMode = SetStretchBltMode(hdcDst, COLORONCOLOR);
        auto result = StretchBlt(hdcDst, xDst, yDst, widthDst, heightDst, hdcSrc, xSrc, ySrc, widthSrc, heightSrc, SRCCOPY);
        SetStretchBltMode(hdcDst, previousMode);
        return result;
    }
}

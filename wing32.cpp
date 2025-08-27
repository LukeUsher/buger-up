#if BUILD_WING32

#include <windows.h>


extern "C" {

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

#endif

struct Ecco : Game
{
    using GetPrivateProfileStringA_t = DWORD(WINAPI*)(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName);
    static inline GetPrivateProfileStringA_t _GetPrivateProfileStringA = nullptr;

    auto applyPatches(std::string hash) -> bool override {
        //No-CD: Hook GetPrivateProfileStringA to Set the CDPath to the games directory
        patchEngine.PatchImportedFunction("kernel32.dll", "GetPrivateProfileStringA", (void*)GetPrivateProfileStringA_Hook, (void**)&_GetPrivateProfileStringA);

        //No-CD: Skip error message when data files are detected in a location that is not a CD-ROM drive
        patchEngine.InjectJump(0x004070E8, 0x00407110);

        //Prevent direct hardware IO during VBlank wait
        patchEngine.InjectJump(0x0040A205, 0x0040A242);
        patchEngine.InjectJump(0x0040A237, 0x0040A242);
        return true;
    }


    static auto WINAPI GetPrivateProfileStringA_Hook(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName) -> DWORD {
        if (lpKeyName && std::string(lpKeyName) == "CDPath") {
            auto path = GetCurrentGameInstallDirectory() + std::string("\\DATA");
            if (lpReturnedString && nSize > 0) {
                auto copySize = std::min(path.size(), static_cast<std::size_t>(nSize - 1));
                std::copy(path.begin(), path.begin() + copySize, lpReturnedString);
                lpReturnedString[copySize] = '\0';
            }
            return static_cast<DWORD>(path.size());
        }

        return _GetPrivateProfileStringA(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
    }
};
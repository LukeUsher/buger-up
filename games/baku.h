#pragma once

struct Baku : Game
{
	static inline decltype(&GetDeviceCaps) _GetDeviceCaps = nullptr;

	auto applyPatches(std::string hash) -> bool override {
		//Remove hard-coded CD check
		patchEngine.PatchFunction("IsGameDiscInserted", 0x004016F0, IsGameDiscInserted_Hook);

		//GetDeviceCaps must return 256 colors
		patchEngine.PatchImportedFunction("gdi32.dll", "GetDeviceCaps", GetDeviceCaps_Hook, (void**)& _GetDeviceCaps);
		return true;
	}

	static auto __cdecl IsDisplayModeCompatible_Hook() -> int {
		return 1;
	}

	static auto __cdecl IsGameDiscInserted_Hook() -> int {
		return 1;
	}

	static auto __stdcall GetDeviceCaps_Hook(HDC hdc, INT index) -> int {
		auto hostCaps = _GetDeviceCaps(hdc, index);

		if (index == SIZEPALETTE) {
			return 256;
		}

		if (index == RASTERCAPS) {
			return hostCaps | RC_PALETTE;
		}
		
		return _GetDeviceCaps(hdc, index);
	}
};

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <Windows.h>
#include "thirdparty/subhook/subhook.h"

class PatchEngine {
public:
	template<typename T> auto PatchBinary(uintptr_t target, T value) -> void {
		DWORD oldProtect, tmp;
		VirtualProtect(reinterpret_cast<void*>(target), sizeof(T), PAGE_READWRITE, &oldProtect);
		*reinterpret_cast<T*>(target) = value;
		VirtualProtect(reinterpret_cast<void*>(target), sizeof(T), oldProtect, &tmp);
	}

	auto PatchBinary(uintptr_t target, std::vector<uint8_t> bytes) -> void {
		DWORD oldProtect, tmp;
		VirtualProtect(reinterpret_cast<void*>(target), bytes.size(), PAGE_READWRITE, &oldProtect);
		memcpy(reinterpret_cast<void*>(target), bytes.data(), bytes.size());
		VirtualProtect(reinterpret_cast<void*>(target), bytes.size(), oldProtect, &tmp);
	}

	auto PatchFunction(const std::string& name, uintptr_t target, void* detour) -> bool;
	auto PatchImportedFunction(const std::string& moduleName, const std::string& function, void* detour, void** orgiginalFunctionPtr) -> bool;
	auto PatchImportedFunction(const std::string& moduleName, const std::string& function, void* detour) -> bool;
	auto InjectCall(uintptr_t target, void* detour) -> void;
	auto InjectJump(uintptr_t target, void* detour) -> void;
	auto InjectJump(uintptr_t target, uintptr_t detour) -> void;
	auto GetOriginalFunctionPointer(const std::string& name) -> void*;
	auto LocateFunction(const std::string& library, const std::string function) -> uintptr_t;
private:
	std::unordered_map<std::string, std::shared_ptr<subhook::Hook>> _hooks;
};

extern PatchEngine patchEngine;

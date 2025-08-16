#include "patch-engine.h"
#include "game-manager.h"

PatchEngine patchEngine;

auto PatchEngine::PatchFunction(const std::string& name, uintptr_t target, void* detour) -> bool
{
	auto hook = std::make_shared<subhook::Hook>();
	if (!hook->Install((void*)target, detour, subhook::HookFlags::HookFlagTrampoline))
	{
		printf("PatchEngine: failed to hook function %s\n", name.c_str());
		return false;
	}
	_hooks[name] = hook;
	printf("PatchEngine: hooked %s\n", name.c_str());
	return true;
}

auto PatchEngine::PatchImportedFunction(const std::string& moduleName, const std::string& function, void* detour, void** originalFunctionPtr) -> bool
{
	auto address = patchEngine.LocateFunction(moduleName, function);
	if (!address) {
		printf("PatchEngine: Could not locate imported function %s in module %s\n", function.c_str(), moduleName.c_str());
		return false;
	}

	auto hookName = moduleName + ":" + function;
	if (!patchEngine.PatchFunction(hookName, address, detour)) {
		printf("PatchEngine: failed to hook function %s in module %s\n", function.c_str(), moduleName.c_str());
		return false;
	}

	if (originalFunctionPtr != nullptr) {
		*originalFunctionPtr = GetOriginalFunctionPointer(hookName);
	}
	return true;
}

auto PatchEngine::PatchImportedFunction(const std::string& moduleName, const std::string& function, void* detour) -> bool
{
	return PatchImportedFunction(moduleName, function, detour, nullptr);
}

auto PatchEngine::InjectCall(uintptr_t target, void* detour) -> void
{
	DWORD oldProtect, tmp;
	VirtualProtect((void*)target, 5, PAGE_READWRITE, &oldProtect);
	uint8_t* FuncBytes = (uint8_t*)target;
	FuncBytes[0] = 0xE8;
	*(uint32_t*)&FuncBytes[1] = (uint32_t)detour - (uint32_t)target - 5;
	VirtualProtect((void*)target, 5, oldProtect, &tmp);
}

auto PatchEngine::InjectJump(uintptr_t target, void* detour) -> void
{
	DWORD oldProtect, tmp;
	VirtualProtect((void*)target, 5, PAGE_READWRITE, &oldProtect);
	uint8_t* FuncBytes = (uint8_t*)target;
	FuncBytes[0] = 0xE9;
	*(uint32_t*)&FuncBytes[1] = (uint32_t)detour - (uint32_t)target - 5;
	VirtualProtect((void*)target, 5, oldProtect, &tmp);
}

auto PatchEngine::InjectJump(uintptr_t target, uintptr_t detour) -> void
{
	DWORD oldProtect, tmp;
	VirtualProtect((void*)target, 5, PAGE_READWRITE, &oldProtect);
	uint8_t* FuncBytes = (uint8_t*)target;
	FuncBytes[0] = 0xE9;
	*(uint32_t*)&FuncBytes[1] = (uint32_t)detour - (uint32_t)target - 5;
	VirtualProtect((void*)target, 5, oldProtect, &tmp);
}

auto PatchEngine::GetOriginalFunctionPointer(const std::string& name) -> void*
{
	auto hook = _hooks.find(name);
	if (hook == _hooks.end()) return nullptr;
	return hook->second->GetTrampoline();
}

auto PatchEngine::LocateFunction(const std::string& library, const std::string function) -> uintptr_t
{
	HMODULE hModule = GetModuleHandleA(library.c_str());
	if (hModule == NULL) {
		hModule = LoadLibraryA(library.c_str());
		if (hModule == NULL) return 0;
	}

	void* pFunction = GetProcAddress(hModule, function.c_str());
	if (pFunction == NULL) {
		return 0;
	}

	return (uintptr_t)pFunction;
}

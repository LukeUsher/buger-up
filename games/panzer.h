struct Panzer : Game
{
    auto applyPatches(std::string hash) -> bool override {
        patchEngine.PatchFunction("IsCDPresent", 0x0041A6E0, IsCDPresent_Hook);
        return true;
    }

    static auto __cdecl IsCDPresent_Hook() -> int {
        return 1;
	}
};

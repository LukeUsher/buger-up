struct ComixZone : Game
{
	auto applyPatches(std::string hash) -> bool override {
		//TODO: Fix full-screen; other than that we seem to get away with only generic patches!
		return true;
	}
};
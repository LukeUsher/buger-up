#include "cd-player.hpp" 

static constexpr uint32_t sector_size = 2352;
static constexpr uint32_t pregap_sectors = 150; // ~2s lead-in

CdPlayer cdPlayer;

auto CdPlayer::open(uint32_t dataTrackSectors) -> bool {
	std::lock_guard<std::mutex> lock(_mutex);

	if (!SDL_Init(SDL_INIT_AUDIO)) {
		return false;
	}

	SDL_AudioSpec spec;
	spec.freq = 44100;
	spec.format = SDL_AUDIO_S16LE;
	spec.channels = 2;
	_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
	if (!_stream) {
		return false;
	}

	_tracks.clear(); 
	
	CdTrack data; 
	data.filename = ""; 
	data.startSector = pregap_sectors; 
	data.endSector = data.startSector + dataTrackSectors - 1; 
	data.isData = true; 
	data.number = 1; 
	_tracks.push_back(data); 
	
	WIN32_FIND_DATAA fd; 
	HANDLE hFind = FindFirstFileA("*.bin", &fd); 
	
	if (hFind == INVALID_HANDLE_VALUE) return false; 
	
	std::vector<std::string> files;
	do { 
		files.push_back(fd.cFileName); 
	} while (FindNextFileA(hFind, &fd)); 
	FindClose(hFind); 
	
	std::sort(files.begin(), files.end()); 
	
	uint32_t sector_offset = data.endSector + 1; 
	for (auto& f : files) { 
		size_t pos1 = f.find("(Track ");
		size_t pos2 = f.find(").bin"); 

		if (pos1 == std::string::npos || pos2 == std::string::npos) continue; 
		std::string numStr = f.substr(pos1 + 7, pos2 - (pos1 + 7));

		int trackNum = atoi(numStr.c_str()); 
		if (trackNum < 2) continue; 
		
		std::ifstream file(f, std::ios::binary | std::ios::ate); 
		if (!file.is_open()) continue; 
		
		uint64_t size = file.tellg(); 
		uint32_t sectors = (uint32_t)(size / sector_size); 
		
		CdTrack t; 
		t.filename = f; 
		t.startSector = sector_offset; 
		t.endSector = sector_offset + sectors - 1; 
		t.isData = false; 
		t.number = trackNum; 

		sector_offset += sectors;
		_tracks.push_back(t); 
	} 
	
	_thread = std::thread(&CdPlayer::playbackThread, this); 
	_terminating = false;

	SDL_ResumeAudioStreamDevice(_stream);
	return _tracks.size() > 1;
} 

auto CdPlayer::close() -> void { 
	stop();
	_terminating = true; 
	if (_thread.joinable()) _thread.join();
	_tracks.clear();
	SDL_DestroyAudioStream(_stream);
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
} 

auto CdPlayer::playTrack(int track, bool loop) -> void { 
	SDL_ClearAudioStream(_stream);
	if (track < 1 || track >(int)_tracks.size()) return;
	auto& t = _tracks[track - 1]; 
	playSectors(t.startSector, t.endSector, loop); 
} 

auto CdPlayer::trackCount() const -> int { 
	return (int)_tracks.size(); 
} 

auto CdPlayer::trackInfo(int track, uint32_t* start, uint32_t* end) -> bool { 
	if (track < 1 || track >(int)_tracks.size()) return false;
	*start = _tracks[track - 1].startSector;
	*end = _tracks[track - 1].endSector;
	return true;
} 

auto CdPlayer::playSectors(uint32_t start, uint32_t end, bool loop) -> void { 
	std::lock_guard<std::mutex> lock(_mutex); 
	if (start >= end) return; 
	
	_currentByte = (uint64_t)start * sector_size; 
	_endByte = (uint64_t)end * sector_size; 

	_loop = loop;
	_position = start; 
	resume();
}

auto CdPlayer::stop() -> void { 
	std::lock_guard<std::mutex> lock(_mutex); 
	SDL_ClearAudioStream(_stream);
	pause();
	_currentByte = 0; 
	_endByte = 0; 
	_position = 0; 
} 

auto CdPlayer::pause() -> void { 
	SDL_PauseAudioStreamDevice(_stream);
} 

auto CdPlayer::resume() -> void { 
	SDL_ResumeAudioStreamDevice(_stream);
} 

auto CdPlayer::position() const -> uint32_t { 
	return _position; 
} 
auto CdPlayer::playbackThread() -> void {
	const size_t chunkSize = sector_size * 16;

	while (!_terminating) {
		if (SDL_AudioStreamDevicePaused(_stream)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			continue;
		}

		CdTrack* currentTrack = nullptr;
		{
			std::lock_guard<std::mutex> lock(_mutex);

			for (auto& t : _tracks) {
				if (_position >= t.startSector && _position <= t.endSector) {
					currentTrack = &t;
					break;
				}
			}

			if (!currentTrack) {
				pause();
				continue;
			}
		}

		if (currentTrack->isData) {
			pause();
			continue;
		}

		uint64_t trackStartByte = (uint64_t)currentTrack->startSector * sector_size;
		uint64_t trackEndByte = (uint64_t)(currentTrack->endSector + 1) * sector_size;
		if (_currentByte < trackStartByte) _currentByte = trackStartByte;

		uint64_t readStart = _currentByte - trackStartByte;
		uint64_t remaining = trackEndByte - _currentByte;
		if (remaining == 0) {
			if (_loop) {
				_currentByte = trackStartByte;
				_position = currentTrack->startSector;
				continue;
			} else {
				pause();
				continue;
			}
		}

		size_t toRead = (size_t)std::min<uint64_t>(remaining, chunkSize);

		std::ifstream file(currentTrack->filename, std::ios::binary);
		if (!file.is_open()) {
			stop();
			continue;
		}

		file.seekg(readStart, std::ios::beg);
		std::vector<char> buffer(toRead);
		file.read(buffer.data(), buffer.size());

		SDL_PutAudioStreamData(_stream, buffer.data(), (int)buffer.size());

		_currentByte += buffer.size();
		_position = (uint32_t)(_currentByte / sector_size);

		int bytesPerSec = 44100 * 2 * 2;
		int ms = (int)((buffer.size() * 1000) / bytesPerSec);
		std::this_thread::sleep_for(std::chrono::milliseconds(ms / 2));
	}
}

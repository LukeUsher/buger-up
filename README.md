# Bug!er Up

**Bug!er Up** is an unofficial patch that fixes issues preventing the game from running on modern Windows, removes the original CD-ROM requirement, and adds support for XInput controllers.

> [!WARNING]  
> This is a quick and dirty weekend project thrown together using code developed for other personal projects that never made it to release. This project is probably a good example of what *not* to do; but it works on my machine; I am not at all proud of the code.

> [!IMPORTANT]  
> This has only been tested on Windows 11 24H2, YMMV on other versions.
---

## Supported Games

- [Bug! (1995, Windows 95)](#bug-1995)
- [Bug Too! (1996, Windows 95)](#bug-too-1996)

---

## Bug! (1995)

### Requirements

- Original *Bug!* disc (U.S. retail release tested).  
  - Redump reference: [http://redump.org/disc/63684/](http://redump.org/disc/63684/)
- Game must be installed from the disc using the original installer (`SETUP95.EXE`).  
- Audio tracks must be ripped in **BIN/CUE format**, with each CD-DA track as a separate `.bin` file (see instructions below).

### Installation Steps

1. **Install the game from the original disc**  
   - Run `SETUP95.EXE` directly from the CD — no compatibility settings needed.

2. **Copy required data files**  
   - Copy the directories `VBFMV` and `VBFMVLO` from the disc into your game installation folder.

3. **Provide audio tracks**  
   - Rip your disc in **BIN/CUE format**, with each audio track saved as its own `.bin` file.  
   - Audio files must be **raw CD-DA** (44,1 kHz, 16-bit, stereo PCM), with no headers or compression.  
   - Place them in the game directory. Filenames must include the pattern `(Track XX).bin`, where `XX` is the track number (for example, `(Track 02).bin`).  
     - Prefixes do not matter; these will be recognised correctly:  
       - `Bug! (USA) (Track 02).bin`  
       - `(Track 02).bin`  
       - `Custom OST (Track 02).bin`  
   - Track 01 (data) is not used and may be omitted.

4. **Apply the patch**  
   - Copy `winmm.dll` from the patch ZIP into the game installation directory.

5. **Play the game**  
   - After setup, the game will run without the disc.  
   - If audio tracks are present, CD-music will play; custom soundtracks are fully supported.

---

## Bug Too! (1996)

> **Note:** Support for *Bug Too!* is planned but not yet supported, but these install instructions are likely to be final

### Requirements

- Original *Bug Too!* disc (U.S. retail release tested).  
  - Redump reference: [http://redump.org/disc/104684/](http://redump.org/disc/104684/)
- Audio tracks must be ripped in **BIN/CUE format**, with each CD-DA track as a separate `.bin` file (see instructions below).

### Installation Steps

The original SETUP.EXE does not function on Modern Windows, so the game will need to be installed manually.

1. **Copy required data files**  
   - `EXEC.RTA`, `DUKDLL95.DL_`, `MSS32.DL_` and `TITLE.BMP` and the `DATA` directory to your desired install directory, we used `C:\SEGA\BUG TOO!\` for consistency with Bug!
   - Rename `EXEC.RTA` to `BUGTOO!.EXE`
   - Rename `DUKDLL95.DL_` to `DUKDLL95.DLL`
   - Rename `MSS32.DL_` to `MSS32.DLL` 

2. **Provide audio tracks**  
   - Rip your disc in **BIN/CUE format**, with each audio track saved as its own `.bin` file.  
   - Audio files must be **raw CD-DA** (44,1 kHz, 16-bit, stereo PCM), with no headers or compression.  
   - Place them in the game directory. Filenames must include the pattern `(Track XX).bin`, where `XX` is the track number (for example, `(Track 02).bin`).  
     - Prefixes do not matter; these will be recognised correctly:  
       - `Bug Too! (USA) (Track 02).bin`  
       - `(Track 02).bin`  
       - `Custom OST (Track 02).bin`  
   - Track 01 (data) is not used and may be omitted.

3. **Apply the patch**  
   - Copy `winmm.dll` from the patch ZIP into the game installation directory.

4. **Play the game**
   - Start the game with BUGTOO!.EXE; on first run, you will be asked if you wish to create desktop and/or start menu shortcuts.
   - After setup, the game will run without the disc.  
   - If audio tracks are present, CD music will play; custom soundtracks are fully supported.
---

## Features

- Fixes compatibility issues that otherwise prevent the game from running on modern Windows.  
- Removes the need for a physical CD during gameplay.  
- Adds fully functional CD-audio emulation using `.bin` files.  
- Supports XInput controllers—configure in-game controller settings to use modern gamepads.

---

## Notes

- Only **separate-track BIN/CUE rips** are supported. Image files, ISOs, or merged BINs will not work.  
- Audio must be **raw CD-DA PCM** format; compressed or container formats (WAV, MP3, FLAC, OGG, etc.) are unsupported.  
- Make sure each track `.bin` file includes `(Track XX).bin` in its name to be recognized.

---

## Disclaimer

This is an **unofficial fan-made project**, unaffiliated with or endorsed by Sega.  
Use of this patch requires owning a legitimate copy of the original *Bug!* or *Bug Too!* disc.

---


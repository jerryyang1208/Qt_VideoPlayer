<div align="right">
  <a href="README.md">中文</a>
</div>

<div align="center">

# 🎬 A Simple Audio Video Player Based on Qt 6

[![Qt](https://img.shields.io/badge/Qt-6.10.2-brightgreen)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C++-17-blue)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.16+-orange)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-yellow)](LICENSE)

</div>

---

## 📖 Introduction

`Qt_VideoPlayer` is a simple audio/video player built with **Qt 6.10.2** and **C++17**. It uses the **CMake** build system and supports seamless development across **Qt Creator**, **VS Code**, and **Trae** IDEs. The project follows an MVC architecture, leveraging Qt's signal-slot mechanism for loose coupling between UI and media playback state.

![Program Screenshot](Resource/program-run.png)

> *This project is primarily for personal learning and sharing design ideas for Qt multimedia development, rather than building a fully-featured daily player to replace mature products like VLC or PotPlayer.*

📚 Detailed tutorial: [Zhihu Article](https://zhuanlan.zhihu.com/p/1929289732564710634)

---

## 📑 Table of Contents

- [Features](#-features)
- [Build & Run](#-build--run)
- [Core Optimizations](#-core-optimizations)
- [Future Plans](#-future-plans)
- [Contact](#-contact)

---

## ✨ Features

### Basic Playback

| Feature | Description |
|---------|-------------|
| 📁 File Scanning | Scan local folders for audio/video files and add them to the playlist |
| ▶️ Playback Control | Play/pause, previous/next track, seek with progress bar drag |
| 🔊 Volume Control | Power-law curve volume mapping for linear perceived loudness |
| ⏱️ Time Display | Real-time display of current playback position and total duration |
| 📋 Playlist | Double-click to play, current track highlight tracking |

### Playback Modes

| Mode | Description |
|------|-------------|
| 🔄 Sequential | Play in list order, auto-advance to next track |
| 🎲 Random | Randomly select next track, with anti-repeat to avoid consecutive same selection |
| 🔂 Repeat One | Restart the current track from the beginning after it finishes |

### Smart Detection

- Automatically detects whether the current file is audio or video
- **Audio**: Video window displays **embedded album art** (via `QMediaMetaData`)
- **Video**: Video window displays video output
- Window close event intercepted as hide, preventing resource overhead from repeated window creation

### Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `M` | Mute / Unmute |
| `F` | Toggle fullscreen for video window |
| `ESC` | Exit fullscreen |

### Multi-IDE Support

Built with CMake, a single codebase can be compiled and run in **Qt Creator**, **VS Code / Trae**, and other IDEs. Only the corresponding IDE configuration files are needed — no core code changes required.

---

## 🛠️ Build & Run

### Requirements

- **Qt 6.10+** (Multimedia, MultimediaWidgets modules)
- **CMake 3.16+**
- **MinGW 64-bit** or **MSVC 2022** compiler
- **Windows 10/11**

### Build Steps

```bash
# 1. Clone the project
git clone https://github.com/yourusername/Qt_VideoPlayer.git
cd Qt_VideoPlayer

# 2. Configure CMake
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=D:/Qt/6.11.1/mingw_64

# 3. Build
cmake --build build --config Debug

# 4. Run
./build/VideoPlayer.exe
```

Or simply open the project folder in **Qt Creator** / **VS Code (CMake Tools extension)** — the IDE will automatically detect the CMake configuration.

---

## 🔧 Core Optimizations

### Architecture

- **MVC architecture**: `QStandardItemModel` manages playlist data, `QListView` handles display, and `VideoPlayer` acts as the controller
- **Signal-slot mechanism** enables loose coupling and event-driven updates between UI and media playback state
- Centralized core logic: `playAtIndex()` as the single playback entry point, `switchSong()` unifying previous/next track switching

### Functional Improvements

- **Random number generation**: Uses `QRandomGenerator::global()->bounded()` instead of C-style `srand/rand` — thread-safe and higher quality
- **Playback completion detection**: Listens to `mediaStatusChanged` signal instead of timer polling, eliminating CPU overhead
- **Error handling**: Connects `errorOccurred` signal, shows dialog to inform users and auto-skips to next available file
- **Volume mapping**: Power-law curve matching human auditory perception, making slider movement feel linear to the ear

### Code Quality

- Redundant code cleanup: Removed unused member variables, unnecessary includes, and pure forwarder slots
- Duplicate logic consolidation: `switchSong`, `currentFileName`, `nextIndex` — three extractions eliminating repetition
- Helper function extraction: `msToTimeString`, `linearToLogVolume`, `currentIndex` — single-responsibility utility functions
- Modernized CMake: `qt_standard_project_setup()` replaces manual setup, dead Qt5 branches removed
- **VideoPlayer.cpp reduced from 513 lines to ~435 lines (~15% reduction), with clear logical layering**

### Stability

- Memory leak fix: Standalone video window fully released in destructor
- Error loop guard: Consecutive error counter prevents infinite loop when all files are corrupt
- State consistency: Stops playback before clearing the playlist when opening a new directory
- Empty path defense: `playAtIndex` validates file path is non-empty
- Defensive programming: Bounds checking, null pointer protection, proper resource release ordering

---

## 🗺️ Future Plans

| Status | Feature | Approach |
|--------|---------|----------|
| ✅ | Keyboard shortcuts | `M` mute, `F` fullscreen toggle, `ESC` exit fullscreen |
| ✅ | Media metadata display | Read embedded album art, display during audio playback |
| ✅ | Video fullscreen toggle | `F` key fullscreen / `ESC` exit, event filter for key interception |
| ⬜ | Playlist persistence | Save playlist to JSON on exit, auto-restore on startup |
| ⬜ | Playback speed control | Use `QMediaPlayer::setPlaybackRate()` for 0.5x-2.0x speed |
| ⬜ | Lyrics display | Parse LRC files, synchronize highlighting with scroll animation |
| ⬜ | Spectrum visualization | FFT on audio data, render bar chart with `QPainter` |
| ⬜ | Equalizer | Multi-band EQ via DSP filtering, adjustable via multiple sliders |
| ⬜ | Play history & favorites | Record play history, favorites support, extend via Model/View |

---

## 📬 Contact

- 📝 Blog: [Zhihu Homepage](https://www.zhihu.com/people/13-73-62-89-19)
- 📧 Email: 2022280099@email.szu.edu.cn

This project will continue to be updated. Issues and suggestions are welcome!

Thank you for your attention and support ✨

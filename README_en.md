<div align="right">
  <a href="README.md">中文</a>
</div>

<div align="center">

# 🎬 A Simple Audio Video Player Based on Qt 6.10.2

![Qt](https://img.shields.io/badge/Qt-6.10.2-brightgreen)
![C++](https://img.shields.io/badge/C++-17-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

</div>

`Qt_VideoPlayer` is a simple audio/video player developed based on Qt 6.10.2 and C++17. Built with the **CMake** build system, it supports seamless development across popular IDEs including **Qt Creator** and **VS Code / Trae**. It aims to provide a clean and user-friendly graphical interface while demonstrating the integration of the Qt framework with multimedia processing libraries. The actual running interface is shown below:

![Program Screenshot](Resource/program-run.png)

> *Note: This project is primarily for personal learning and sharing design ideas for Qt multimedia development, rather than building a fully-featured daily player that can replace mature products like VLC or PotPlayer.*

Detailed tutorial documentation: https://zhuanlan.zhihu.com/p/1929289732564710634

---

## Features

### Basic Functions

- Feature 1: Implement basic player UI layout and standardized naming.
- Feature 2: Read local folder media files into the player list, double-click to play audio or video, supporting common audio/video formats.
- Feature 3: Automatically detect whether the currently playing media is audio or video. When playing audio, the video playback window is hidden by default; only when playing video is the window displayed and the current media rendered to the window.
- Feature 4: Implement button switching logic, including automatic switching after playback (with optional single loop, sequential play, and random play modes), previous/next track button interaction, and highlighting the current playing item in the list.
- Feature 5: Rich button control logic, including play/pause control, volume adjustment, dragging the progress bar to change playback position, and display of total media duration and current playback progress.
- Feature 6: Custom window close event. When closing a playing video window, the play button, audio, progress bar, and time label are synchronized to pause. Clicking the play button again or double-clicking a video file in the list will re-display the window, ensuring the video window appears synchronously with playback operations.

### Multi-IDE Support

The project is built with CMake, allowing a single codebase to be compiled and run in multiple IDEs such as **Qt Creator** and **VS Code / Trae**. Only the corresponding IDE configuration files are needed — no changes to the core code are required.


---

## Core Optimizations

To improve code quality, running efficiency, and user experience, the project has undergone three important optimizations:

### Random Number Generation Optimization
- Original method: Used C-style srand/rand to generate random indexes, requiring manual seed setting and not thread-safe.
- Optimized: Uses Qt's QRandomGenerator::global()->bounded(n), no manual initialization needed.
- Improvement: Simplifies random number usage, thread-safe with higher quality random numbers, code more compliant with Qt standards.

### Playback Completion Detection Optimization
- Original method: Used a timer to poll every second checking if playback position approached total duration (currentPos >= totalDur - 1000), with precision errors and CPU waste.
- Optimized: Listens to QMediaPlayer::mediaStatusChanged signal, triggering automatic track switching immediately when status changes to QMediaPlayer::EndOfMedia.
- Improvement: More precise and reliable detection, eliminates polling overhead, cleaner and more elegant code, compliant with Qt standards.

### Error Handling Optimization
- Original method: When playback failed (e.g., file corruption, unsupported format), the program gave no prompt, leaving users unaware of the problem.
- Optimized: Connects errorOccurred signal, pops up error dialog to inform users, outputs error information to console, and automatically plays the next available file.
- Improvement: Significantly enhances user experience, facilitates debugging, and strengthens program robustness.

### Code Quality & Maintainability Optimization
- Redundant code cleanup: Removed unused member variables (`volumeSlider`, `volumeWidget`, `m_isVideoPlaying`), unnecessary includes (`QTimer`, `<utility>`), and pure forwarder slot (`seekPosition`), reducing code noise.
- Duplicate logic consolidation: Merged two symmetric "previous/next song" implementations into `switchSong(int direction)`; encapsulated 5 repeated filename lookups into `currentFileName()`; unified two separate index calculations into `nextIndex(int direction)`.
- Variable constification: Supported audio/video format lists declared `const`, initialized once in the constructor initializer list, clarifying read-only semantics.
- Helper function extraction: Extracted small utility functions such as `msToTimeString()` for time formatting, `linearToLogVolume()` for logarithmic volume mapping, and `currentIndex()` for bounds-checked index access — each with a single clear responsibility.
- Modernized CMake: Replaced manual `AUTOMOC/AUTOUIC/AUTORCC` setup with `qt_standard_project_setup()`, removed dead Qt5 compatibility branches; CMakeLists.txt shrank from 84 lines to ~40 lines.
- Improvement: VideoPlayer.cpp reduced from 513 lines to ~320 lines (~37% reduction), with clearer logical layering and easier future extensibility.

---

## Contact

Author's blog: https://www.zhihu.com/people/13-73-62-89-19

Email: 2022280099@email.szu.edu.cn

This project will continue to be updated with more new features and UI interactions. Welcome to submit issues sharing suggestions for modifications and improvements!

Thank you for your attention and interest!

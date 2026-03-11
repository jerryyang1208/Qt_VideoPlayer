<div align="right">
  <a href="README.md">中文</a>
</div>

<div align="center">

# 🎬 Simple Audio/Video Player Based on Qt 6.8.2

![Qt](https://img.shields.io/badge/Qt-6.8.2-brightgreen)
![C++](https://img.shields.io/badge/C++-17-blue)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

</div>

`Qt_VideoPlayer` is a simple audio/video player developed based on Qt 6.8.2 and C++17, currently successfully running in the Qt Creator environment. It aims to provide a clean and user-friendly graphical interface while demonstrating the practical integration of the Qt framework with multimedia processing libraries. The actual running interface is as follows:

![Program Screenshot](Resource/program-run.png)

> *Note: This project is primarily intended for personal learning and sharing design ideas related to Qt multimedia development, rather than creating a fully-featured, daily-use player that can replace mature products like VLC or PotPlayer.*

Detailed project introduction documentation: https://zhuanlan.zhihu.com/p/1929289732564710634


## Features

- Feature 1: Implemented basic player UI layout and standardized naming conventions.
- Feature 2: Reads media files from local folders into a playlist. Double-click to play audio or video, supporting common audio and video formats.
- Feature 3: Automatically identifies whether the currently playing media is audio or video. When playing audio, the video window is hidden by default. The video window is only displayed when playing video, rendering the output to the window.
- Feature 4: Implemented playback mode switching logic: auto-switch upon completion (with selectable modes: Repeat One, Sequential Play, Shuffle Play), previous/next track button interaction, and current playlist item highlighting.
- Feature 5: Enriched button control logic: control media play/pause, volume adjustment, drag the progress bar to change playback position, and display total media duration and current playback progress.
- Feature 6: Custom window close event handling. When closing a playing video window, the play button, audio, progress bar, and time labels pause simultaneously. Clicking the play button again or double-clicking a video file in the list will redisplay the window, ensuring the video window appears synchronously with playback operations.


## Core Optimizations

To improve code quality, operational efficiency, and user experience, the project has undergone the following three important optimizations:

### Random Number Generation Optimization
- **Original Method**: Used C-style `srand`/`rand` to generate random indices, requiring manual seed setting and being non-thread-safe.
- **Optimized Method**: Uses Qt's `QRandomGenerator::global()->bounded(n)`, eliminating the need for manual initialization.
- **Improvements**: Simplifies random number usage, is thread-safe with higher quality randomness, and makes the code more compliant with Qt standards.

### Playback Completion Detection Optimization
- **Original Method**: Used a timer to poll every second, checking if the playback position was near the total duration (`currentPos >= totalDur - 1000`), which suffered from precision errors and wasted CPU.
- **Optimized Method**: Listens to the `QMediaPlayer::mediaStatusChanged` signal. When the status becomes `QMediaPlayer::EndOfMedia`, it immediately triggers auto-switch to the next track.
- **Improvements**: Judgment is more precise and reliable, eliminates polling overhead, and results in cleaner, more elegant code that follows Qt best practices.

### Error Handling Optimization
- **Original Method**: When playback failed (e.g., due to corrupted files or unsupported formats), the program provided no feedback, leaving the user unaware of the issue.
- **Optimized Method**: Connects to the `errorOccurred` signal. Displays an error message box to inform the user, outputs error information to the console for debugging, and automatically plays the next available file.
- **Improvements**: Significantly enhances user experience, facilitates debugging, and increases program robustness.

  
## Contact Information

Author's Blog: https://www.zhihu.com/people/13-73-62-89-19

Email: 2022280099@email.szu.edu.cn 

This project will continue to be improved and updated with new features and UI interactions. Feel free to submit issues to share your suggestions for modifications and enhancements!

Thank you for your attention and support!

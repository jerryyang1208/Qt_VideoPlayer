<div align="right">
  <a href="README.md">简体中文</a>
</div>

## A Simple Audio-Video Player Based on Qt 6.8.2

<span style="background:#3b82f6;color:white;padding:2px 6px;border-radius:3px;font-size:12px;margin-right:4px;">License MIT</span>
<span style="background:#2563eb;color:white;padding:2px 6px;border-radius:3px;font-size:12px;margin-right:4px;">C++ 17</span>
<span style="background:#1d4ed8;color:white;padding:2px 6px;border-radius:3px;font-size:12px;">Platform Windows</span>

`Qt_VideoPlayer` is a simple audio-video player developed with Qt 6.8.2 and C++17. Currently, it runs successfully only in the Qt Creator environment. It aims to provide a concise and user-friendly graphical interface while demonstrating the integration of Qt framework with multimedia processing libraries. The actual running interface is as follows:

![Program Running Screenshot](Resource/program-run.png)

> *Note: This project is mainly for sharing personal learning experiences in Qt multimedia development, rather than creating a fully functional player that can replace mature products like VLC or PotPlayer for daily use.*

Detailed project introduction and tutorial: https://zhuanlan.zhihu.com/p/1929289732564710634

## Features

- Feature 1: Implemented basic player UI layout and standardized naming.
- Feature 2: Read media files from local folders into the player list; double-click to play audio or video, supporting common audio and video formats.
- Feature 3: Automatically identify whether the current media is audio or video. When playing audio, the video window is hidden by default; when playing video, the window is displayed and the media is rendered to it.
- Feature 4: Implemented button switching logic, including automatic switch to next item after playback, previous/next track button interaction, and highlight tracking of current playing item in the list.
- Feature 5: Rich button control logic, including media play/pause, volume adjustment, progress bar dragging, and duration display.
- Feature 6: Custom window closing event. When closing the video window during playback, the play button, audio, progress bar, and time label will pause synchronously. The window will reappear when clicking the play button again or double-clicking a video file in the list, ensuring the video window synchronizes with playback operations.

## Planned Features

This project will continue to be improved with more new features and UI interactions. The priority order of planned features is as follows:
- New button interactions: Add three switching modes after automatic playback ends (single track loop, sequential play, shuffle play), switchable via button clicks.
- Variable speed playback: Mainly for video files, supporting 0.5x, 2x speed, etc., with progress bar and time label updating accordingly.

## Contact & Inquiries

For more detailed analysis or questions about the project, please refer to the author's Zhihu blog https://www.zhihu.com/people/13-73-62-89-19
Email: 2022280099@email.szu.edu.cn. Welcome to submit issues with suggestions for improvements!

Thank you for your attention and support!

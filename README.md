<div align="right">
  <a href="README_en.md">English</a>
</div>

<div align="center">

# 🎬 基于 Qt 6 开发的简易音视频播放器

[![Qt](https://img.shields.io/badge/Qt-6.10.2-brightgreen)](https://www.qt.io/)
[![C++](https://img.shields.io/badge/C++-17-blue)](https://en.cppreference.com/w/cpp/17)
[![CMake](https://img.shields.io/badge/CMake-3.16+-orange)](https://cmake.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-MIT-yellow)](LICENSE)

</div>

---

## 📖 简介

`Qt_VideoPlayer` 是一款基于 **Qt 6.10.2** 和 **C++17** 开发的简易音视频播放器，采用 **CMake** 构建，支持在 **Qt Creator** 与 **VS Code / Trae** 等主流 IDE 中无缝开发。项目采用 MVC 架构，利用 Qt 信号槽机制实现 UI 与媒体播放状态的松散耦合。

![程序运行截图](Resource/program-run.png)

> *本项目主要用于个人学习 Qt 多媒体开发的设计思路分享，而非打造一个功能完备、可以替代 VLC 或 PotPlayer 等成熟产品的日常播放器。*

📚 项目介绍详细教学文档：[知乎专栏](https://zhuanlan.zhihu.com/p/1929289732564710634)

---

## 📑 目录

- [功能特性](#-功能特性)
- [构建运行](#-构建运行)
- [核心优化](#-核心优化)
- [未来规划](#-未来规划)
- [联系](#-联系)

---

## ✨ 功能特性

### 基础播放

| 功能 | 说明 |
|------|------|
| 📁 文件扫描 | 读取本地文件夹，自动扫描音视频文件添加到播放列表 |
| ▶️ 播放控制 | 播放/暂停、上一首/下一首、拖动进度条 Seek 定位 |
| 🔊 音量调节 | 基于幂律曲线的音量映射，滑块调节与实际听感呈线性关系 |
| ⏱️ 时间显示 | 实时显示当前播放进度与总时长 |
| 📋 播放列表 | 双击列表项播放，当前播放项高亮跟踪 |

### 播放模式

| 模式 | 说明 |
|------|------|
| 🔄 顺序播放 | 按列表顺序依次播放，播完自动切换到下一首 |
| 🎲 随机播放 | 随机选取下一首，带防重复机制避免连续选中同一曲目 |
| 🔂 单曲循环 | 当前曲目播放完毕后自动从头重播 |

### 智能识别

- 自动判断当前播放文件是音频还是视频
- 播放**音频**时，视频窗口显示**内嵌封面图**（通过 `QMediaMetaData` 读取）
- 播放**视频**时，视频窗口显示视频画面
- 窗口关闭事件拦截为隐藏，避免重复创建窗口造成的资源开销与状态丢失

### 快捷键

| 快捷键 | 功能 |
|--------|------|
| `M` | 静音 / 取消静音 |
| `F` | 视频窗口全屏 / 退出全屏 |
| `ESC` | 退出全屏 |

### 多 IDE 支持

项目基于 CMake 构建，一份源码可在 **Qt Creator**、**VS Code / Trae** 等多种 IDE 中编译运行，仅需对应 IDE 的配置文件即可，核心代码无需修改。

---

## 🛠️ 构建运行

### 环境要求

- **Qt 6.10+**（Multimedia、MultimediaWidgets 模块）
- **CMake 3.16+**
- **MinGW 64-bit** 或 **MSVC 2022** 编译器
- **Windows 10/11**

### 构建步骤

```bash
# 1. 克隆项目
git clone https://github.com/yourusername/Qt_VideoPlayer.git
cd Qt_VideoPlayer

# 2. 配置 CMake
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=D:/Qt/6.11.1/mingw_64

# 3. 编译
cmake --build build --config Debug

# 4. 运行
./build/VideoPlayer.exe
```

或直接在 **Qt Creator** / **VS Code（CMake Tools 扩展）** 中打开项目文件夹，IDE 会自动识别 CMake 配置。

---

## 🔧 核心优化

### 架构设计

- 采用 **MVC 架构**：`QStandardItemModel` 管理播放列表数据，`QListView` 负责展示，`VideoPlayer` 类作为控制器
- 基于**信号槽机制**实现 UI 与媒体播放状态的松散耦合与事件驱动更新
- 核心逻辑集中化：`playAtIndex()` 作为唯一播放入口，`switchSong()` 统一上/下一首切换

### 功能优化

- **随机数生成**：使用 `QRandomGenerator::global()->bounded()` 替代 C 风格 `srand/rand`，线程安全且质量更高
- **播放完成判断**：监听 `mediaStatusChanged` 信号替代定时器轮询，消除 CPU 开销
- **错误处理**：连接 `errorOccurred` 信号，弹框提示用户并自动切到下一首可用文件
- **音量映射**：基于人耳感知曲线的幂律映射算法，使滑块调节与实际听感呈线性关系

### 代码质量

- 冗余代码清理：移除未使用的成员变量、冗余头文件、纯转发槽函数
- 重复逻辑合并：`switchSong`、`currentFileName`、`nextIndex` 三次提取消除重复
- 辅助函数抽取：`msToTimeString`、`linearToLogVolume`、`currentIndex` 等职责单一的小工具函数
- CMake 现代化：`qt_standard_project_setup()` 替代手动设置，移除 Qt5 兼容死分支
- **VideoPlayer.cpp 从 513 行精简至约 435 行（减少约 15%），逻辑分层清晰**

### 稳定性

- 内存泄漏修复：独立视频窗口在析构时完整释放
- 错误循环防护：连续错误计数器防止所有文件损坏时死循环
- 状态一致性：打开新目录时先停止播放再清空列表
- 空路径防御：`playAtIndex` 校验文件路径非空
- 防御性编程：边界校验、空指针保护、资源释放顺序保障

---

## 🗺️ 未来规划

| 状态 | 功能 | 简要思路 |
|------|------|---------|
| ✅ | 快捷键支持 | `M` 静音、`F` 全屏切换、`ESC` 退出全屏 |
| ✅ | 媒体元数据展示 | 读取内嵌封面图，音频播放时显示封面 |
| ✅ | 视频全屏切换 | `F` 键全屏 / `ESC` 退出，事件过滤器拦截按键 |
| ⬜ | 播放列表持久化 | 退出时保存播放列表到 JSON，启动时自动恢复 |
| ⬜ | 倍速播放 | 使用 `QMediaPlayer::setPlaybackRate()` 实现 0.5x-2.0x 变速 |
| ⬜ | 歌词显示 | 解析 LRC 文件，同步高亮当前行，配合滚动动画 |
| ⬜ | 频谱可视化 | 获取音频数据做 FFT，用 `QPainter` 绘制柱状频谱 |
| ⬜ | 均衡器 | 多频段 EQ，通过多个滑块独立调节 |
| ⬜ | 播放历史与收藏 | 记录播放历史，支持收藏夹，基于 Model/View 架构扩展 |

---

## 📬 联系

- 📝 博客：[知乎主页](https://www.zhihu.com/people/13-73-62-89-19)
- 📧 邮箱：2022280099@email.szu.edu.cn

本项目会持续完善更新，欢迎提 Issue 分享建议！

感谢你的关注与支持 ✨

# Mass Effect 2 Loading Fixer (2026 Refactored)

![C++](https://img.shields.io/badge/C++-17-blue.svg) ![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg) ![License](https://img.shields.io/badge/License-MIT-green.svg)

A high-performance system utility designed to fix the notorious long loading screen bug in **Mass Effect 2 (Legacy Edition)**. This project is a 2026 refactor of my original 2021 tool, implementing modern C++ standards and safer Windows API interactions.

## 🔴 The Problem: Unreal Engine 3 Sync Stall
Mass Effect 2, built on **Unreal Engine 3**, suffers from a synchronization bottleneck on modern multi-core CPUs. During loading screens, the engine's asset decompression and loading movie synchronization logic often stall as it attempts to synchronize threads across multiple CPU cores. This results in loading times that are significantly longer than they should be on modern hardware.

## 🟢 The Solution: CPU Affinity Optimization
By forcing the game process to run on a single logical core (**Core 1**) during the loading phase, we eliminate the thread-switching overhead and synchronization race conditions. 

**This utility automates the process:**
1. It identifies the game path via a local `.ini` file or the Windows Registry.
2. It launches the game in a suspended state to set the **CPU Affinity Mask to 2 (0010 in binary)**.
3. It monitors for a user-triggered hotkey (**F9**) to restore all CPU cores once loading is finished, ensuring maximum FPS during gameplay.

## 🛠️ 2026 Refactor Highlights
Compared to the 2021 version, this refactor introduces several professional-grade improvements:
* **Dynamic Registry Handling:** Replaced fixed-size buffers with dynamic memory allocation to prevent buffer overflows when reading long file paths from Windows Registry.
* **Modern C++17 Standards:** Utilizes `<filesystem>` for robust, cross-platform path manipulation and directory navigation.
* **Native Windows API:** Switched from `system()` calls to `CreateProcessA` for direct process management, providing better handle control and security.
* **Real-time Monitoring:** Implements `GetAsyncKeyState` for non-blocking background hotkey detection even when the game is in full-screen focus.

**Workflow:**
1. **Detects** the game via `me2lsf.ini` or Registry.
2. **Launches** with CPU Affinity Mask set to 2.
3. **Restores** power when the user presses **[F9]** after the loading screen.

## 🚀 How to Build
```bash
g++ -O3 -std=c++17 main.cpp -o me2lsf.exe -luser32
```

## 📖 Usage
* Create and define your game path in `me2lsf.ini`, or let the program locate the Windows registry
* Run `me2lsf.exe` (Note: The .ini file takes priority. Only create it if you intend to manually provide a path, as it overrides the automatic Registry detection).
* Press **F9** once your game finish the loading screen to restore performance.

---
**Developed by [0xRobert](https://github.com/zahanzo)**
*Systems Analysis and Development Student | Low-level Programming Enthusiast*
# Image Viewer with Brightness Control (Qt Widgets)

A clean Qt Widgets application that loads an image and lets you adjust its brightness in real time using a horizontal slider.

## Features (meets the task requirements)
- **Image Holder**: Displays grayscale or color images (PNG/JPG and more supported by Qt).
- **Default Image**: App starts with a built‑in generated gradient image, so you can test immediately.
- **Brightness Adjustment**: Slider range **-100 to +100** with **live updates** via the signal–slot mechanism.
- **Reset Button**: Resets the slider to 0 and restores the original image.

## Bonus
- **Load New Image**: `File → Open Image…` (also on toolbar, `Ctrl/Cmd+O`) uses `QFileDialog`.
- **Current Value Display**: Numeric label next to the slider shows the current brightness delta.
- **Smooth Updates**: Uses fast scanline access and `Qt::SmoothTransformation` for scaled previews; minimal flicker.

## Tech & Code Quality
- **Qt Widgets**: `QMainWindow`, `QLabel`, `QSlider`, `QPushButton`, menus and toolbar.
- **Image Processing**: Pure Qt via `QImage`/`QPixmap` (no OpenCV required).
- **Modular Structure**: Separate `ImageProcessor` for processing and `MainWindow` for UI.
- **Clean CMake**: Builds with **Qt 6** (preferred) or **Qt 5** automatically.

## Project Structure
```
qt_image_brightness/
├── CMakeLists.txt
└── src/
    ├── ImageProcessor.cpp
    ├── ImageProcessor.h
    ├── MainWindow.cpp
    ├── MainWindow.h
    └── main.cpp
```

## Build Instructions

### Prerequisites
- **Qt 6 (Widgets)** recommended, or **Qt 5.15+**.
- **CMake 3.16+**
- A C++17 compiler (GCC/Clang/MSVC).

### Configure & Build

```bash
# 1) Configure (Qt 6 example)
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64

# If using Qt 5:
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/5.15.2/gcc_64

# 2) Build
cmake --build build --config Release

# 3) Run
./build/ImageBrightnessViewer   # (or ImageBrightnessViewer.exe on Windows)
```

> **Tip:** On Windows with Qt installed via the Maintenance Tool, your `CMAKE_PREFIX_PATH` often looks like:
> `C:/Qt/6.7.2/msvc2019_64` or `C:/Qt/5.15.2/msvc2019_64`.

## Notes & Assumptions
- Brightness adjustment is a **simple additive** operation on RGB channels with clamping to `[0,255]`; alpha is preserved.
- All images are normalized to `QImage::Format_ARGB32` for speed and simplicity.
- The preview scales to fit the window while keeping aspect ratio; the original backing image is preserved, so resets are lossless.

## Extending (Ideas)
- Add **contrast** and **gamma** sliders.
- Add **drag-and-drop** to load images.
- Add a **status bar** histogram preview.
- Add an **"Auto"** button that computes optimal brightness using image statistics.

---

Made for the provided *QT ASSESSMENT*.
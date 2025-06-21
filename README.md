# OpenGL ArUco Detector

A real-time ArUco marker detection application built with OpenGL, ImGui, and OpenCV. This educational project provides an interactive interface for detecting and tracking ArUco markers using your computer's camera.

## Features

- **Real-time ArUco Detection**: Detects ArUco markers in live camera feed
- **Interactive UI**: Rich ImGui-based interface with dual-panel layout
- **Multiple Dictionary Support**: Supports various ArUco dictionaries (4x4, 5x5, 6x6, 7x7) and AprilTag variants
- **Marker Generation**: Generate and display custom ArUco markers
- **Statistics Tracking**: Monitor detection frequency and performance metrics
- **Camera Integration**: Real-time camera feed processing

## How It Works

The application consists of three main windows:

1. **Scene Window**: Displays the live camera feed with detected ArUco markers overlaid. Shows real-time detection statistics including:
   - Total detections per marker
   - Detections per second
   - Current frame detection count

2. **ArUco Window**: Allows you to:
   - Select different ArUco dictionaries
   - Choose marker IDs
   - Generate and preview custom markers

3. **Camera Window**: Provides camera selection interface and a preview of the received images.

The system uses OpenCV's ArUco detector to identify markers in each camera frame and provides visual feedback with bounding boxes and marker IDs.

## System Requirements

- **OS**: Linux (tested on Ubuntu 20.04.6 LTS)
- **Compiler**: GCC 9.4.0 or later
- **Build System**: CMake 3.16.3 or later
- **Dependencies**: OpenGL, OpenCV, ImGui, GLFW

## Dependencies

- **OpenCV**: Computer vision library for ArUco detection
- **OpenGL**: Graphics rendering
- **ImGui**: User interface framework
- **GLFW**: Window management and OpenGL context
- **GLAD**: OpenGL loading library

## Compilation

1. **Install Dependencies**:

   ```bash
   sudo apt update
   sudo apt install build-essential cmake
   sudo apt install libopencv-dev libglfw3-dev
   ```

2. **Clone and Build**:

   ```bash
   git clone https://github.com/MertKures/opengl_aruco_detector.git
   cd opengl_aruco_detector
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run the Application**:

   ```bash
   ./src/opengl_aruco_detector
   ```

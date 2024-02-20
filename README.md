Compressing data in C++ using wxWidgets.

## Project Overview

- A utility application for compressing and decompressing files.
- Provides a graphical user interface (GUI) using wxWidgets.
- Supports both zip and unzip functionalities.
- Easy-to-use interface for selecting files and directories.
- Real-time progress updates during compression and decompression.
- Handles large files efficiently.


## Technology Aspects

- Written in C++ with wxWidgets for the GUI.
- Uses wxZipOutputStream and wxZipInputStream for handling zip files.
- Implements custom traversers for directory handling.
- Includes progress dialogs for user feedback during operations.
- Cross-platform compatibility with Windows, Mac, and Linux.
- Utilizes modern C++ features for better performance and readability.
- Modular design for easy maintenance and extension.
- Comprehensive error handling and user notifications.

## Prerequisites

- C++ compiler supporting C++17 or later.
- wxWidgets library installed on your system.
- CMake for building the project.
- Git for cloning the repository.

## Installation

1. Clone the repository:

   ```sh
   git clone https://github.com/yourusername/wx_zip_tutorial.git
   cd wx_zip_tutorial
   ```

2. Create a build directory and navigate to it:

   ```sh
   mkdir build
   cd build
   ```

3. Configure the project using CMake:

   ```sh
   cmake ..
   ```

4. Build the project:

   ```sh
   cmake --build .
   ```

5. Run the application:
   ```sh
   ./wx_zip_tutorial
   ```

## References

- [wxWidgets Documentation](https://docs.wxwidgets.org/)
- [CMake Documentation](https://cmake.org/documentation/)
- [C++ Reference](https://en.cppreference.com/w/)

---

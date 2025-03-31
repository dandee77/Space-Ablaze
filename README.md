# Space Ablaze

## Installation Guide

### **Required Dependencies**
To build and run **Space Ablaze**, ensure you have the following installed:

- **CMake** (Minimum version 3.10)
- **C++ Compiler**:
  - **MinGW-w64** (For Windows users who prefer GCC)
  - **Microsoft Visual Studio (MSVC)** (For Windows users using Visual Studio)
  - **GCC** (For Linux users)
- **Make** (Required for Linux and MinGW users)

### **Installation Steps**

#### **1. Install CMake**
- **Windows**: [Download CMake](https://cmake.org/download/) and install it.
- **Linux (Ubuntu/Debian)**:
  ```sh
  sudo apt update && sudo apt install cmake
  ```
- **Mac (Homebrew)**:
  ```sh
  brew install cmake
  ```

#### **2. Install a C++ Compiler**
- **For Windows (MinGW-w64)**:
  - Install [MinGW-w64](https://www.mingw-w64.org/) and add its `bin` folder to your PATH.
  - Verify installation:
    ```sh
    g++ --version
    ```
- **For Windows (Visual Studio MSVC)**:
  - Install [Visual Studio](https://visualstudio.microsoft.com/) with the "Desktop Development with C++" workload.
  - Use the **Visual Studio Developer Command Prompt**.
- **For Linux (GCC)**:
  ```sh
  sudo apt install g++
  ```

### **3. Build and Run the Project**

#### **Using MinGW (Windows)**:
```sh
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build .
./SpaceAblaze  # Run the executable
```

#### **Using Visual Studio (MSVC on Windows)**:
```sh
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
cmake --build .
```
- Open the generated `.sln` file in Visual Studio and build/run the project.

#### **Using GCC (Linux/macOS)**:
```sh
mkdir build
cd build
cmake ..
cmake --build .
./SpaceAblaze  # Run the executable
```



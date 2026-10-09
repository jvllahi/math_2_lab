# Laboratory 1 · Mathematics II · 2026

1. Download the repository using **Code -> Download ZIP** and extract it.
2. Install **CMake 3.24 or later** and a **C++17** compiler. On Windows,
   install Visual Studio with "Desktop development with C++"; on macOS,
   install Xcode command line tools; on Linux, install build tools and
   OpenGL and SDL3 development dependencies.
3. Open a terminal in the project folder and run:

```sh
cmake --preset default
cmake --build --preset default --parallel
```

The first build needs Internet access to download SDL3 and Dear ImGui.

4. Run the application:

**Windows (PowerShell):**

```powershell
.\build\bin\lab1.exe
```

**macOS/Linux:**

```sh
./build/bin/lab1
```

After modifying the code, repeat the build command and launch the
application again.

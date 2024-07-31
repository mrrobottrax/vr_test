To build:

Windows:
You can use cmake to build it, but I prefer to just open the directory using Visual Studio. It should automatically detect that it's a CMake project and use VS integrated CMake support.

Other platforms:
I have tested this on 0 other platforms, but it should in theory work once you modify the CMake files a little. Mostly src/CMakeLists.txt. It needs support for other platforms (currently anything not win32 launches in console mode, and uses the wrong openvr library).
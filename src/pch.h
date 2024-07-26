#pragma once

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_win32.h"

#ifdef _WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#endif // _WINDOWS

#include <GL/glew.h>
#include <GL/wglew.h>

#include <openvr.h>

#include <iostream>
#include <combaseapi.h>
#include <locale>
#include <codecvt>
#include <string>
#include <format>
#include <functional>
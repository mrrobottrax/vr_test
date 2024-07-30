#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <SDL2/SDL.h>
#include <glad/glad.h>

//#include <openvr.h>

// std

#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <format>

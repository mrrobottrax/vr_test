#pragma once

// todo: is this stuff just Windows?
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "SDL2/SDL.h"

//#include <openvr.h>

//#include <GL/glew.h>
//#include <GL/wglew.h>

#include <iostream>
#include <combaseapi.h>
#include <locale>
#include <codecvt>
#include <string>
#include <format>
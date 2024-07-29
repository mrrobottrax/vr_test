#pragma once

//// so we get the ids
//#ifdef _WINDOWS
//#include <resource.h>
//#endif // _WINDOWS
//
//constexpr auto RCT_SHADER = L"Shader";

class FileManager
{
public:
	// resources are built into the executable
	static const char *LoadResourceBytes(int id, const wchar_t *type);
};
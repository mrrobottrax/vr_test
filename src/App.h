#pragma once

#ifdef _WINDOWS
#include <windows/WinApp.h>
inline WinApp& App()
{
	static WinApp app;
	return app;
}
#endif // _WINDOWS
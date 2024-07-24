#pragma once

#ifdef _WINDOWS
#include <windows/win.h>
inline WinApp& App()
{
	static WinApp app;
	return app;
}
#endif // _WINDOWS
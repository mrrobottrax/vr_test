#pragma once

#include <gl/GlInstance.h>
#include "Window.h"

class WinApp
{
private:
	HINSTANCE m_hInstance = NULL;
	Window m_mainWindow;
	GlInstance m_glInstance;

public:
	bool shouldQuit = false;

public:
	WinApp() :
		m_mainWindow(Window()),
		m_glInstance(GlInstance())
	{}

public:
	void Init();
	void Cleanup();
	void MainLoop();

	void CreateConsole();
	void DestroyConsole(bool pause = false);

	inline Window& MainWindow() { return m_mainWindow; };
};
#pragma once

#include <gl/GlInstance.h>

class Window
{
private:
	int m_width;
	int m_height;

public:
	HWND hWnd;

	void Init(HINSTANCE hInstance, int width, int height);

	int Width() const { return m_width; }
	int Height() const { return m_height; }

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

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

	Window& MainWindow() { return m_mainWindow; };
};
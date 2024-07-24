#include <pch.h>
#include "Window.h"

void Window::Init(HINSTANCE hInstance, int width, int height)
{
	constexpr wchar_t CLASS_NAME[] = L"MainWindow";

	// set up window class
	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClassW(&wc);

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	m_width = width;
	m_height = height;
	RECT rect = { 0, 0, width, height };
	AdjustWindowRectEx(&rect, dwStyle, FALSE, 0);

	hWnd = CreateWindowExW(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Game",                        // Window text
		dwStyle,						// Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,

		NULL,       // Parent window    
		NULL,		// Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
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
		this        // Additional application data
	);

	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	Window *pThis = reinterpret_cast<Window *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (uMsg)
	{
		case WM_NCCREATE:
		{
			LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
			pThis = static_cast<Window *>(lpcs->lpCreateParams);

			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED)
			{
				pThis->m_width = LOWORD(lParam);
				pThis->m_height = HIWORD(lParam);
			}
			break;
		}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
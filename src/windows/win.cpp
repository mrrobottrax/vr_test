#include <pch.h>
#include <app.h>
#include <engine/EngineInstance.h>
#include <gl/GlInstance.h>

#include "win.h"

// Window

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

// End Window

// WinApp

void WinApp::Init()
{
	m_mainWindow.Init(m_hInstance, 800, 600);
	CreateConsole();

	Engine().Init();
}

void WinApp::Cleanup()
{
	Engine().Cleanup();

	DestroyConsole(true);
}

void WinApp::MainLoop()
{
	// run the message loop.
	MSG msg = {};
	while (!shouldQuit)
	{
		// todo: stop processing messages after they take too long

		while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);

			if (msg.message == WM_QUIT)
			{
				shouldQuit = true;
			}
		}

		Engine().Frame();
	}

	std::cout << "DONE\n";
}

void WinApp::CreateConsole()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
}

void WinApp::DestroyConsole(bool pause)
{
	if (pause) std::system("pause");
	FreeConsole();
}

// End WinApp

// WinMain

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PWSTR pCmdLine,
	_In_ int nCmdShow
)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		App().Init();
		App().MainLoop();
		App().Cleanup();
	}
	catch (const std::exception& err)
	{
		std::string s = std::string("Error: ") + std::string(err.what());

		std::wstring ws = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(s);
		MessageBoxW(NULL, ws.c_str(), L"Error!", MB_ICONERROR | MB_OK);

		system("pause");
		::exit(EXIT_FAILURE);
	}

	::exit(EXIT_SUCCESS);
}
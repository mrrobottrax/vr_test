#include <pch.h>
#include "App.h"
//#include <engine/EngineInstance.h>
//#include <gl/GlInstance.h>

// MainApp

void MainApp::Init()
{
	//m_mainWindow.Init(m_hInstance, 800, 600);
	//CreateConsole();

	//Engine().Init();
}

void MainApp::Cleanup()
{
	//Engine().Cleanup();

	//DestroyConsole(true);
}

void MainApp::MainLoop()
{
	// run the message loop.
	//MSG msg = {};
	//while (!shouldQuit)
	//{
	//	// todo: stop processing messages after they take too long

	//	while (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessageW(&msg);

	//		if (msg.message == WM_QUIT)
	//		{
	//			shouldQuit = true;
	//		}
	//	}

	//	//Engine().Frame();
	//}

	std::cout << "DONE\n";
}

void MainApp::CreateConsole()
{
	/*AllocConsole();
	freopen("CONOUT$", "w", stdout);*/
}

void MainApp::DestroyConsole(bool pause)
{
	(void)pause;
	/*if (pause) std::system("pause");
	FreeConsole();*/
}

// End MainApp

// Main function

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("SDL_Init failed (%s)", SDL_GetError());
		return 1;
	}

	SDL_Quit();
	return 0;
}

//int WINAPI wWinMain(
//	_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ PWSTR pCmdLine,
//	_In_ int nCmdShow
//)
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//
//	try
//	{
//		App().Init();
//		App().MainLoop();
//		App().Cleanup();
//	}
//	catch (const std::exception& err)
//	{
//		std::string s = std::string(err.what());
//
//		std::cout << s << std::endl;
//
//		std::wstring ws = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(s);
//		MessageBoxW(NULL, ws.c_str(), L"Error!", MB_ICONERROR | MB_OK);
//
//		system("pause");
//		::exit(EXIT_FAILURE);
//	}
//
//	::exit(EXIT_SUCCESS);
//}
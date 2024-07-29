#include <pch.h>
#include "App.h"
#include <engine/EngineInstance.h>

void MainApp::Init()
{
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		throw std::runtime_error(std::format("SDL_Init failed {}", SDL_GetError()));
	}

	m_pMainWindow = SDL_CreateWindow(
		"VR Game",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
	);

	if (m_pMainWindow == nullptr)
	{
		throw std::runtime_error(std::format("SDL_CreateWindow failed {}", SDL_GetError()));
	}

	Engine().Init();
}

void MainApp::Cleanup()
{
	Engine().Cleanup();

	SDL_Quit();
}

void MainApp::MainLoop()
{
	// run the message loop.
	while (m_isRunning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					m_isRunning = false;
					break;
				case SDL_KEYDOWN:
					// toggle fullscreen
					if (event.key.keysym.sym == SDLK_F11)
					{
						bool isFullscreen = SDL_GetWindowFlags(m_pMainWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP;
						SDL_SetWindowFullscreen(m_pMainWindow, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
					}
					break;
				default:
					break;
			}
		}

		Engine().Frame();
	}
}

// entry point
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	// memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	try
	{
		App().Init();
		App().MainLoop();
		App().Cleanup();
	}
	catch (const std::exception &err)
	{
		std::string s = std::string(err.what());

		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, s.c_str());
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", s.c_str(), nullptr);

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
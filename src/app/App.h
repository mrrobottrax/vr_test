#pragma once

//#include <gl/GlInstance.h>

class MainApp
{
private:
	//SDL_Window* m_pMainWindow;
	//GlInstance m_glInstance;

public:
	bool shouldQuit = false;

public:
	MainApp() //:
		//m_glInstance(GlInstance())
	{}

public:
	void Init();
	void Cleanup();
	void MainLoop();

	void CreateConsole();
	void DestroyConsole(bool pause = false);

	//inline Window& MainWindow() { return m_mainWindow; };
};

inline MainApp& App()
{
	static MainApp app;
	return app;
}
#pragma once

class MainApp
{
private:
	bool m_isRunning = true;
	SDL_Window *m_pMainWindow = nullptr;

public:
	void Init();
	void Cleanup();
	void MainLoop();

	inline void Quit() { m_isRunning = false; }

	inline SDL_Window *MainWindow() { return m_pMainWindow; };
};

inline MainApp &App()
{
	static MainApp app;
	return app;
}
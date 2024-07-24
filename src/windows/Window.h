#pragma once

class Window
{
private:
	int m_width;
	int m_height;

public:
	HWND hWnd;

	void Init(HINSTANCE hInstance, int width, int height);

	inline int Width() const { return m_width; }
	inline int Height() const { return m_height; }

private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
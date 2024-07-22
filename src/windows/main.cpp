#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <openvr.h>

#include <iostream>
#include <combaseapi.h>
#include <locale>
#include <codecvt>
#include <string>
#include <format>

#include <GL/glew.h>
#include <GL/wglew.h>

#ifdef GL_ASSERTS
#define glAssertSkip(expr, expected) if (expr != expected) { throw std::runtime_error(std::format("GL expected {} got {:x} \nGL: {}", #expected, expr, (const char*)gluErrorString(glGetError()))); }
#define glCheckErrors() if (glGetError()) { gluErrorString(glGetError()); }
#else
#define glAssertSkip(expr, expected)
#define glCheckErrors()
#endif // GL_ASSERTS

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

class Console
{
public:
	void Init();
	void Cleanup();
};

class VrInstance
{
public:
	vr::IVRSystem *pVrSys;

	void Init();
	void Cleanup();
};

class GlInstance
{
private:
	GLuint m_leftEyeRenderbuffer;
	GLuint m_leftEyeFramebuffer;

public:
	void Init(Window *pWindow);
	void Cleanup();

	void RenderFrame();
};

class WinApp
{
private:
	HINSTANCE m_hInstance = NULL;

	Window *m_pMainWindow = nullptr;
	Console *m_pConsole = nullptr;
	VrInstance *m_pVR = nullptr;
	GlInstance *m_pGL = nullptr;

public:
	bool shouldQuit = false;

	void Init();
	void Cleanup();
	void MainLoop();

	Window *MainWindow() const { return m_pMainWindow; }
};

static WinApp& App()
{
	static WinApp app;
	return app;
};

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

// Console

void Console::Init()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
}

void Console::Cleanup()
{
	std::system("pause");
	FreeConsole();
}

// GlInstance

void GlInstance::Init(Window *pWindow)
{
	// initialize windows gl
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	HDC hdc = GetDC(pWindow->hWnd);

	int pixelFormat;
	PIXELFORMATDESCRIPTOR pixelFormatDesc;

	/* initialize bits to 0 */
	memset(&pixelFormatDesc, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pixelFormatDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDesc.nVersion = 1;
	pixelFormatDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDesc.cColorBits = 8;
	pixelFormatDesc.cAlphaBits = 0;
	pixelFormatDesc.cDepthBits = 0;

	pixelFormat = ChoosePixelFormat(hdc, &pixelFormatDesc);
	SetPixelFormat(hdc, pixelFormat, &pixelFormatDesc);

	HGLRC hlgrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hlgrc);

	// initialize glew
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		throw std::runtime_error((const char*)glewGetErrorString(error));
	}

	// create frame buffer
	glGenFramebuffers(1, &m_leftEyeFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_leftEyeFramebuffer);

	// create render buffer
	glGenRenderbuffers(1, &m_leftEyeRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_leftEyeRenderbuffer);
	uint32_t width, height;
	vr::VRSystem()->GetRecommendedRenderTargetSize(&width, &height);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, width, height);

	// bind the two
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_leftEyeRenderbuffer);

	// check
	glCheckErrors();
	glAssertSkip(glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER), GL_FRAMEBUFFER_COMPLETE);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GlInstance::Cleanup()
{
	CoUninitialize();
}

void GlInstance::RenderFrame()
{
	vr::VRCompositor()->WaitGetPoses(nullptr, 0, nullptr, 0);

	uint32_t width, height;
	vr::VRSystem()->GetRecommendedRenderTargetSize(&width, &height);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_leftEyeFramebuffer);

	glViewport(0, 0, width, height);
	glClearColor(0, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	vr::Texture_t tex{};
	tex.handle = (void*)(uintptr_t)m_leftEyeRenderbuffer;
	tex.eType = vr::TextureType_OpenGL;
	tex.eColorSpace = vr::ColorSpace_Gamma;

	vr::VRCompositor()->Submit(vr::Eye_Left, &tex, nullptr, vr::Submit_GlRenderBuffer);
	vr::VRCompositor()->Submit(vr::Eye_Right, &tex, nullptr, vr::Submit_GlRenderBuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// copy to main window
	// todo: pofile viewport
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_leftEyeFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, App().MainWindow()->Width(), App().MainWindow()->Height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glFinish();

}

// VrInstance

void VrInstance::Init()
{
	vr::EVRInitError error;
	vr::EVRApplicationType appType = vr::VRApplication_Scene;

	pVrSys = vr::VR_Init(&error, appType);
}

void VrInstance::Cleanup()
{
	vr::VR_Shutdown();
}

// WinApp

void WinApp::Init()
{
	m_pMainWindow = new Window();
	m_pMainWindow->Init(m_hInstance, 800, 600);

	m_pConsole = new Console();
	m_pConsole->Init();

	m_pVR = new VrInstance();
	m_pVR->Init();

	m_pGL = new GlInstance();
	m_pGL->Init(m_pMainWindow);
}

void WinApp::Cleanup()
{
	m_pVR->Cleanup();
	m_pConsole->Cleanup();
	m_pGL->Cleanup();

	delete m_pMainWindow;
	delete m_pVR;
	delete m_pConsole;
	delete m_pGL;
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

		m_pGL->RenderFrame();
	}

	std::cout << "DONE\n";
}

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
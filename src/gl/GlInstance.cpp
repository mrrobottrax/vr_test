#include <pch.h>
#include "GlInstance.h"
#include <App.h>

#ifdef GL_ASSERTS
#define glAssertSkip(expr, expected) if (expr != expected) { throw std::runtime_error(std::format("GL expected {} got {:x} \nGL: {}", #expected, expr, (const char*)gluErrorString(glGetError()))); }
#define glCheckErrors() if (glGetError()) { gluErrorString(glGetError()); }
#else
#define glAssertSkip(expr, expected)
#define glCheckErrors()
#endif // GL_ASSERTS

void GlInstance::Init()
{

#ifdef _WINDOWS
	// initialize windows gl
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	HDC hdc = GetDC(App().MainWindow().hWnd);

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
#endif // _WINDOWS

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
#ifdef _WINDOWS
	CoUninitialize();
#endif // _WINDOWS
}

void GlInstance::RenderFrame()
{
	vr::TrackedDevicePose_t renderPose;
	vr::VRCompositor()->WaitGetPoses(&renderPose, 1, nullptr, 0);

	uint32_t width, height;
	vr::VRSystem()->GetRecommendedRenderTargetSize(&width, &height);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_leftEyeFramebuffer);

	glViewport(0, 0, width, height);
	glClearColor(0, 1, 1, 1);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vr::HmdMatrix44_t projLeft = vr::VRSystem()->GetProjectionMatrix(vr::Eye_Left, 0.01f, 1000.0f);

	vr::Texture_t tex{};
	tex.handle = (void*)(uintptr_t)m_leftEyeRenderbuffer;
	tex.eType = vr::TextureType_OpenGL;
	tex.eColorSpace = vr::ColorSpace_Gamma;

	// todo: profile rendering eyes on seperate rts and submitting at once, or sequencially on same rt
	vr::VRCompositor()->Submit(vr::Eye_Left, &tex, nullptr, vr::Submit_GlRenderBuffer);
	vr::VRCompositor()->Submit(vr::Eye_Right, &tex, nullptr, vr::Submit_GlRenderBuffer);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// copy to main window
	// todo: pofile viewport
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_leftEyeFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, App().MainWindow().Width(), App().MainWindow().Height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glFinish();
}
#include <pch.h>
#include "GlInstance.h"
#include <App.h>

float vertices[] = {
	0, 0, 0,
	0, 1, 0,
	1, 1, 0
};

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

	// init render targets
	m_leftEyeFramebuffer.Init();
	m_rightEyeFramebuffer.Init();

	// create vertex array
	glGenVertexArrays(1, &m_vertexArray);
	glBindVertexArray(m_vertexArray);

	// create vertex buffer
	glGenBuffers(1, &m_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// set vertex format
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void GlInstance::Cleanup()
{
#ifdef _WINDOWS
	CoUninitialize();
#endif // _WINDOWS
}

void GlInstance::RenderScene(vr::EVREye eye)
{
	// setup
	RenderTarget& rt = eye == vr::Eye_Right ? m_rightEyeFramebuffer : m_leftEyeFramebuffer;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rt.FramebufferName());
	glViewport(0, 0, rt.Width(), rt.Height());

	// clear
	glClearColor(0, 1, 1, 1);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// get projection matrix
	//vr::HmdMatrix44_t projLeft = vr::VRSystem()->GetProjectionMatrix(eye, 0.01f, 1000.0f);

	// draw test triangle
	glBindVertexArray(m_vertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GlInstance::RenderFrame()
{
	vr::TrackedDevicePose_t renderPose;
	vr::VRCompositor()->WaitGetPoses(&renderPose, 1, nullptr, 0);

	RenderScene(vr::Eye_Left);
	RenderScene(vr::Eye_Right);

	// submit textures
	vr::Texture_t leftEye{};
	leftEye.handle = (void*)(uintptr_t)m_leftEyeFramebuffer.RenderbufferName();
	leftEye.eType = vr::TextureType_OpenGL;
	leftEye.eColorSpace = vr::ColorSpace_Gamma;

	vr::Texture_t rightEye{};
	rightEye.handle = (void*)(uintptr_t)m_rightEyeFramebuffer.RenderbufferName();
	rightEye.eType = vr::TextureType_OpenGL;
	rightEye.eColorSpace = vr::ColorSpace_Gamma;

	// todo: profile rendering eyes on seperate rts and submitting at once, or sequencially on same rt
	vr::VRCompositor()->Submit(vr::Eye_Left, &leftEye, nullptr, vr::Submit_GlRenderBuffer);
	vr::VRCompositor()->Submit(vr::Eye_Right, &rightEye, nullptr, vr::Submit_GlRenderBuffer);

	// copy to main window
	// todo: pofile with/without viewport
	//glViewport()
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_leftEyeFramebuffer.FramebufferName());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(
		0, 0,
		m_leftEyeFramebuffer.Width(), m_leftEyeFramebuffer.Height(),
		0, 0,
		App().MainWindow().Width(), App().MainWindow().Height(),
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glFinish();
}